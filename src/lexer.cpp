#include "lexer.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

namespace {
    const std::unordered_map<std::string, TokenID> keywords = {
        {"exit", TokenID::_exit},
    };
};

Lexer::Lexer(std::string src_code) : m_src_code(std::move(src_code)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    std::string buffer;
    
    while(auto starting_char = peek()) {
        if (CharInfo::is_alpha(*starting_char) || CharInfo::is_underscore(*starting_char)) {
            buffer.push_back(consume());
            auto next_char = peek();
            while (next_char && (CharInfo::is_alpha(*next_char) || CharInfo::is_underscore(*next_char) || CharInfo::is_digit(*next_char))) {
                buffer.push_back(consume());
                next_char = peek();
            }
            if (auto iterator = keywords.find(buffer); iterator != keywords.end()) {
                tokens.push_back({.id = iterator->second});
            } else {
                tokens.push_back({.id = TokenID::_ident, .value = buffer});
            }
            buffer.clear();
        } else if (CharInfo::is_digit(*starting_char)) {
            buffer.push_back(consume());
            auto next_digit = peek();
            while (next_digit && CharInfo::is_digit(*next_digit)) {
                buffer.push_back(consume());
                next_digit = peek();
            }
            tokens.push_back({.id = TokenID::_int, .value = buffer});
            buffer.clear();
        } else {
            if (CharInfo::is_equal(*starting_char)) {
                tokens.push_back({ .id = TokenID::_assign});
            } else if (CharInfo::is_plus(*starting_char)) {
                tokens.push_back({ .id = TokenID::_plus});
            } else if (CharInfo::is_semi(*starting_char)) {
                tokens.push_back({ .id = TokenID::_semi});
            } else if (CharInfo::is_white_space(*starting_char))  {
                consume();
                continue;
            } else {
                std::cerr << "Lexing Error: Unexpected character '" << *starting_char << "'";
                exit(1);
            }
            consume();
        }
    }
    m_index = 0;
    return tokens;
}

std::optional<char> Lexer::peek(size_t index_offset) const {
    if (m_index + index_offset >= m_src_code.length()) {
        return {};
    } else {
        return m_src_code[m_index + index_offset];
    }
}

char Lexer::consume() {
    return m_src_code.at(m_index++);
}