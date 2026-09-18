#pragma once

#include "lexer/Lexer.h"
#include "parser/AST.h"

#include <memory>
#include <stdexcept>

namespace minic {

class Parser {
public:
    explicit Parser(Lexer& lexer);

    std::unique_ptr<Program> parseProgram();

private:
    // =============================
    // Top-level parsing
    // =============================

    std::unique_ptr<FunctionDeclaration> parseFunction();

    // =============================
    // Statement parsing
    // =============================

    std::unique_ptr<BlockStatement> parseBlock();

    std::unique_ptr<Statement> parseStatement();

    std::unique_ptr<Statement> parseVariableDeclaration();

    std::unique_ptr<Statement> parseReturnStatement();

    // =============================
    // Expression parsing
    // =============================

    std::unique_ptr<Expr> parseExpression();

    std::unique_ptr<Expr> parseEquality();

    std::unique_ptr<Expr> parseComparison();

    std::unique_ptr<Expr> parseTerm();

    std::unique_ptr<Expr> parseFactor();

    std::unique_ptr<Expr> parseUnary();

    std::unique_ptr<Expr> parsePrimary();

    // =============================
    // Token utilities
    // =============================

    void advance();

    bool check(TokenType type) const;

    Token consume(TokenType type, const std::string& message);

    [[noreturn]] void error(const std::string& message) const;

    Lexer& lexer_;
    Token currentToken_;
};

} // namespace minic