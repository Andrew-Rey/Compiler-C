cmake_minimum_required(VERSION 3.20)

project(syntax_analyzer_v1)

set(CMAKE_CXX_STANDARD 17)

include_directories(header src
        header/LexicalAnalyzer
        header/SyntaxParser)

file(GLOB_RECURSE compiler_file
        header/*.h
        header/LexicalAnalyzer/*.h
        header/SyntaxParser/*.h
        src/*.h
        src/LexicalAnalyzer/*.cpp
        src/SyntaxParser/*.cpp)

add_library(compilerlib ${compiler_file})

add_executable(main)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC compilerlib)
target_compile_definitions(main PUBLIC -DDATA_DIR="${PROJECT_SOURCE_DIR}")
target_compile_definitions(main PUBLIC -DLR_DIR="${PROJECT_SOURCE_DIR}")

