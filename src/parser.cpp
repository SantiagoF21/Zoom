#include "parser.hpp"

#include <cstdlib>
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

std::optional<Node::Term> Parser::parse_term() {
    if (peek() && peek()->id == TokenID::_ident) {
        return Node::Term{consume()};
    }
    if (peek() && peek()->id == TokenID::_int) {
        return Node::Term{consume()};
    }
    return {};
}

std::optional<Node::Expression> Parser::parse_expr() {
    auto term = parse_term();
    if (!term) {
        return {};
    }

    if (peek() && peek()->id == TokenID::_plus) {
        consume();
        auto right_hand_side = parse_expr();
        if (!right_hand_side) {
            std::cerr << "Parse Error: Expected expression after '+'\n";
            exit(1);
        }
        Node::RecursiveExpression recursive_expression{std::move(*term), std::make_unique<Node::Expression>(std::move(*right_hand_side))};
        return Node::Expression{std::move(recursive_expression)};
    }

    return Node::Expression{std::move(*term)};
}

std::optional<Node::Instruction> Parser::parse_instr() {
    if (peek() && peek()->id == TokenID::_exit) {
        consume();
        auto expr = parse_expr();
        if (!expr) {
            std::cerr << "Parse Error: Expected expression after exit\n";
            exit(1);
        }
        expect(TokenID::_semi, "Expected ';' after exit statement");
        return Node::Instruction{Node::InstructionExit{std::move(*expr)}};
    }

    if (peek() && peek()->id == TokenID::_ident) {
        Token ident = consume();
        expect(TokenID::_assign, "Expected '=' after identifier");
        auto expr = parse_expr();
        if (!expr) {
            std::cerr << "Parse Error: Expected expression after '='\n";
            exit(1);
        }
        expect(TokenID::_semi, "Expected ';' after assignment");
        return Node::Instruction{Node::InstructionAssign{ident, std::move(*expr)}};
    }

    return {};
}

std::vector<Node::Instruction> Parser::parse_program() {
    std::vector<Node::Instruction> instructions;
    bool seen_exit = false;
    while (peek()) {
        if (seen_exit) {
            std::cerr << "Warning: Unreachable code after 'exit' statement\n";
        }
        auto instr = parse_instr();
        if (!instr) {
            std::cerr << "Parse Error: Expected instruction\n";
            exit(1);
        }
        if (std::holds_alternative<Node::InstructionExit>(instr->value)) {
            seen_exit = true;
        }
        instructions.push_back(std::move(*instr));
    }
    return instructions;
}

std::optional<Token> Parser::peek(size_t index_offset) const {
    if (m_index + index_offset >= m_tokens.size()) {
        return {};
    } else {
        return m_tokens[m_index + index_offset];
    }
}

Token Parser::consume() {
    return m_tokens.at(m_index++);
}

void Parser::expect(TokenID id, const std::string& err_msg) {
    if (!peek() || peek()->id != id) {
        std::cerr << "Parse Error: " << err_msg << '\n';
        exit(1);
    }
    consume();
}