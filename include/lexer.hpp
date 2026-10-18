#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <optional>

#include "token.hpp"

class Lexer {
public:
    explicit Lexer(std::string src_code);
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    std::vector<Token> tokenize();
private:
    enum class State {
        Start,
        Identifier,
        Number,
        Finished
    };

    const std::string m_src_code;
    size_t m_index{};

    State handle_start(std::vector<Token>& tokens);
    State handle_identifier(std::vector<Token>& tokens, size_t starting_index);
    State handle_number(std::vector<Token>& tokens, size_t starting_index);

    std::optional<char> peek(size_t index_offset = 0) const;
    char consume();
};

#endif