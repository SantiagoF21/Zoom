#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <optional>

#include "token.hpp"

class Lexer {
public:
    explicit Lexer(std::string src_code);
    
    std::vector<Token> tokenize();
private:
    const std::string m_src_code;
    size_t m_index{};

    std::optional<char> peek(size_t index_offset = 0) const;
    char consume();
};

#endif