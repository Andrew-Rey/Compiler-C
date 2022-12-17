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
    productions.push_back(*temp);
}

void generateSyntax() {
    addProduction("Program",    {"Sentence"});
    addProduction("Sentence",   {"Stmt"});
    addProduction("Sentence",   {"Func"});
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
    addProduction("Type",       {"int"});
    addProduction("RelationExpr", {"UnequalExpr"});
    addProduction("RelationExpr", {"EqualExpr"});
    addProduction("UnequalExpr",  {"OperationExpr"});
    addProduction("UnequalExpr",  {"UnequalExpr", "<", "AssignExpr"});
    addProduction("UnequalExpr",  {"UnequalExpr", ">", "AssignExpr"});
    addProduction("EqualExpr",  {"OperationExpr"});
    addProduction("EqualExpr",  {"UnequalExpr", "=", "AssignExpr"});
    addProduction("EqualExpr",  {"UnequalExpr", "!", "AssignExpr"});
    addProduction("AssignExpr", {"OperationExpr"});
    addProduction("AssignExpr", {"=", "OperationExpr"});
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
}

int findProduction(ProductionTable pt, int loc, const Token& tok) {
    if (alphabet::isTerminal(tok)) {
        return GO_ERROR;
    } else {
        for (int i = loc; i < pt.size(); ++i) {
            if (pt[i].left_ == tok) {
                return i;
            }
        }
        return GO_ERROR;
    }
}

//  find all productions whose left is `tok`
std::vector<int> findAllProduction(const ProductionTable& pt, const Token& tok) {
    std::vector<int> res;
    int loc = 0;
    while (loc != productions.size() - 1 && loc != GO_ERROR) {
        loc = findProduction(pt, loc + 1, tok);
        if (loc != GO_ERROR) {
            res.push_back(loc);
        }
    }
    return res;
}

// current reading token x, find out the items
std::vector<Item> findCurrentProduction(const Closure& closure, const Token& x) {
    if (closure.empty()) {
        return {};
    }
    std::vector<Item> res{};
    for (const auto& item: closure) {
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
            for (const auto& l: v.right_) {
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