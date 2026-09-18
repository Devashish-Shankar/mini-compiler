#include "lexer/Lexer.h"
#include "parser/AST.h"
#include "parser/Parser.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace minic;

// ========================================
// Test 1: Basic function
// ========================================

void testBasicFunction() {

    const std::string source = R"(
        int main() {
            return 10;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parseProgram();

    assert(program != nullptr);
    assert(program->getFunctions().size() == 1);

    const auto* function = program->getFunctions()[0].get();

    assert(function->getName() == "main");
    assert(function->getReturnType() == "int");

    std::cout << "[PASS] Basic function\n";
}

// ========================================
// Test 2: Variable declaration
// ========================================

void testVariableDeclaration() {

    const std::string source = R"(
        int main() {
            int x = 42;
            return x;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parseProgram();

    const auto* body =
        program->getFunctions()[0]->getBody();

    assert(body->getStatements().size() == 2);

    const auto* declaration =
        dynamic_cast<const VariableDeclaration*>(
            body->getStatements()[0].get()
        );

    assert(declaration != nullptr);
    assert(declaration->getType() == "int");
    assert(declaration->getName() == "x");

    const auto* initializer =
        dynamic_cast<const NumberExpr*>(
            declaration->getInitializer()
        );

    assert(initializer != nullptr);
    assert(initializer->getValue() == "42");

    std::cout << "[PASS] Variable declaration\n";
}

// ========================================
// Test 3: Operator precedence
// ========================================

void testOperatorPrecedence() {

    const std::string source = R"(
        int main() {
            int x = 10 + 20 * 2;
            return x;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parseProgram();

    const auto* body =
        program->getFunctions()[0]->getBody();

    const auto* declaration =
        dynamic_cast<const VariableDeclaration*>(
            body->getStatements()[0].get()
        );

    assert(declaration != nullptr);

    const auto* plus =
        dynamic_cast<const BinaryExpr*>(
            declaration->getInitializer()
        );

    assert(plus != nullptr);
    assert(plus->getOperator() == "+");

    const auto* left =
        dynamic_cast<const NumberExpr*>(
            plus->getLeft()
        );

    assert(left != nullptr);
    assert(left->getValue() == "10");

    const auto* multiply =
        dynamic_cast<const BinaryExpr*>(
            plus->getRight()
        );

    assert(multiply != nullptr);
    assert(multiply->getOperator() == "*");

    const auto* rightLeft =
        dynamic_cast<const NumberExpr*>(
            multiply->getLeft()
        );

    const auto* rightRight =
        dynamic_cast<const NumberExpr*>(
            multiply->getRight()
        );

    assert(rightLeft != nullptr);
    assert(rightRight != nullptr);

    assert(rightLeft->getValue() == "20");
    assert(rightRight->getValue() == "2");

    std::cout << "[PASS] Operator precedence\n";
}

// ========================================
// Test 4: Parentheses
// ========================================

void testParentheses() {

    const std::string source = R"(
        int main() {
            int x = (10 + 20) * 2;
            return x;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parseProgram();

    const auto* body =
        program->getFunctions()[0]->getBody();

    const auto* declaration =
        dynamic_cast<const VariableDeclaration*>(
            body->getStatements()[0].get()
        );

    assert(declaration != nullptr);

    const auto* multiply =
        dynamic_cast<const BinaryExpr*>(
            declaration->getInitializer()
        );

    assert(multiply != nullptr);
    assert(multiply->getOperator() == "*");

    const auto* addition =
        dynamic_cast<const BinaryExpr*>(
            multiply->getLeft()
        );

    assert(addition != nullptr);
    assert(addition->getOperator() == "+");

    std::cout << "[PASS] Parentheses\n";
}

// ========================================
// Test 5: Return statement
// ========================================

void testReturnStatement() {

    const std::string source = R"(
        int main() {
            return 25;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parseProgram();

    const auto* body =
        program->getFunctions()[0]->getBody();

    const auto* returnStatement =
        dynamic_cast<const ReturnStatement*>(
            body->getStatements()[0].get()
        );

    assert(returnStatement != nullptr);

    const auto* expression =
        dynamic_cast<const NumberExpr*>(
            returnStatement->getExpression()
        );

    assert(expression != nullptr);
    assert(expression->getValue() == "25");

    std::cout << "[PASS] Return statement\n";
}

// ========================================
// Test 6: Invalid syntax
// ========================================

void testInvalidSyntax() {

    const std::string source = R"(
        int main() {
            int x = 10
            return x;
        }
    )";

    bool errorCaught = false;

    try {

        Lexer lexer(source);
        Parser parser(lexer);

        parser.parseProgram();

    }
    catch (const std::runtime_error&) {

        errorCaught = true;
    }

    assert(errorCaught);

    std::cout << "[PASS] Invalid syntax detection\n";
}

// ========================================
// Test Runner
// ========================================

int main() {

    try {

        testBasicFunction();
        testVariableDeclaration();
        testOperatorPrecedence();
        testParentheses();
        testReturnStatement();
        testInvalidSyntax();

        std::cout << "\nAll parser tests passed!\n";

    }
    catch (const std::exception& error) {

        std::cerr << "\nTEST FAILURE: "
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}