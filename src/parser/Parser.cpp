#include "parser/Parser.h"

#include <sstream>
#include <utility>

namespace minic {

// =============================
// Constructor
// =============================

Parser::Parser(Lexer& lexer)
    : lexer_(lexer),
      currentToken_(lexer_.nextToken()) {}

// =============================
// Token utilities
// =============================

void Parser::advance() {
    currentToken_ = lexer_.nextToken();
}

bool Parser::check(TokenType type) const {
    return currentToken_.type == type;
}

Token Parser::consume(
    TokenType type,
    const std::string& message
) {
    if (!check(type)) {
        error(message);
    }

    Token token = currentToken_;
    advance();

    return token;
}

[[noreturn]]
void Parser::error(const std::string& message) const {
    std::ostringstream oss;

    oss << "Parser error at line "
        << currentToken_.location.line
        << ", column "
        << currentToken_.location.column
        << ": "
        << message
        << " (found '"
        << currentToken_.lexeme
        << "')";

    throw std::runtime_error(oss.str());
}

// =============================
// Program
// =============================

std::unique_ptr<Program> Parser::parseProgram() {

    auto program = std::make_unique<Program>();

    while (!check(TokenType::EndOfFile)) {
        program->addFunction(parseFunction());
    }

    return program;
}

// =============================
// Function
// =============================

std::unique_ptr<FunctionDeclaration>
Parser::parseFunction() {

    std::string returnType;

    if (check(TokenType::Int)) {
        returnType = "int";
    }
    else if (check(TokenType::Float)) {
        returnType = "float";
    }
    else if (check(TokenType::Bool)) {
        returnType = "bool";
    }
    else {
        error("Expected function return type");
    }

    advance();

    Token name = consume(
        TokenType::Identifier,
        "Expected function name"
    );

    consume(
        TokenType::LeftParen,
        "Expected '(' after function name"
    );

    consume(
        TokenType::RightParen,
        "Expected ')' after function parameters"
    );

    auto body = parseBlock();

    return std::make_unique<FunctionDeclaration>(
        returnType,
        name.lexeme,
        std::move(body)
    );
}

// =============================
// Block
// =============================

std::unique_ptr<BlockStatement>
Parser::parseBlock() {

    consume(
        TokenType::LeftBrace,
        "Expected '{' to start block"
    );

    auto block = std::make_unique<BlockStatement>();

    while (!check(TokenType::RightBrace) &&
           !check(TokenType::EndOfFile)) {

        block->addStatement(parseStatement());
    }

    consume(
        TokenType::RightBrace,
        "Expected '}' after block"
    );

    return block;
}

// =============================
// Statement
// =============================

std::unique_ptr<Statement>
Parser::parseStatement() {

    if (check(TokenType::Int) ||
        check(TokenType::Float) ||
        check(TokenType::Bool)) {

        return parseVariableDeclaration();
    }

    if (check(TokenType::Return)) {
        return parseReturnStatement();
    }

    error("Unexpected token in statement");
}

// =============================
// Variable Declaration
// =============================

std::unique_ptr<Statement>
Parser::parseVariableDeclaration() {

    std::string type;

    if (check(TokenType::Int)) {
        type = "int";
    }
    else if (check(TokenType::Float)) {
        type = "float";
    }
    else if (check(TokenType::Bool)) {
        type = "bool";
    }
    else {
        error("Expected variable type");
    }

    advance();

    Token name = consume(
        TokenType::Identifier,
        "Expected variable name"
    );

    std::unique_ptr<Expr> initializer = nullptr;

    if (check(TokenType::Assign)) {
        advance();

        initializer = parseExpression();
    }

    consume(
        TokenType::Semicolon,
        "Expected ';' after variable declaration"
    );

    return std::make_unique<VariableDeclaration>(
        type,
        name.lexeme,
        std::move(initializer)
    );
}

// =============================
// Return Statement
// =============================

std::unique_ptr<Statement>
Parser::parseReturnStatement() {

    consume(
        TokenType::Return,
        "Expected 'return'"
    );

    std::unique_ptr<Expr> expression = nullptr;

    if (!check(TokenType::Semicolon)) {
        expression = parseExpression();
    }

    consume(
        TokenType::Semicolon,
        "Expected ';' after return statement"
    );

    return std::make_unique<ReturnStatement>(
        std::move(expression)
    );
}

// =============================
// Expressions
// =============================

std::unique_ptr<Expr>
Parser::parseExpression() {
    return parseEquality();
}

// =============================
// Equality
// =============================

std::unique_ptr<Expr>
Parser::parseEquality() {

    auto expression = parseComparison();

    while (check(TokenType::Equal) ||
           check(TokenType::NotEqual)) {

        std::string op = currentToken_.lexeme;
        advance();

        auto right = parseComparison();

        expression = std::make_unique<BinaryExpr>(
            op,
            std::move(expression),
            std::move(right)
        );
    }

    return expression;
}

// =============================
// Comparison
// =============================

std::unique_ptr<Expr>
Parser::parseComparison() {

    auto expression = parseTerm();

    while (check(TokenType::Less) ||
           check(TokenType::LessEqual) ||
           check(TokenType::Greater) ||
           check(TokenType::GreaterEqual)) {

        std::string op = currentToken_.lexeme;
        advance();

        auto right = parseTerm();

        expression = std::make_unique<BinaryExpr>(
            op,
            std::move(expression),
            std::move(right)
        );
    }

    return expression;
}

// =============================
// Term
// =============================

std::unique_ptr<Expr>
Parser::parseTerm() {

    auto expression = parseFactor();

    while (check(TokenType::Plus) ||
           check(TokenType::Minus)) {

        std::string op = currentToken_.lexeme;
        advance();

        auto right = parseFactor();

        expression = std::make_unique<BinaryExpr>(
            op,
            std::move(expression),
            std::move(right)
        );
    }

    return expression;
}

// =============================
// Factor
// =============================

std::unique_ptr<Expr>
Parser::parseFactor() {

    auto expression = parseUnary();

    while (check(TokenType::Star) ||
           check(TokenType::Slash)) {

        std::string op = currentToken_.lexeme;
        advance();

        auto right = parseUnary();

        expression = std::make_unique<BinaryExpr>(
            op,
            std::move(expression),
            std::move(right)
        );
    }

    return expression;
}

// =============================
// Unary
// =============================

std::unique_ptr<Expr>
Parser::parseUnary() {

    if (check(TokenType::Minus) ||
        check(TokenType::Plus)) {

        std::string op = currentToken_.lexeme;
        advance();

        auto right = parseUnary();

        auto zero = std::make_unique<NumberExpr>("0");

        return std::make_unique<BinaryExpr>(
            op,
            std::move(zero),
            std::move(right)
        );
    }

    return parsePrimary();
}

// =============================
// Primary
// =============================

std::unique_ptr<Expr>
Parser::parsePrimary() {

    if (check(TokenType::IntegerLiteral) ||
        check(TokenType::FloatLiteral)) {

        std::string value = currentToken_.lexeme;
        advance();

        return std::make_unique<NumberExpr>(value);
    }

    if (check(TokenType::Identifier)) {

        std::string name = currentToken_.lexeme;
        advance();

        return std::make_unique<VariableExpr>(name);
    }

    if (check(TokenType::LeftParen)) {

        advance();

        auto expression = parseExpression();

        consume(
            TokenType::RightParen,
            "Expected ')' after expression"
        );

        return expression;
    }

    error("Expected expression");
}

} // namespace minic