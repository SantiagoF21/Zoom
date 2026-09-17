#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>
#include <vector>

#include "token.hpp"
#include "node.hpp"

namespace Debug {
    std::string token_to_string(const Token& token);
    void print_tokens(const std::vector<Token>& tokens);

    std::string term_to_string(const Node::Term& term);
    std::string expr_to_string(const Node::Expression& expr);
    std::string instr_to_string(const Node::Instruction& instr);
    std::string program_to_string(const std::vector<Node::Instruction>& program);
}

#endif