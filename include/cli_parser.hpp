#ifndef CLI_PARSER_HPP
#define CLI_PARSER_HPP

#include <string_view>
#include <string>

inline constexpr std::string_view HELP_MESSAGE = 
    "Zoom Compiler Usage:\n"
    "\tzoom <input.zm> [options]\n\n"
    "Options:\n"
    "\t-h, --help\t\tShow this help message\n"
    "\t-d, --debug\t\tPrint token stream and AST\n"
    "\t-S, --assembly\tStop after generating assembly\n"
    "\t-c, --object-file\tStop after generating object file\n"
    "\t-o, --output <file>\tSpecify output name (default: program)\n";

/* Enum is ordered by pipeline stage; the earliest requested stop wins. */
enum class BuildTarget {
    Assembly,
    ObjectFile,
    Executable
};

struct ArgvConfig {
    std::string input_file_path;
    std::string output_file_path = "program";

    bool debug_option = false;

    BuildTarget build_target = BuildTarget::Executable;
};

class CLIParser {
public:
    explicit CLIParser(int argc, char* argv[]);

    ArgvConfig parse_argv() const;
private:
    int m_argc;
    char** m_argv;
};

#endif