// libraries used
#include <iostream>
#include "scanner.hpp"

// static array to hold human-readable names for token types
std::string Scanner::tokenNames[] = {
    "n/a", "<EOF>", "BIT", "CLONE", "DUB", "POP", "MUL", "DIV", "PLUS", 
    "MINUS", "STRING", "RHEAD", "LHEAD", "EHEAD", "BODY", "DUCK_FR", 
    "DUCK_FL", "DUCK_ER", "LEG", "ARROW", "KEYWORD", "ID"
};

// constructor for scanner: initializing the input buffer
// setting the starting point
Scanner::Scanner(int length, char* buffer) : length_(length), 
                buffer_(buffer), index_(0) {
    lookahead_ = buffer_[index_]; // set the initial lookahead character
}

// advancing the current pos in the input buffer and updating the
// lookahead character
void Scanner::consume() {
    // if we have not reached the end of the file
    if(index_ < length_) {
        index_++;  // move to the next character
        // update the lookahead character
        lookahead_ = (index_ >= length_) ? EOF : buffer_[index_];
    }
}

// matching the current char against the expected char and consuming
// if it matches
void Scanner::match(char x) {
    if(lookahead_ == x) {
        consume();  // consume the match character and move to next
    }
}

// function to check if the lookahead 
// character is an alphabet or underscore
bool Scanner::isAlpha() const {
    return (lookahead_ >= 'a' && lookahead_ <= 'z') || 
           (lookahead_ >= 'A' && lookahead_ <= 'Z') || 
           lookahead_ == '_';
}

// function to check if the lookahead character is an alphabet
bool Scanner::isLetter() const {
    return (lookahead_ >= 'a' && lookahead_ <= 'z') || 
           (lookahead_ >= 'A' && lookahead_ <= 'Z');
}

// function to handle whitespace characters
void Scanner::whitespace() {
    while (lookahead_ == ' ' || lookahead_ == '\t' || 
           lookahead_ == '\n' || lookahead_ == '\r') {
        consume(); // calling consume
    }
}

// function to get an identifier or keyword
Token Scanner::name() {
    std::string lexeme;
    while (isAlpha()) { // accumulating alpha chars
        lexeme.push_back(lookahead_);
        consume(); // calling consume
    }

    // check if the lexeme matches any keywords
    if (lexeme == "hey" || lexeme == "vous" || lexeme == "LeQuack") {
        return Token(tok_keyword, lexeme); // return as keyword
    }
    // otherwise, it's an identifier
    return Token(tok_id, lexeme);
}

// scanning and returning next token
Token Scanner::nextToken() {
    // calling whitespace function
    whitespace();

    if (index_ >= length_) {
        return Token(tok_eof, "<EOF>");  // EOF token
    }

    std::string lexeme(1, lookahead_);

    // match for different token types based
    // to the given lookahead character
    switch (lookahead_) {
        // matching the start of a BIT token "./"
        case '.':
            match(lookahead_); // matching lookahead char
            if (lookahead_ == '/') {
                match(lookahead_);
                return Token(tok_bit, "./");
            }
            break;
        // matching start of a CLONE token ",/"
        case ',':
            match(lookahead_);
            if (lookahead_ == '/') {
                match(lookahead_);
                return Token(tok_clone, ",/");
            }
            break;
        // matching a DUB token "!"
        case '!':
            match(lookahead_);
            return Token(tok_dub, "!");
        // matching a POP token "?"
        case '?':
            match(lookahead_);
            return Token(tok_pop, "?");
        // matching a MUL token "*"
        case '*':
            match(lookahead_);
            return Token(tok_mul, "*");
        // matching the start of a DIV token "//"
        case '/':
            match(lookahead_);
            if (lookahead_ == '/') {
                match(lookahead_);
                return Token(tok_div, "//");
            }
            break;
        // matching a PLUS token "+"
        case '+':
            match(lookahead_);
            return Token(tok_plus, "+");
        // matching the start of a MINUS or ARROW token
        case '-':
            match(lookahead_);
            if (lookahead_ == '>') {
                match(lookahead_);
                return Token(tok_arrow, "->");
            } else {
                return Token(tok_minus, "-");
            }
        // matching a STRING token "%"
        case '%':
            match(lookahead_);
            return Token(tok_string, "%");
        // matching the start of a RHEAD or EHEAD token
        case '@':
            match(lookahead_);
            if (lookahead_ == '<') {
                match(lookahead_);
                return Token(tok_rhead, "@<");
            } else if (lookahead_ == '=') {
                match(lookahead_);
                return Token(tok_ehead, "@=");
            }
            break;
        // matching the start of an LHEAD token
        case '>':
            match(lookahead_);
            if (lookahead_ == '@') {
                match(lookahead_);
                return Token(tok_lhead, ">@");
            }
            break;
        // matching the start of a BODY token "###"
        case '#':
            match(lookahead_);
            if (lookahead_ == '#' && buffer_[index_ + 1] == '#') {
                match(lookahead_);
                match(lookahead_);
                return Token(tok_body, "###");
            }
            break;
        // case for letters that might start an identifier or keyword
        default:
            if (isLetter()) {
                return name();  // handle identifiers and keywords
            }
            // handling error for unrecognized character if needed
        std::cout << "[SCANNER] Unknown character found '" 
                  << lookahead_ << "'" << std::endl;
        exit(0);
    }
    return Token(tok_na, lexeme);  // return 'NA' token if match !found
}