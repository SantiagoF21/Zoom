#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>

namespace Node {
    struct Term {
        Token value;
    };

    struct Expression;

    struct RecursiveExpression {
        Term left_hand_side;
        std::unique_ptr<Expression> right_hand_side;
    };

    struct Expression {
        std::variant<Term, RecursiveExpression> value;
    };

    struct InstructionExit {
        Expression expression;
    };

    struct InstructionAssign {
        Token ident;
        Expression expression;
    };

    struct Instruction {
        std::variant<InstructionExit, InstructionAssign> value;
    };
}

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::optional<Node::Term> parse_term();
    std::optional<Node::Expression> parse_expr();
    std::optional<Node::Instruction> parse_instr();

    std::vector<Node::Instruction> parse_program();
private:
    const std::vector<Token> m_tokens;
    size_t m_index{};

    std::optional<Token> peek(size_t index_offset = 0) const;
    Token consume();

    void expect(TokenID id, const std::string& err_msg);
};

#endif