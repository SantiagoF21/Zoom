#include "cli_parser.hpp"

#include <getopt.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>

CLIParser::CLIParser(int argc, char** argv) : m_argc(argc), m_argv(argv) {}

ArgvConfig CLIParser::parse_argv() const {
    ArgvConfig config;

    const struct option long_opts[] = {
        {"help",        no_argument,        nullptr, 'h'},
        {"debug",       no_argument,        nullptr, 'd'},
        {"assembly",    no_argument,        nullptr, 'S'},
        {"object-file", no_argument,        nullptr, 'c'},
        {"output",      required_argument,  nullptr, 'o'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    optind = 1;
    while ((opt = getopt_long(m_argc, m_argv, "hdSo:c", long_opts, nullptr)) != -1) {
        switch (opt) {
            case 'd': config.debug_option = true; break;
            case 'S': config.build_target = std::min(config.build_target, BuildTarget::Assembly); break;
            case 'c': config.build_target = std::min(config.build_target, BuildTarget::ObjectFile); break;
            case 'o': config.output_file_path = optarg; break;
            case 'h': std::cout << HELP_MESSAGE; std::exit(0); break;
            case '?': /* Defaults to getopt error messages */ std::cerr << "Try 'zoom --help' for documentation.\n"; std::exit(1); break;
            default: std::cerr << "CLIParsing Error: Unhandled option (bug in parser)\n"; std::exit(1); break;
        }
    }

    if (optind >= m_argc) {
        std::cerr << "CLIParsing Error: No input file specified.\n";
        std::exit(1);
    }
    if (optind + 1 < m_argc) {
        std::cerr << "CLIParsing Error: Multiple input files provided. Only one is supported.\n";
        std::exit(1);
    }
    std::string input_file_path = m_argv[optind];
    if (!input_file_path.ends_with(".zm")) {
        std::cerr << "CLIParsing Error: '" << input_file_path << "' is not a .zm file.\n";
        std::exit(1);
    }
    config.input_file_path = std::move(input_file_path);

    return config;
}