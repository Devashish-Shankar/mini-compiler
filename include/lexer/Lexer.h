#pragma once

#include "lexer/Token.h"

#include <string>
#include <unordered_map>

namespace minic {

class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token nextToken();

private:
    char currentChar() const;
    char peekChar() const;

    void advance();
    void skipWhitespace();

    Token scanIdentifierOrKeyword();
    Token scanNumber();

    Token makeToken(
        TokenType type,
        std::size_t startPosition,
        SourceLocation startLocation
    ) const;

    bool isAtEnd() const;

    const std::string& source_;

    std::size_t position_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    static const std::unordered_map<std::string, TokenType> keywords_;
};

} // namespace minic
