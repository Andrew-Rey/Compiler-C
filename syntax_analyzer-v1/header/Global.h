#pragma once

/*
 * define the global variables and macros, including:
 * types:
 * - token
 * - astNode
 * - closure
 * - item
 * */

#include "Alphabet.h"

#include <utility>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <cstring>
#include <iostream>
#include <iomanip>

#define COUT_WIDTH 12
#define MAX_PRO_LEN 10

using std::cout;
using std::cin;
using std::endl;
using std::setw;

struct Production {
    int current_{};
    Token left_;
    std::vector<Token> right_;
};

struct astNode {
    Token tok_;
    std::string value_;
    astNode *child[MAX_PRO_LEN];
};

typedef std::set<Token> NextToken;

typedef std::pair<Production, NextToken> Item;

typedef std::vector<Item> Closure;

typedef int(*Action)(int idx);

typedef std::vector<std::vector<Action> > LRTable;

typedef std::vector<std::pair<Token, std::string> > SymbolTable;

typedef std::vector<Production> ProductionTable;

// global variables
SymbolTable symbol_table;
ProductionTable productions;

// global functions
void printSymbolTable() {
    if (!symbol_table.empty()) {
        cout << "The symbol table is:" << endl;
        for (auto _ = 0; _ < 50; ++_) {
            cout << "=";
        }
        cout << endl;

        for (const auto &item: symbol_table) {
            cout << setw(COUT_WIDTH) << item.second << setw(COUT_WIDTH) << item.first << endl;
        }

        for (auto _ = 0; _ < 50; ++_) {
            cout << "=";
        }
        cout << endl;
    }
}

void addProduction(std::string left, std::vector<std::string> right) {
    auto *temp = new Production;
    temp->current_ = -1;
    temp->left_ = std::move(left);
    temp->right_ = std::move(right);
    productions.push_back(*temp);
}