#include "lexer/Lexer.h"

#include <iostream>

int main() {

    const std::string source = R"(
        int main() {
            int x = 10 + 20;
            return x;
        }
    )";

    minic::Lexer lexer(source);

    std::cout << "=== Mini Compiler Lexer ===\n\n";

    while (true) {

        minic::Token token = lexer.nextToken();

        std::cout
            << tokenTypeToString(token.type)
            << "\t"
            << "\"" << token.lexeme << "\""
            << "\t"
            << "line: " << token.location.line
            << ", column: " << token.location.column
            << '\n';

        if (token.type == minic::TokenType::EndOfFile) {
            break;
        }
    }

    return 0;
}
