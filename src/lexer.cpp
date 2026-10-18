#include "lexer.hpp"

#include <utility>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <string_view>

Lexer::Lexer(std::string src_code) : m_src_code(std::move(src_code)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    State current_state = State::Start;
    size_t starting_index = 0;

    while (current_state != State::Finished) {
        switch (current_state) {
            case State::Start:
                starting_index = m_index;
                current_state = handle_start(tokens);
                break;
            case State::Identifier:
                current_state = handle_identifier(tokens, starting_index);
                break;
            case State::Number:
                current_state = handle_number(tokens, starting_index);
                break;
            default:
                std::cerr << "Lexing Error: Unknown state encountered\n";
                exit(1);
                break;
        }
    }
    return tokens;
}

Lexer::State Lexer::handle_start(std::vector<Token>& tokens) {
    auto current_char = peek();
    if (!current_char) {
        return State::Finished;
    } else if (Char::is_white_space(*current_char)) {
        consume();
        return State::Start;
    } else if (Char::is_alpha(*current_char) || Char::is_underscore(*current_char)) {
        consume();
        return State::Identifier;
    } else if (Char::is_digit(*current_char)) {
        consume();
        return State::Number;
    } else {
        if (Char::is_equal(*current_char)) tokens.push_back({.id = TokenID::_assign});
        else if (Char::is_plus(*current_char)) tokens.push_back({.id = TokenID::_plus});
        else if (Char::is_minus(*current_char)) tokens.push_back({.id = TokenID::_minus});
        else if (Char::is_mul(*current_char)) tokens.push_back({.id = TokenID::_mul});
        else if (Char::is_div(*current_char)) tokens.push_back({.id = TokenID::_div});
        else if (Char::is_mod(*current_char)) tokens.push_back({.id = TokenID::_mod});
        else if (Char::is_semi(*current_char)) tokens.push_back({.id = TokenID::_semi});
        else {
            std::cerr << "Lexing Error: Unexpected character '" << *current_char << "'\n";
            exit(1);
        }
        consume();
        return State::Start;
    }
}

Lexer::State Lexer::handle_identifier(std::vector<Token>& tokens, size_t starting_index) {
    static const std::unordered_map<std::string_view, TokenID> keywords = {
        {"exit", TokenID::_exit},
    };

    auto current_char = peek();
    if (current_char && (Char::is_alpha(*current_char) || Char::is_underscore(*current_char) || Char::is_digit(*current_char))) {
        consume();
        return State::Identifier;
    } else {
        std::string_view view(m_src_code.data() + starting_index, m_index - starting_index);
        if (auto entry = keywords.find(view); entry != keywords.end()) {
            tokens.push_back({.id = entry->second});
        } else {
            tokens.push_back({.id = TokenID::_ident, .value = view});
        }
        return State::Start;
    }
}

Lexer::State Lexer::handle_number(std::vector<Token>& tokens, size_t starting_index) {
    auto current_char = peek();
    if (current_char && Char::is_digit(*current_char)) {
        consume();
        return State::Number;
    } else {
        tokens.push_back({.id = TokenID::_int, .value = std::string_view(m_src_code.data() + starting_index, m_index - starting_index)});
        return State::Start;
    }
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
