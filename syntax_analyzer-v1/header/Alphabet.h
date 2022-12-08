#pragma once

/*
 * define non-terminals and terminal that will be used in productions
 * */

#include <vector>
#include <cstring>
#include <string>

typedef std::string Token;

namespace alphabet {
    const std::vector<Token> reserve_ = {
            "if", "else", "while", "return", "break", "continue", "int", "void", ";", ",", ":", ".", "+", "-", "*", "/",
            "<", "<=", ">", ">=", "=", "%", "!", "{", "}", "(", ")", "[", "]"
    };
    const std::vector<Token> non_reserve_ = {
            "Program",
            // tokens
            "Sentence",
            // for Sentence
            "Stmt", "Func",

            // for Stmt
            "VarStmt", "Block", "Cond",
            // for VarStmt
            "Type", "Expr", "VarDecl", "VarDef", "Identifier", "Digits",
            // for Expr
            "RelationExpr", "OperationExpr",
            // for Cond
            // RelationExpr

            // for Func
            "FuncDecl", "FuncDef"
    };
}

/*const std::set<Token> alphabet = {
        // reserved words
        "if", "else", "while", "return", "break", "continue", "int", "void", ";", ",", ":", ".", "+", "-", "*", "/",
        "<", "<=", ">", ">=", "=", "%", "!", "{", "}", "(", ")", "[", "]",

        // tokens
        "Sentence",
        // for Sentence
        "Stmt", "Func",

        // for Stmt
        "VarStmt", "Block", "Cond",
        // for VarStmt
        "Type", "Expr", "VarDecl", "VarDef", "Identifier",
        // for Expr
        "RelationExpr", "OperationExpr",
        // for Cond
        // RelationExpr

        // for Func
        "FuncDecl", "FuncDef"

};*/