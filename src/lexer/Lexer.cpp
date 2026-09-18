#include "lexer/Lexer.h"

#include <cctype>
#include <stdexcept>
#include <utility>

namespace minic {

const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"int", TokenType::Int},
    {"float", TokenType::Float},
    {"bool", TokenType::Bool},
    {"return", TokenType::Return},
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While}
};

Lexer::Lexer(const std::string& source)
    : source_(source) {}

char Lexer::currentChar() const {
    if (isAtEnd()) {
        return '\0';
    }

    return source_[position_];
}

char Lexer::peekChar() const {
    if (position_ + 1 >= source_.size()) {
        return '\0';
    }

    return source_[position_ + 1];
}

bool Lexer::isAtEnd() const {
    return position_ >= source_.size();
}

void Lexer::advance() {
    if (isAtEnd()) {
        return;
    }

    if (source_[position_] == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }

    ++position_;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() &&
           std::isspace(static_cast<unsigned char>(currentChar()))) {
        advance();
    }
}

Token Lexer::makeToken(
    TokenType type,
    std::size_t startPosition,
    SourceLocation startLocation
) const {
    return Token(
        type,
        source_.substr(
            startPosition,
            position_ - startPosition
        ),
        startLocation
    );
}

Token Lexer::scanIdentifierOrKeyword() {
    const std::size_t startPosition = position_;

    const SourceLocation startLocation{
        line_,
        column_
    };

    while (!isAtEnd() &&
           (std::isalnum(
                static_cast<unsigned char>(currentChar())
            ) ||
            currentChar() == '_')) {
        advance();
    }

    const std::string lexeme =
        source_.substr(
            startPosition,
            position_ - startPosition
        );

    auto keyword = keywords_.find(lexeme);

    if (keyword != keywords_.end()) {
        return Token(
            keyword->second,
            lexeme,
            startLocation
        );
    }

    return Token(
        TokenType::Identifier,
        lexeme,
        startLocation
    );
}

Token Lexer::scanNumber() {
    const std::size_t startPosition = position_;

    const SourceLocation startLocation{
        line_,
        column_
    };

    bool isFloat = false;

    while (!isAtEnd() &&
           std::isdigit(
               static_cast<unsigned char>(currentChar()))) {
        advance();
    }

    if (currentChar() == '.' &&
        std::isdigit(
            static_cast<unsigned char>(peekChar()))) {

        isFloat = true;
        advance();

        while (!isAtEnd() &&
               std::isdigit(
                   static_cast<unsigned char>(currentChar()))) {
            advance();
        }
    }

    return makeToken(
        isFloat
            ? TokenType::FloatLiteral
            : TokenType::IntegerLiteral,
        startPosition,
        startLocation
    );
}

Token Lexer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return Token(
            TokenType::EndOfFile,
            "",
            {line_, column_}
        );
    }

    const std::size_t startPosition = position_;

    const SourceLocation startLocation{
        line_,
        column_
    };

    const char c = currentChar();

    // Identifier / keyword
    if (std::isalpha(
            static_cast<unsigned char>(c)) ||
        c == '_') {

        return scanIdentifierOrKeyword();
    }

    // Number
    if (std::isdigit(
            static_cast<unsigned char>(c))) {

        return scanNumber();
    }

    advance();

    switch (c) {

        case '+':
            return Token(
                TokenType::Plus,
                "+",
                startLocation
            );

        case '-':
            return Token(
                TokenType::Minus,
                "-",
                startLocation
            );

        case '*':
            return Token(
                TokenType::Star,
                "*",
                startLocation
            );

        case '/':
            return Token(
                TokenType::Slash,
                "/",
                startLocation
            );

        case '=':
            if (currentChar() == '=') {
                advance();

                return makeToken(
                    TokenType::Equal,
                    startPosition,
                    startLocation
                );
            }

            return Token(
                TokenType::Assign,
                "=",
                startLocation
            );

        case '!':
            if (currentChar() == '=') {
                advance();

                return makeToken(
                    TokenType::NotEqual,
                    startPosition,
                    startLocation
                );
            }

            return Token(
                TokenType::Invalid,
                "!",
                startLocation
            );

        case '<':
            if (currentChar() == '=') {
                advance();

                return makeToken(
                    TokenType::LessEqual,
                    startPosition,
                    startLocation
                );
            }

            return Token(
                TokenType::Less,
                "<",
                startLocation
            );

        case '>':
            if (currentChar() == '=') {
                advance();

                return makeToken(
                    TokenType::GreaterEqual,
                    startPosition,
                    startLocation
                );
            }

            return Token(
                TokenType::Greater,
                ">",
                startLocation
            );

        case '(':
            return Token(
                TokenType::LeftParen,
                "(",
                startLocation
            );

        case ')':
            return Token(
                TokenType::RightParen,
                ")",
                startLocation
            );

        case '{':
            return Token(
                TokenType::LeftBrace,
                "{",
                startLocation
            );

        case '}':
            return Token(
                TokenType::RightBrace,
                "}",
                startLocation
            );

        case ';':
            return Token(
                TokenType::Semicolon,
                ";",
                startLocation
            );

        case ',':
            return Token(
                TokenType::Comma,
                ",",
                startLocation
            );

        default:
            return Token(
                TokenType::Invalid,
                std::string(1, c),
                startLocation
            );
    }
}

} // namespace minic
