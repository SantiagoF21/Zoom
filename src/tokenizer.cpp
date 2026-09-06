#include "tokenizer.hpp"

#include <iostream>
#include <sstream>

Tokenizer::Tokenizer(const std::string& src_code) : m_src_code(std::move(src_code)) {}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    std::string buffer;

    while(auto character = peek()) {
        if (CharInfo::is_alpha(*character)) {
            buffer.push_back(consume());
            auto next_alpha = peek();
            while (next_alpha && CharInfo::is_alpha(*next_alpha)) {
                buffer.push_back(consume());
                next_alpha = peek();
            }
            if (buffer == "exit") {
                tokens.push_back({.id = TokenID::_exit});
                buffer.clear();
            } else {
                std::cerr << "Lexing Error: The string " + buffer + " could not be formatted into a token.";
                exit(1);
            }
        } else if (CharInfo::is_digit(*character)) {
            buffer.push_back(consume());
            auto next_digit = peek();
            while (next_digit && CharInfo::is_digit(*next_digit)) {
                buffer.push_back(consume());
                next_digit = peek();
            }
            tokens.push_back({.id = TokenID::_int_literal, .value = buffer});
            buffer.clear();
        } else if (CharInfo::is_semi(*character)) {
            consume();
            tokens.push_back({ .id = TokenID::_semi});
        } else if (CharInfo::is_white_space(*character))  {
            consume();
            continue;
        } else {
            std::cerr << "Lexing Error: Unexpected character '" << *character << ".'";
            exit(1);
        }
    }
    m_index = 0;
    return tokens;
}

/* Remove in the future */

std::string Tokenizer::tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << ".global _main\n.align 2\n\n_main:\n";
    for (size_t i {}; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.id == TokenID::_exit) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).id == TokenID::_int_literal) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).id == TokenID::_semi) {
                    output << "\tmov x0, #" + tokens.at(i + 1).value.value();
                    output << "\n\tret";
                }
            }
        }
    }
    return output.str();
}

std::optional<char> Tokenizer::peek(size_t index_offset) const {
    if (m_index + index_offset >= m_src_code.length()) {
        return {};
    } else {
        return m_src_code[m_index + index_offset];
    }
}

char Tokenizer::consume() {
    return m_src_code.at(m_index++);
}