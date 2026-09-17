#include "debug.hpp"

#include <iostream>

namespace Debug {

std::string token_to_string(const Token& token) {
    switch (token.id) {
        case (TokenID::_exit): return "_exit";
        case (TokenID::_int): return "_int";
        case (TokenID::_semi): return "_semi";
        case (TokenID::_ident): return "_ident";
        case (TokenID::_assign): return "_assign";
        case (TokenID::_plus): return "_plus";
        default: return "_NOT_DEFINED_TOKEN";
    }
}

std::string term_to_string(const Node::Term& term) {
    return "Term(" + term.value.value.value_or("") + ")";
}

std::string expr_to_string(const Node::Expression& expr) {
    return std::visit([](const auto& e) -> std::string {
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, Node::Term>) {
            return term_to_string(e);
        } else if constexpr (std::is_same_v<T, Node::RecursiveExpression>) {
            return "Add(" + term_to_string(e.left_hand_side) + " + " + expr_to_string(*e.right_hand_side) + ")";
        } else {
            static_assert(!sizeof(T), "expr_to_string: unhandled Expression alternative");
        }
    }, expr.value);
}

std::string instr_to_string(const Node::Instruction& instr) {
    return std::visit([](const auto& i) -> std::string {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, Node::InstructionExit>) {
            return "Exit(" + expr_to_string(i.value) + ")";
        } else if constexpr (std::is_same_v<T, Node::InstructionAssign>) {
            return "Assign(" + i.target.value.value_or("") + " = " + expr_to_string(i.value) + ")";
        } else {
            static_assert(!sizeof(T), "instr_to_string: unhandled Instruction alternative");
        }
    }, instr.value);
}

std::string program_to_string(const std::vector<Node::Instruction>& program) {
    std::string result;
    for (const auto& instr : program) {
        result += instr_to_string(instr) + "\n";
    }
    return result;
}
    
}