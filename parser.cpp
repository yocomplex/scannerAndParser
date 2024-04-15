#include "parser.hpp"

// constructor that initializes the parser with a list of tokens
// and setting the index to start at 0
Parser::Parser(std::vector<Token>& tokens) : tokens_(tokens), index_(0) {
    consume(); // moves to the first token to start parsing
}

// function that checks if the current token matches the expected type
// and consumes it, otherwise exits
void Parser::match(int x) {
    // if current token matches expected type, consume
    if (lookahead_.getType() == x)
        consume();
    else {
        // if the current token doesn't match, print error and exit
        std::cout << "[PARSER]  Expecting " << Scanner::tokenNames[x] 
                  << ", but found '" << lookahead_.toString()
                  << "'" << std::endl;
        exit(0);
    }
}

// function to check if the current token type matches the expected type
bool Parser::check(TokenType type) {
    if (lookahead_.getType() == type) {
        return true;
    }
    return false;
}

// overload check function to return bool based on if the current token
// type matches any in a list of expected types
bool Parser::check(std::vector<TokenType> types) {
    TokenType type = lookahead_.getType();
    for (const TokenType t : types)
        if (type == t) {
            return true; // match found
        }
    return false; // no found matches
}

// code non-terminal
void Parser::code() {
    // check to see if the lookahead is one of the tokens
    // that can start a statement
    if (check({TokenType::tok_duck_fr, TokenType::tok_duck_fl,
                  TokenType::tok_duck_er, TokenType::tok_bit,
                  TokenType::tok_clone, TokenType::tok_dub,
                  TokenType::tok_pop, TokenType::tok_mul,
                  TokenType::tok_div, TokenType::tok_plus,
                  TokenType::tok_minus, TokenType::tok_string,
                  TokenType::tok_arrow, TokenType::tok_keyword,
                  TokenType::tok_id})) {
        statement();
        // call code recursively to handle more statements
        code();
    } else {
        // epsilon
    }

    if (check(TokenType::tok_eof)) {
        match(TokenType::tok_eof); // matching token to make sure EOF
        std::cout << "[PARSER] Success!" << std::endl;
        exit(0);
    } else {
        // error message if end of input not reached
        std::cout << "[PARSER] Something went wrong during parsing!" 
                  << std::endl;
        exit(0);
    }
}

// statement non-terminal
void Parser::statement() {
    if (check({TokenType::tok_duck_fr, TokenType::tok_duck_fl, 
               TokenType::tok_duck_er})) {
        duck(); // parsing duck() if the current token is a duck
    } else {
        // epsilon
    }
    quack(); // parsing a quack
    if (check({TokenType::tok_duck_fr, TokenType::tok_duck_fl, 
               TokenType::tok_duck_er})) {
        duck(); // parsing duck() if the current token is a duck
    } else {
        // epsilon
    }
}

// duck non-terminal
void Parser::duck() {
    // checking for different types of duck tokens and match accordingly
    if (check(TokenType::tok_duck_fr)) {
        match(TokenType::tok_duck_fr);
    }
    if (check(TokenType::tok_duck_fl)) {
        match(TokenType::tok_duck_fl);
    }
    if (check(TokenType::tok_duck_er)) {
        match(TokenType::tok_duck_er);
    }
}

// quack non-terminal
void Parser::quack() {
    // check for each token type that can start a quack
    // and then call match for the corresponding token.
    // Check for each token type that can start a quack
    if (check(TokenType::tok_keyword)) {
        if (lookahead_.getLexeme() == "hey") {
            match(TokenType::tok_keyword); // Consume the "hey" keyword
            // Ensure the next token is an 
            // identifier before checking its lexeme
            if (check(TokenType::tok_id) && 
                lookahead_.getLexeme() == "vous") {
                match(TokenType::tok_id); // consume the vous identifier
                if (check(TokenType::tok_dub)) {
                    // consume the "!" (DUB) token
                    match(TokenType::tok_dub);
                }
            } else {
                // print an error and exit
                match(TokenType::tok_keyword);
            }
        } else {
            // If not "hey", just consume the keyword
            match(TokenType::tok_keyword);
        }
    }
    // checks for other token types that could be part of a quack 
    // construct each if statement checking for specific token 
    // types and consume it

    // checking for token id
    if (check(TokenType::tok_id)) {
        match(TokenType::tok_id);
    }
    // checking for token "./"
    if (check(TokenType::tok_bit)) {
        match(TokenType::tok_bit);
    }
    // checking for token ",/"
    if (check(TokenType::tok_clone)) {
        match(TokenType::tok_clone);
    }
    // checking for token "!"
    if (check(TokenType::tok_dub)) {
        match(TokenType::tok_dub);
    }
    // checking for token "?"
    if (check(TokenType::tok_pop)) {
        match(TokenType::tok_pop);
    }
    // checking for token "*"
    if (check(TokenType::tok_mul)) {
        match(TokenType::tok_mul);
    }
    // checking for token "//"
    if (check(TokenType::tok_div)) {
        match(TokenType::tok_div);
    }
    // checking for token "+"
    if (check(TokenType::tok_plus)) {
        match(TokenType::tok_plus);
    }
    // checking for token "-"
    if (check(TokenType::tok_minus)) {
        match(TokenType::tok_minus);
    }
    // checking for token "%"
    if (check(TokenType::tok_string)) {
        match(TokenType::tok_string);
    }
    // checking for token "->"
    if (check(TokenType::tok_arrow)) {
        match(TokenType::tok_arrow);
    }
}

// function to consume the current token and advance the
// lookahead to the next token
void Parser::consume() {
    index_++; // moving to the next token
    if (index_ < tokens_.size()) {
        lookahead_ = tokens_[index_]; // Update the lookahead token
    }
}