#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>
#include <string>
#include <cctype>

enum class TokenID {
    _ident,
    _assign,
    _int,
    _plus,
    _semi,
    _exit
};

struct Token {
    TokenID id;
    std::optional<std::string> value {};
};

namespace CharInfo {
    constexpr bool is_semi(char character) {return character == ';';}
    constexpr bool is_alpha(char character) {return std::isalpha(static_cast<unsigned char>(character)) != 0;}
    constexpr bool is_digit(char character) {return std::isdigit(static_cast<unsigned char>(character)) != 0;}
    constexpr bool is_white_space(char character) { return std::isspace(static_cast<unsigned char>(character)) != 0;}
    constexpr bool is_equal(char character) {return character == '=';}
    constexpr bool is_underscore(char character) {return character == '_';}
    constexpr bool is_plus(char character) {return character == '+';}
}

#endif