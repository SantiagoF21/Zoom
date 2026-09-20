#include "generator.hpp"

#include <iostream>
#include <cstdlib>

Generator::Generator(std::vector<Node::Instruction> program) : m_program(std::move(program)) {}

std::string Generator::generate_factor(const Node::Factor& factor) {
    if (factor.value.id == TokenID::_int) {
        return "\tmov x0, #" + factor.value.value.value_or("") + '\n';
    } else if (factor.value.id == TokenID::_ident) {
        const std::string& var_name = factor.value.value.value();
        if (!m_vars.contains(var_name)) {
            std::cerr << "Generator Error: Undeclared variable '" << var_name << "'\n";
            exit(1);
        }
        size_t offset = m_vars.at(var_name);
        return "\tldr x0, [sp, #" + std::to_string(offset) + "]\n";
    }
    return "";
}

std::string Generator::generate_term(const Node::Term& term) {
    return std::visit([&](const auto& t) -> std::string {
        using T = std::decay_t<decltype(t)>;

        if constexpr (std::is_same_v<T, Node::Factor>) {
            return generate_factor(t);
        } else if constexpr (std::is_same_v<T, Node::RecursiveTerm>) {
            std::string output = generate_term(*t.left_hand_side);
            output += "\tmov x9, x0\n";

            auto [push_text, left_hand_side_offset] = push("x9");
            output += push_text;

            output += generate_factor(t.right_hand_side);

            output += "\tldr x9, [sp, #" + std::to_string(left_hand_side_offset) + "]\n";

            switch (t.math_operator.id) {
                case TokenID::_mul:
                    output += "\tmul x0, x9, x0\n";
                    break;
                case TokenID::_div:
                    output += "\tsdiv x0, x9, x0\n";
                    break;
                case TokenID::_mod: {
                    output += "\tmov x10, x9\n";     // x10 = a
                    output += "\tmov x11, x0\n";     // x11 = b
                    output += "\tsdiv x0, x9, x0\n"; // x0 = a / b
                    output += "\tmul x0, x0, x11\n"; // x0 = (a / b) * b
                    output += "\tsub x0, x10, x0\n"; // x0 = a - ((a / b) * b)
                    break;
                }
                default:
                    std::cerr << "Generator Error: Unhandled Term operator\n";
                    exit(1);
            }
            return output;
        }
        return "";
    }, term.value);
}

std::string Generator::generate_expression(const Node::Expression& expr) {
    return std::visit([&](const auto& e) -> std::string {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, Node::Term>) {
            return generate_term(e);
        } else if constexpr (std::is_same_v<T, Node::RecursiveExpression>) {
            std::string output = generate_expression(*e.left_hand_side);
            output += "\tmov x9, x0\n";

            auto [push_text, left_hand_side_offset] = push("x9");
            output += push_text;

            output += generate_term(e.right_hand_side);

            output += "\tldr x9, [sp, #" + std::to_string(left_hand_side_offset) + "]\n";

            switch (e.math_operator.id) {
                case TokenID::_plus:
                    output += "\tadd x0, x9, x0\n";
                    break;
                case TokenID::_minus:
                    output += "\tsub x0, x9, x0\n";
                    break;
                case TokenID::_mul:
                    output += "\tmul x0, x9, x0\n";
                    break;
                case TokenID::_div:
                    output += "\tsdiv x0, x9, x0\n";
                    break;
                case TokenID::_mod: {
                    // a mod b = a - (b * (a / b))
                    // x0 currently has 'b' (right side), x9 has 'a' (left side)
                    output += "\tmov x10, x9\n";     // x10 = a
                    output += "\tmov x11, x0\n";     // x11 = b
                    output += "\tsdiv x0, x9, x0\n"; // x0 = a / b
                    output += "\tmul x0, x0, x11\n"; // x0 = (a / b) * b
                    output += "\tsub x0, x10, x0\n"; // x0 = a - ((a / b) * b)
                    break;
                }
                default:
                    std::cerr << "Generator Error: Unhandled operator\n";
                    exit(1);
            }
            return output;
        }

    }, expr.value);
}

std::string Generator::generate_instruction(const Node::Instruction& instr) {
    return std::visit([&](const auto& i) -> std::string {
        using T = std::decay_t<decltype(i)>;

        if constexpr (std::is_same_v<T, Node::InstructionExit>) {
            std::string output = generate_expression(i.value);
            output += "\tadd sp, sp, #__EPILOGUE_SIZE__\n";
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
    output += "\tsub sp, sp, #__PROLOGUE_SIZE__\n";

    for (const auto& instr : m_program) {
        output += generate_instruction(instr);
    }

    std::string final_size = std::to_string(m_stack_size);

    for (const std::string& placeholder : { std::string("__PROLOGUE_SIZE__"), std::string("__EPILOGUE_SIZE__") }) {
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