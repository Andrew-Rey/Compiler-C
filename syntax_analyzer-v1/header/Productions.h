#pragma once

/*
 * define the productions used in syntax parsing
 * */

#include "Global.h"


void addProduction(std::string left, std::vector<std::string> right) {
    auto *temp = new Production;
    temp->current_ = 0;
    temp->left_ = std::move(left);
    temp->right_ = std::move(right);
    // if right has only empty string, then is reducing state directly
    if (temp->right_ == std::vector<Token>{EMPTY_STR}) {
        temp->current_ += 1;
    }
    productions.push_back(*temp);
}

void generateSyntax() {
    addProduction(GLOBAL_START,    {START_SIGN});
    addProduction(START_SIGN,   {"Stmt"});
    addProduction(START_SIGN,   {"Func"});
    addProduction("Stmt",       {"VarStmt"});
    addProduction("Stmt",       {"VarStmt", "Stmt"});
    addProduction("Stmt",       {"Block"});
    addProduction("Stmt",       {"if", "(", "RelationExpr", ")", "Stmt"});
    addProduction("Stmt",       {"if", "(", "RelationExpr", ")", "Stmt", "else", "Stmt"});
    addProduction("Stmt",       {"while", "(", "RelationExpr", ")", "Stmt"});
    addProduction("VarStmt",    {"VarDecl"});
    addProduction("VarStmt",    {"VarDef"});
    addProduction("VarDecl",    {"Type", "Identifier", ";"});
    addProduction("VarDef",     {"Type", "Identifier", "=", "Expr", ";"});
    addProduction("Block",      {"{", "Stmt", "}"});
    addProduction("Expr",       {"RelationExpr"});
    addProduction("Expr",       {"OperationExpr"});
    addProduction("Func",       {"FuncDecl"});
    addProduction("Func",       {"FuncDef"});
    addProduction("FuncDecl",   {"Type", "Identifier", "(", "FuncParam", ")"});
    addProduction("FuncDef",    {"Type", "Identifier", "(", "FuncParam", ")", "Block"});
    addProduction("FuncParam",  {"Type", "Identifier"});
    addProduction("FuncParam",  {"Type", "Identifier", "FuncParam"});
    addProduction("Type",       {"int"});
    addProduction("RelationExpr", {"UnequalExpr"});
    addProduction("RelationExpr", {"EqualExpr"});
    addProduction("UnequalExpr", {"OperationExpr"});
    addProduction("UnequalExpr", {"UnequalExpr", "<", "OperationExpr"});
    addProduction("UnequalExpr", {"UnequalExpr", ">", "OperationExpr"});
    addProduction("UnequalExpr", {"UnequalExpr", "<=", "OperationExpr"});
    addProduction("UnequalExpr", {"UnequalExpr", ">=", "OperationExpr"});
    addProduction("EqualExpr", {"UnequalExpr"});
    addProduction("EqualExpr", {"EqualExpr", "==", "UnequalExpr"});
    addProduction("EqualExpr", {"EqualExpr", "!=", "UnequalExpr"});
    addProduction("OperationExpr", {"OperationExpr", "+", "Term"});
    addProduction("OperationExpr", {"OperationExpr", "-", "Term"});
    addProduction("OperationExpr", {"Term"});
    addProduction("Term",    {"Term", "*", "Factor"});
    addProduction("Term",    {"Term", "/", "Factor"});
    addProduction("Term",    {"Factor"});
    addProduction("Factor",  {"(", "OperationExpr", ")"});
    addProduction("Factor",  {"Digits"});
    addProduction("Factor",  {"Identifier"});

    // the productions below are used for testing
    /*
   addProduction(GLOBAL_START, {START_SIGN});
   addProduction(START_SIGN, {"0", START_SIGN, "1"});
   addProduction(START_SIGN, {"0", "1"});
    */

    // test with left recursion
    /*
    addProduction(GLOBAL_START, {START_SIGN});
    addProduction(START_SIGN, {START_SIGN, "(", START_SIGN, ")", START_SIGN});
    addProduction(START_SIGN, {EMPTY_STR});
     */
}

//  find all productions whose left is `tok`
std::vector<int> findAllProduction(const ProductionTable &pt, const Token &tok) {
    std::vector<int> res;
    for (int i = 0; i < productions.size(); ++i) {
        if (tok == productions[i].left_) {
            res.push_back(i);
        }
    }
    return res;
}

// current reading token x, find out the items
std::vector<Item> findCurrentProduction(const Closure &closure, const Token &x) {
    if (closure.empty()) {
        return {};
    }
    std::vector<Item> res{};
    for (const auto &item: closure) {
        Production prod = item.first;
        if (!willReduce(prod)) {
            if (prod.right_[prod.current_] == x) {
                res.push_back(item);
            }
        }
    }
    return res;
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
            for (const auto &l: v.right_) {
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