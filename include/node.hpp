#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <variant>

#include "token.hpp"

namespace Node {
    struct Factor{
        Token value;
    };

    struct Term;

    struct RecursiveTerm {
        Factor left_hand_side;
        Token math_operator;
        std::unique_ptr<Term> right_hand_side;
    };

    struct Term {
        std::variant<Factor, RecursiveTerm> value;
    };

    struct Expression;

    struct RecursiveExpression {
        Term left_hand_side;
        Token math_operator;
        std::unique_ptr<Expression> right_hand_side;
    };

    struct Expression {
        std::variant<Term, RecursiveExpression> value;
    }; 

    struct InstructionExit {
        Expression value;
    };

    struct InstructionAssign {
        Token target;
        Expression value;
    };

    struct Instruction {
        std::variant<InstructionExit, InstructionAssign> value;
    };
}

#endif