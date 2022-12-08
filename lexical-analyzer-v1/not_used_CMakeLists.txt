cmake_minimum_required(VERSION 3.23)
project(lexical_analyzer_v1)

set(CMAKE_CXX_STANDARD 20)

include_directories(header src)
add_library(analyzerlib
        ${PROJECT_SOURCE_DIR}/header/Analyzer_demo.h
        header/LexicalAnalyzer.h
        header/LexIdentifier.h
        header/LexDigits.h
        header/LexExpr.h
        header/LexComment.h
        header/LexRelationExpr.h
        header/LexicalDefine.h
        ${PROJECT_SOURCE_DIR}/src/Analyzer_demo.cpp
        src/LexicalAnalyzer.cpp header/LexSeparator.h header/LexOperator.h)

add_executable(main)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC analyzerlib)
target_compile_definitions(main PUBLIC -DDATA_DIR="${PROJECT_SOURCE_DIR}")
