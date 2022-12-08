#pragma once

/*
 * define the productions used in syntax parsing
 * */

#include "Global.h"

void generateSyntax() {
    addProduction("Program",    {"Sentence"});
    addProduction("Sentence",   {"Stmt", "Func"});
    addProduction("Stmt",       {"VarStmt"});
    addProduction("Stmt",       {"VarStmt", "Stmt"});
    addProduction("Stmt",       {"Block"});
    addProduction("Stmt",       {"if", "(", "Cond", ")", "Stmt"});
    addProduction("Stmt",       {"if", "(", "Cond", ")", "Stmt", "else", "Stmt"});
    addProduction("Stmt",       {"while", "(", "Cond", ")", "Stmt"});
    addProduction("VarStmt",    {"VarDecl"});
    addProduction("VarStmt",    {"VarDef"});
    addProduction("VarDecl",    {"Type", "Identifier", ";"});
    addProduction("VarDef",     {"Type", "Identifier", "=", "Expr", ";"});
    addProduction("Block",      {"{", "Stmt", "}"});
    addProduction("Cond",       {"RelationExpr"});
    addProduction("Expr",       {"RelationExpr"});
    addProduction("Expr",       {"OperationExpr"});
    addProduction("Func",       {"FuncDecl"});
    addProduction("Func",       {"FuncDef"});
    addProduction("FuncDecl",   {"Type", "Identifier", "(", "FuncParam", ")"});
    addProduction("FuncDef",    {"Type", "Identifier", "(", "FuncParam", ")", "Block"});
    addProduction("FuncParam",  {"Type", "Identifier"});
    addProduction("FuncParam",  {"Type", "Identifier", "FuncParam"});
}

void printProductions() {
    if (!productions.empty()) {
        cout << "The Productions we use:" << endl;

        for (int _ = 0; _ < 50; ++_) {
            cout << "=";
        }
        cout << endl;

        for (int i = 0; i < productions.size(); ++i) {
            auto v = productions[i];
            cout << setw(4) << "(" << i << ")" << setw(COUT_WIDTH) << v.left_ << " -> ";
            for (auto l: v.right_) {
                cout << l << " ";
            }
            cout << endl;
        }

        for (int _ = 0; _ < 50; ++_) {
            cout << "=";
        }
        cout << endl;
    }
}