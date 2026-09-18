#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include <iostream>
#include <memory>
#include <string>

namespace minic {

void printIndent(int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
}

void printExpression(const Expr* expr, int indent);

void printStatement(const Statement* statement, int indent) {

    if (const auto* declaration =
            dynamic_cast<const VariableDeclaration*>(statement)) {

        printIndent(indent);
        std::cout << "VariableDeclaration"
                  << " type=" << declaration->getType()
                  << " name=" << declaration->getName()
                  << '\n';

        if (declaration->getInitializer()) {
            printIndent(indent + 1);
            std::cout << "Initializer:\n";

            printExpression(
                declaration->getInitializer(),
                indent + 2
            );
        }

        return;
    }

    if (const auto* returnStatement =
            dynamic_cast<const ReturnStatement*>(statement)) {

        printIndent(indent);
        std::cout << "ReturnStatement\n";

        if (returnStatement->getExpression()) {
            printExpression(
                returnStatement->getExpression(),
                indent + 1
            );
        }

        return;
    }

    printIndent(indent);
    std::cout << "Unknown Statement\n";
}

void printExpression(const Expr* expr, int indent) {

    if (const auto* number =
            dynamic_cast<const NumberExpr*>(expr)) {

        printIndent(indent);
        std::cout << "NumberExpr value="
                  << number->getValue()
                  << '\n';

        return;
    }

    if (const auto* variable =
            dynamic_cast<const VariableExpr*>(expr)) {

        printIndent(indent);
        std::cout << "VariableExpr name="
                  << variable->getName()
                  << '\n';

        return;
    }

    if (const auto* binary =
            dynamic_cast<const BinaryExpr*>(expr)) {

        printIndent(indent);
        std::cout << "BinaryExpr operator="
                  << binary->getOperator()
                  << '\n';

        printIndent(indent + 1);
        std::cout << "Left:\n";

        printExpression(
            binary->getLeft(),
            indent + 2
        );

        printIndent(indent + 1);
        std::cout << "Right:\n";

        printExpression(
            binary->getRight(),
            indent + 2
        );

        return;
    }

    printIndent(indent);
    std::cout << "Unknown Expression\n";
}

void printAST(const Program* program) {

    std::cout << "\n=== Abstract Syntax Tree ===\n\n";

    for (const auto& function : program->getFunctions()) {

        std::cout << "FunctionDeclaration"
                  << " returnType=" << function->getReturnType()
                  << " name=" << function->getName()
                  << '\n';

        const auto* body = function->getBody();

        printIndent(1);
        std::cout << "Body:\n";

        for (const auto& statement : body->getStatements()) {
            printStatement(statement.get(), 2);
        }

        std::cout << '\n';
    }
}

} // namespace minic

int main() {

    const std::string source = R"(
        int main() {
            int x = 10 + 20 * 2;
            return x;
        }
    )";

    try {

        minic::Lexer lexer(source);

        minic::Parser parser(lexer);

        auto program = parser.parseProgram();

        minic::printAST(program.get());

    }
    catch (const std::exception& error) {

        std::cerr << "Compilation failed:\n"
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}