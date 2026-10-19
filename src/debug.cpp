#include "debug.hpp"

#include <iostream>

namespace Debug {

std::string token_to_string(const Token& token) {
    switch (token.id) {
        case (TokenID::_exit): return "_exit";
        case (TokenID::_int): return "_int(" + std::string(token.value.value_or("")) + ")";
        case (TokenID::_semi): return "_semi";
        case (TokenID::_ident): return "_ident(" + std::string(token.value.value_or("")) + ")";
        case (TokenID::_assign): return "_assign";
        case (TokenID::_plus): return "_plus";
        case (TokenID::_minus): return "_minus";
        case (TokenID::_mul): return "_mul";
        case (TokenID::_div): return "_div";
        case (TokenID::_mod): return "_mod";
        default: return "_NOT_DEFINED_TOKEN";
    }
}

std::string factor_to_string(const Node::Factor& factor) {
    return "Factor(" + token_to_string(factor.value) + ")";
}

std::string term_to_string(const Node::Term& term) {
    return std::visit([&](const auto& t) -> std::string {
        using T = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T, Node::Factor>) {
            return factor_to_string(t);
        } else if constexpr (std::is_same_v<T, Node::RecursiveTerm>) {
            std::string op_name;
            switch (t.math_operator.id) {
                case TokenID::_mul: op_name = " * "; break;
                case TokenID::_div: op_name = " / "; break;
                case TokenID::_mod: op_name = " % "; break;
                default: op_name = " ? "; break;
            }
            return "Term(" + term_to_string(*t.left_hand_side) + op_name + factor_to_string(t.right_hand_side) + ")";
        } else {
            static_assert(!sizeof(T), "term_to_string: unhandled Term alternative");
        }
    }, term.value);
}

std::string expr_to_string(const Node::Expression& expr) {
    return std::visit([](const auto& e) -> std::string {
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, Node::Term>) {
            return term_to_string(e);
        } else if constexpr (std::is_same_v<T, Node::RecursiveExpression>) {
            std::string op_name;
            switch (e.math_operator.id) {
                case TokenID::_plus: op_name = " + "; break;
                case TokenID::_minus: op_name = " - "; break;
                case TokenID::_mul: op_name = " * "; break;
                case TokenID::_div: op_name = " / "; break;
                case TokenID::_mod: op_name = " % "; break;
                default: op_name = " ? "; break;
            }
            return "Expr(" + expr_to_string(*e.left_hand_side) + op_name + term_to_string(e.right_hand_side) + ")";
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
            return "Assign(" + std::string(i.target.value.value_or("")) + " = " + expr_to_string(i.value) + ")";
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