add_executable(zoom 
    main.cpp 
    lexer.cpp 
    parser.cpp 
    generator.cpp
)

target_include_directories(zoom PRIVATE ${PROJECT_SOURCE_DIR}/include)