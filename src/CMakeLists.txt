add_executable(zoom 
    main.cpp 
    cli_parser.hpp
    lexer.cpp 
    parser.cpp 
    generator.cpp
    debug.cpp
)

target_include_directories(zoom PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(zoom PRIVATE -Wall -Wextra -Wpedantic)