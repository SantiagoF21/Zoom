#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <optional>
#include <string>

#include "node.hpp"
#include "token.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<Node::Instruction> parse_program();
private:
    const std::vector<Token> m_tokens;
    size_t m_index{};

    std::optional<Node::Term> parse_term();
    std::optional<Node::Expression> parse_expr();
    std::optional<Node::Instruction> parse_instr();

    std::optional<Token> peek(size_t index_offset = 0) const;
    Token consume();

    void expect(TokenID id, const std::string& err_msg);
};

#endif