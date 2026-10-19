#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "cli_parser.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "generator.hpp"

#include "debug.hpp"

static inline void run_build_step(const std::string& command) {
    std::cout << "Executing: " << command << std::endl;
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Command failed with exit code: " << result << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    CLIParser cli_parser = CLIParser(argc, argv);
    ArgvConfig config = cli_parser.parse_argv(); 
    
    std::string contents; {
        std::ifstream input(config.input_file_path);
        if (!input) {
            std::cerr << "Could not open file: " << config.input_file_path << '\n';
            return 1;
        }
        std::stringstream buffer;
        buffer << input.rdbuf();
        contents = buffer.str();
    }

    Lexer lexer(std::move(contents));
    std::vector<Token> tokens = lexer.tokenize();

    if (config.debug_option) {
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
    
    if (config.debug_option) {
        std::cout << Debug::program_to_string(program);
        std::cout << '\n';
    }

    Generator generator(std::move(program));
    std::string assembly = generator.generate_program();

    std::cout << "--- ASSEMBLY START ---\n" << assembly << "\n--- ASSEMBLY END ---\n";

    std::string asm_file = config.output_file_path + ".asm";
    std::cout << "Writing assembly (" << assembly.length() << " bytes) to " << asm_file << "..." << std::endl;
    std::ofstream out(asm_file);
    if (!out) {
        std::cerr << "Could not write " << asm_file << "\n";
        return 1;
    }
    out << assembly;
    out.close();
    std::cout << "Assembly written to " << asm_file << std::endl;

    if (config.build_target == BuildTarget::ObjectFile) {
        std::string obj_file = config.output_file_path + ".o";
        std::string cmd = "g++ -c " + asm_file + " -o " + obj_file;
        run_build_step(cmd);
        std::cout << "Object file created: " << obj_file << std::endl;
        return 0;
    }

    if (config.build_target == BuildTarget::Executable) {
        std::string cmd = "g++ " + asm_file + " -o " + config.output_file_path;
        run_build_step(cmd);
        std::cout << "Executable created: " << config.output_file_path << std::endl;
    }
    
    return 0;
}