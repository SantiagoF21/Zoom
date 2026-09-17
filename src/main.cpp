#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "lexer.hpp"
#include "parser.hpp"
#include "generator.hpp"

#include "debug.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: zoom <input.zm> [--debug]\n";
        return 1;
    }

    bool debug = (argc == 3 && std::string(argv[2]) == "--debug");
    
    std::string contents; {
        std::ifstream input(argv[1]);
        if (!input) {
            std::cerr << "Could not open file: " << argv[1] << '\n';
            return 1;
        }
        std::stringstream buffer;
        buffer << input.rdbuf();
        contents = buffer.str();
    }

    Lexer lexer(std::move(contents));
    std::vector<Token> tokens = lexer.tokenize();

    if (debug) {
        for (const Token& token : tokens) {
            std::cout << Debug::token_to_string(token) << " ";
            if (token.id == TokenID::_semi) {
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    Parser parser(std::move(tokens));
    std::vector<Node::Instruction> program = parser.parse_program();
    
    if (debug) {
        std::cout << Debug::program_to_string(program);
        std::cout << '\n';
    }

    Generator generator(std::move(program));
    std::string assembly = generator.generate_program();

    std::ofstream out("program.asm");
    if (!out) {
        std::cerr << "Could not write program.asm\n";
        return 1;
    }
    out << assembly;

    return 0;
}