add_executable(zoomc 
    main.cpp 
    cli_parser.cpp
    lexer.cpp 
    parser.cpp 
    generator.cpp
    debug.cpp
)

target_include_directories(zoomc PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(zoomc PRIVATE -Wall -Wextra -Wpedantic)