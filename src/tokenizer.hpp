#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <optional>
#include <string>
#include <vector>

enum class TokenID {
    _exit,
    _int_literal,
    _semi
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
}

class Tokenizer {
public:
    explicit Tokenizer(const std::string& src_code);
    
    std::vector<Token> tokenize();

    /* Remove in the future */
    std::string tokens_to_asm(const std::vector<Token>& tokens);
private:
    const std::string m_src_code;
    size_t m_index{};

    std::optional<char> peek(size_t index_offset = 0) const;
    char consume();
};

#endif