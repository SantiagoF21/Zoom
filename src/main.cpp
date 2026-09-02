#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <cctype>

/*
    Zoom (.zm)
*/

enum class TokenType {
    _return,
    _int_lit,
    _semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str) {
    for (char c : str) {
        std::cout << c << std::endl;
    }
    std::vector<Token> v;
    return v;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "zoom <input.zm>" << std::endl;
        return 1;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    tokenize(contents);

    std::cout << contents << std::endl;
    
    std::cout << argv[1] << std::endl;
    return 0;
}