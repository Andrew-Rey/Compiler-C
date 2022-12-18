#pragma once

/*
 * define non-terminals and terminal that will be used in productions
 * */

#include <vector>
#include <cstring>
#include <string>

#define GLOBAL_START "Program"
#define START_SIGN "Sentence"
#define EMPTY_STR "eps"
#define END_SIGN "$"

#define RESERVED_WORD_NUM 27  // 2
#define TERMINAL_NUM 32  // 4
#define NON_TERMINAL_NUM 22  // 2

typedef std::string Token;

namespace alphabet {

    // keep sequences order!
    std::vector<Token> total_token = {
            "if", "else", "while", "return", "break", "continue", "int", "void", ";", "," , "+", "-", "*", "/",
            "<", "<=", ">", ">=", "=", "==", "!=", "{", "}", "(", ")", "[", "]",

            "Identifier", "Digits", EMPTY_STR, END_SIGN,

            GLOBAL_START,
            // tokens
            START_SIGN,
            // for Sentence
            "Stmt", "Func",

            // for Stmt
            "VarStmt", "Block",
            // for VarStmt
            "Type", "Expr", "VarDecl", "VarDef", "VarAssign", "BlockItem",
            // for Expr
            "RelationExpr", "OperationExpr", "IdentList",
            // for Cond
            // RelationExpr
            // for RelationExpr
            "EqualExpr", "UnequalExpr",
            // for OperationExpr
            "Term", "Factor",

            // for Func
            "FuncDecl", "FuncDef",
            // for FuncDecl | FuncDef
            "FuncParam"

            // the alphabet below are used for testing
            /*
            "0", "1", EMPTY_STR, END_SIGN,

            GLOBAL_START, START_SIGN
             */

            // test with left recursion
            /*
            "(", ")", EMPTY_STR, END_SIGN,

            GLOBAL_START, START_SIGN
             */
    };

    bool isTerminal(const Token &tok) {
        bool res = false;
        for (int i = 0; i < TERMINAL_NUM; ++i) {
            if (tok == total_token[i]) {
                res = true;
                break;
            }
        }
        return res;
    }

    bool isEps(const Token &tok) {
        return tok == EMPTY_STR;
    }

    size_t tokenCount() {
        return NON_TERMINAL_NUM + TERMINAL_NUM;
    }

}