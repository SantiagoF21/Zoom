#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>
#include <string_view>

enum class TokenID {
    _ident,
    _assign,
    _int,
    _plus,
    _minus,
    _mul,
    _div,
    _mod,
    _semi,
    _exit
};

struct Token {
    TokenID id;
    std::optional<std::string_view> value {};
};

namespace Char {
    constexpr bool is_semi(char character) {return character == ';';}
    constexpr bool is_alpha(char character) {return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');}
    constexpr bool is_digit(char character) {return character >= '0' && character <= '9';}
    constexpr bool is_white_space(char character) {return character == ' ' || character == '\t' || character == '\n' || character == '\r' || character == '\v' || character == '\f';}
    constexpr bool is_equal(char character) {return character == '=';}
    constexpr bool is_underscore(char character) {return character == '_';}
    constexpr bool is_plus(char character) {return character == '+';}
    constexpr bool is_minus(char character) {return character == '-';}
    constexpr bool is_mul(char character) {return character == '*';}
    constexpr bool is_div(char character) {return character == '/';}
    constexpr bool is_mod(char character) {return character == '%';}
}

#endif