#include "token.hpp"
#include "scanner.hpp"

Token::Token() : type_(TokenType::tok_na), lexeme_("") { }

// constructor initializing a token with a specific type and lexeme
Token::Token(TokenType type, std::string lexeme) : type_(type), lexeme_(lexeme) { }

// setter for the token type
void Token::setType(TokenType type) {
    type_ = type;
}

//setter for the token lexeme
void Token::setLexeme(std::string lexeme) {
    lexeme_ = lexeme;
}

// getter for the token type
TokenType Token::getType() const {
    return type_;
}

//getter for the token lexeme
std::string Token::getLexeme() const {
    return lexeme_;
}

// getting a string representation of the token
std::string Token::toString() const {
    // returning a formatted string containing the token's lexeme, type
    // and the string representation of the type
    return "<'" + lexeme_ + "'," + std::to_string(type_) 
    + "," + Scanner::tokenNames[type_] + ">";
}
