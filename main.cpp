// libraries
#include <iostream> // For I/O operations
#include <fstream> // file operations
#include <cstdlib> // general purpose functions
#include <vector> // vectors

// custom header files for parsing, 
// scanning, and token functionalities
#include "scanner.hpp"
#include "parser.hpp"
#include "token.hpp"

// using namespace standard
using namespace std;

// function creats a duck token from its head and body tokens
Token createDuck(const Token& head, const Token& body) {
    switch (head.getType()) {
        // Right facing duck
        case TokenType::tok_rhead:
            return { TokenType::tok_duck_fr, "@<\n###" };
        // Equal facing duck
        case TokenType::tok_ehead:
            return { TokenType::tok_duck_er, "@=\n###" };
        // Left facing duck
        case TokenType::tok_lhead:
            return { TokenType::tok_duck_fl, ">@\n   ###" };
        default: break;
    }
    return { TokenType::tok_na, "n/a" }; // ret for annoying CG
}

// function to find a token in a vector of tokens
// based on the type
int find(const vector<Token>& tokens, Token& tok) {
    for (int i = 0; i < tokens.size(); ++i)
        if (tokens[i].getType() == tok.getType())
            return i;
    return -1; // ret -1 if not found
}

// main function: the entry point of the program
int main(int argc, char* argv[]) {
    // argc checks to ensure a filename is provided in 
    // cmd line argument
    if (argc < 2) {
        cout << "Invalid command-line arguments!" << endl;
        exit(0);
    }

    // opening the file for reading
    ifstream infile(argv[1]);
    if (!infile) {
        cout << "Could not open file '" << argv[1] << "'" << endl;
        exit(0);
    }

    // reading the entire content of the file into a buffer
    infile.seekg(0, std::ifstream::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ifstream::beg);
    char* buffer = new char[int(length) + 1];
    infile.read(buffer, int(length));
    if (!infile) {
        cout << "Could not read file '" << argv[1] << "'" << endl;
        exit(0);
    }
    infile.close();
    buffer[length] = '\0'; // terminate the buffer with null

    // creating a scanner that feeds off the input buffer.
    Scanner scanner(int(length), buffer);
    cout << "Scanning the file..." << endl;
    vector<Token> tokens;

    // scanning tokens until EOF has reached
    Token token;
    do {
        token = scanner.nextToken();
        tokens.push_back(token);
    } while (token.getType() != TokenType::tok_eof);

    // printing the scanned tokens
    cout << "Scanning done!\n\nList of tokens:" << endl;
    for (const auto& token : tokens) {
        cout << token.toString() << endl;
    }

    // rewriting the tokens from scanner to form ducks by combining
    // the head and body
    cout << "\nRewriting some tokens..." << endl;
    for (size_t i = 0; i < tokens.size(); ++i) {
        Token temp;
        temp.setLexeme("###");
        temp.setType(TokenType::tok_body);
        // for each head token, find the corresponding
        // body token and create a duck token and replace
        if (tokens[i].getType() == TokenType::tok_rhead || 
            tokens[i].getType() == TokenType::tok_ehead || 
            tokens[i].getType() == TokenType::tok_lhead ) {
                int j = find(tokens, temp);
                Token duck = createDuck(tokens[i], tokens[j]);
                tokens[i].setType(duck.getType());
                tokens[i].setLexeme(duck.getLexeme());
                tokens.erase(tokens.begin() + j);
        }
    }

    // print the rewritten tokens
    cout << "New list of tokens:" << endl;
    for (const auto& tokens : tokens) {
        cout << tokens.toString() << endl;
    }

    // parsing the rewritten tokens
    cout << "\nParsing file..." << endl;
    Parser parser(tokens);
    parser.code(); // starting the parsing process

    delete[] buffer; // clean up dynamic allocated buffer
    return 0;
}