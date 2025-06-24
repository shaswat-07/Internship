#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>
using namespace std;


// Token types represent different components of the expression
enum class TokenType {
    NUMBER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END
};

// Token struct to store the token type and numeric value (if any)
struct Token {
    TokenType type;
    double value; 
};

// Lexer: Responsible for converting the input string into a stream of tokens
class Lexer {
    string input;      
    size_t pos;       
    char currentChar;  

public:
    // Constructor initializes input and sets the current character
    Lexer(const string& text) : input(text), pos(0) {
        currentChar = input[pos];
    }

    // Moves to the next character in the input
    void advance() {
        pos++;
        currentChar = (pos < input.size()) ? input[pos] : '\0';
    }

    // Skips whitespaces between tokens
    void skipWhitespace() {
        while (isspace(currentChar)) advance();
    }

    // Extracts a multi-digit (and possibly floating-point) number
    double number() {
        string numStr;
        while (isdigit(currentChar) || currentChar == '.') {
            numStr += currentChar;
            advance();
        }
        return stod(numStr);  
    }

    // Returns the next token from the input string
    Token getNextToken() {
        skipWhitespace();

        if (isdigit(currentChar) || currentChar == '.') {
            return Token{TokenType::NUMBER, number()};
        }

        if (currentChar == '+') { advance(); return Token{TokenType::PLUS, 0}; }
        if (currentChar == '-') { advance(); return Token{TokenType::MINUS, 0}; }
        if (currentChar == '*') { advance(); return Token{TokenType::MUL, 0}; }
        if (currentChar == '/') { advance(); return Token{TokenType::DIV, 0}; }
        if (currentChar == '(') { advance(); return Token{TokenType::LPAREN, 0}; }
        if (currentChar == ')') { advance(); return Token{TokenType::RPAREN, 0}; }

        if (currentChar == '\0') return Token{TokenType::END, 0};

        // If unknown character is encountered
        throw runtime_error(string("Unknown character: ") + currentChar);
    }
};

// Parser: Implements recursive descent parsing and evaluates the expression
class Parser {
    Lexer lexer;        
    Token currentToken; 

public: 
    Parser(const string& text) : lexer(text) {
        currentToken = lexer.getNextToken();
    }

    // Verifies that the current token matches the expected type and moves to next
    void eat(TokenType type) {
        if (currentToken.type == type)
            currentToken = lexer.getNextToken();
        else
            throw runtime_error("Syntax error: Unexpected token");
    }

    // Parses and returns a factor
    double factor() {
        if (currentToken.type == TokenType::NUMBER) {
            double val = currentToken.value;
            eat(TokenType::NUMBER);
            return val;
        } else if (currentToken.type == TokenType::LPAREN) {
            eat(TokenType::LPAREN);
            double result = expr();  
            if (currentToken.type != TokenType::RPAREN)
                throw runtime_error("Syntax error: Expected ')'");
            eat(TokenType::RPAREN);
            return result;
        } else if (currentToken.type == TokenType::MINUS) {
            eat(TokenType::MINUS);
            return -factor();
        }
        throw runtime_error("Syntax error: Unexpected token in factor");
    }

    // Parses and returns a term
    double term() {
        double result = factor();

        while (currentToken.type == TokenType::MUL || currentToken.type == TokenType::DIV) {
            Token token = currentToken;
            if (token.type == TokenType::MUL) {
                eat(TokenType::MUL);
                result *= factor();
            } else if (token.type == TokenType::DIV) {
                eat(TokenType::DIV);
                double denominator = factor();
                if (denominator == 0)
                    throw runtime_error("Math error: Division by zero");
                result /= denominator;
            }
        }

        return result;
    }

    // Parses and returns an expression
    double expr() {
        double result = term();

        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
            Token token = currentToken;
            if (token.type == TokenType::PLUS) {
                eat(TokenType::PLUS);
                result += term();
            } else if (token.type == TokenType::MINUS) {
                eat(TokenType::MINUS);
                result -= term();
            }
        }

        return result;
    }

    // Begins the parsing process and returns the final result
    double parse() {
        double result = expr();
        if (currentToken.type != TokenType::END)
            throw runtime_error("Syntax error: Unexpected input after expression");
        return result;
    }
};

// Entry point of the program
int main() {
    cout << "Enter an arithmetic expression:\n";
    string input;
    getline(cin, input);

    try {
        Parser parser(input);
        double result = parser.parse();
        cout << "Result: " << result << endl;
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
