#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include <unordered_map>
#include <utility>
#include <string>

#include "node.hpp"

class Generator {
public:
    explicit Generator(std::vector<Node::Instruction> program);

    std::string generate_program();
private:
    const std::vector<Node::Instruction> m_program;

    std::unordered_map<std::string, size_t> m_vars;

    size_t m_stack_size = 0;

    std::string generate_term(const Node::Term& term);
    std::string generate_expression(const Node::Expression& expr);
    std::string generate_instruction(const Node::Instruction& instr);

    std::pair<std::string, size_t> push(const std::string& reg);
    size_t reserve_stack_slot();
};
#endif