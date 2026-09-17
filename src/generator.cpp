#include "generator.hpp"

#include <iostream>
#include <cstdlib>

Generator::Generator(std::vector<Node::Instruction> program) : m_program(std::move(program)) {}

std::string Generator::generate_term(const Node::Term& term) {
    if (term.value.id == TokenID::_int) {
        return "\tmov x0, #" + term.value.value.value_or("") + '\n';
    } else if (term.value.id == TokenID::_ident) {
        const std::string& name = term.value.value.value();
        if (!m_vars.contains(name)) {
            std::cerr << "Generator Error: Undeclared variable '" << name << "'\n";
            exit(1);
        }
        size_t offset = m_vars.at(name);
        return "\tldr x0, [sp, #" + std::to_string(offset) + "]\n";
    }
    return "";
}

std::string Generator::generate_expression(const Node::Expression& expr) {
    return std::visit([&](const auto& e) -> std::string {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, Node::Term>) {
            return generate_term(e);
        } else if constexpr (std::is_same_v<T, Node::RecursiveExpression>) {
            std::string output = generate_term(e.left_hand_side);
            output += "\tmov x9, x0\n";

            auto [push_text, left_hand_side_offset] = push("x9");
            output += push_text;

            output += generate_expression(*e.right_hand_side);

            output += "\tldr x9, [sp, #" + std::to_string(left_hand_side_offset) + "]\n";
            output += "\tadd x0, x9, x0\n";
            return output;
        }

    }, expr.value);
}

std::string Generator::generate_instruction(const Node::Instruction& instr) {
    return std::visit([&](const auto& i) -> std::string {
        using T = std::decay_t<decltype(i)>;

        if constexpr (std::is_same_v<T, Node::InstructionExit>) {
            std::string output = generate_expression(i.value);
            output += "\tadd sp, sp, #EXIT_STACK_SIZE\n";
            output += "\tret\n";
            return output;
        } else if constexpr (std::is_same_v<T, Node::InstructionAssign>) {
            std::string output = generate_expression(i.value);
            auto [push_text, offset] = push("x0");
            output += push_text;
            m_vars[i.target.value.value()] = offset;
            return output; 
        }
    }, instr.value);
}

std::string Generator::generate_program() {
    std::string output = ".global _main\n.align 2\n\n_main:\n";
    output += "\tsub sp, sp, #STACK_SIZE\n";

    for (const auto& instr : m_program) {
        output += generate_instruction(instr);
    }

    std::string final_size = std::to_string(m_stack_size);

    for (const std::string& placeholder : { std::string("STACK_SIZE"), std::string("EXIT_STACK_SIZE") }) {
        size_t pos = 0;
        while ((pos = output.find(placeholder, pos)) != std::string::npos) {
            output.replace(pos, placeholder.length(), final_size);
            pos += final_size.length();
        }
    }

    return output;
}

std::pair<std::string, size_t> Generator::push(const std::string& reg) {
    size_t offset = reserve_stack_slot();
    std::string asm_text = "\tstr " + reg + ", [sp, #" + std::to_string(offset) + "]\n";
    return {asm_text, offset};
}

size_t Generator::reserve_stack_slot() {
    size_t offset = m_stack_size;
    m_stack_size += 16;
    return offset;
}