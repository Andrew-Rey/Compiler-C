#pragma once

/*
 * define non-terminals and terminal that will be used in productions
 * */

#include <vector>
#include <cstring>
#include <string>

typedef std::string Token;

namespace alphabet {

    const std::vector<Token> terminal_res_ = {
            "if", "else", "while", "return", "break", "continue", "int", "void", ";", ",", ":", ".", "+", "-", "*", "/",
            "<", "<=", ">", ">=", "=", "%", "!", "{", "}", "(", ")", "[", "]"
    };

    const std::vector<Token> terminal_non_res_ = {
            "Identifier", "Digits"
    };

    const std::vector<Token> non_terminal_ = {
            "Program",
            // tokens
            "Sentence",
            // for Sentence
            "Stmt", "Func",

            // for Stmt
            "VarStmt", "Block", "Cond",
            // for VarStmt
            "Type", "Expr", "VarDecl", "VarDef",
            // for Expr
            "RelationExpr", "OperationExpr",
            // for Cond
            // RelationExpr

            // for Func
            "FuncDecl", "FuncDef"
    };

    const std::vector<Token> boundary = {
            "eps", "$"
    };

    bool isTerminal(Token tok) {
        bool res = false;
        for (auto t: terminal_res_) {
            if (t == tok) {
                res = true;
                break;
            }
        }
        if (!res) {
            for (auto t: terminal_non_res_) {
                if (t == tok) {
                    res = true;
                    break;
                }
            }
        }
        return res;
    }

    bool isEps(Token tok) {
        return tok == "eps";
    }

}