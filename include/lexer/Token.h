#pragma once

#include <cstddef>
#include <string>
#include <utility>

namespace minic {

enum class TokenType {
    // Keywords
    Int,
    Float,
    Bool,
    Return,
    If,
    Else,
    While,

    // Identifiers and literals
    Identifier,
    IntegerLiteral,
    FloatLiteral,

    // Operators
    Plus,
    Minus,
    Star,
    Slash,
    Assign,

    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    // Delimiters
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Semicolon,
    Comma,

    // Special
    EndOfFile,
    Invalid
};

struct SourceLocation {
    std::size_t line;
    std::size_t column;
};

struct Token {
    TokenType type;
    std::string lexeme;
    SourceLocation location;

    Token(
        TokenType type,
        std::string lexeme,
        SourceLocation location
    )
        : type(type),
          lexeme(std::move(lexeme)),
          location(location) {}
};

inline const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Int:             return "INT";
        case TokenType::Float:           return "FLOAT";
        case TokenType::Bool:            return "BOOL";
        case TokenType::Return:          return "RETURN";
        case TokenType::If:              return "IF";
        case TokenType::Else:            return "ELSE";
        case TokenType::While:           return "WHILE";

        case TokenType::Identifier:      return "IDENTIFIER";
        case TokenType::IntegerLiteral:  return "INTEGER";
        case TokenType::FloatLiteral:    return "FLOAT_LITERAL";

        case TokenType::Plus:            return "PLUS";
        case TokenType::Minus:           return "MINUS";
        case TokenType::Star:            return "STAR";
        case TokenType::Slash:           return "SLASH";
        case TokenType::Assign:          return "ASSIGN";

        case TokenType::Equal:            return "EQUAL";
        case TokenType::NotEqual:         return "NOT_EQUAL";
        case TokenType::Less:             return "LESS";
        case TokenType::LessEqual:        return "LESS_EQUAL";
        case TokenType::Greater:          return "GREATER";
        case TokenType::GreaterEqual:     return "GREATER_EQUAL";

        case TokenType::LeftParen:        return "LEFT_PAREN";
        case TokenType::RightParen:       return "RIGHT_PAREN";
        case TokenType::LeftBrace:        return "LEFT_BRACE";
        case TokenType::RightBrace:       return "RIGHT_BRACE";
        case TokenType::Semicolon:        return "SEMICOLON";
        case TokenType::Comma:            return "COMMA";

        case TokenType::EndOfFile:        return "EOF";
        case TokenType::Invalid:          return "INVALID";
    }

    return "UNKNOWN";
}

} // namespace minic
