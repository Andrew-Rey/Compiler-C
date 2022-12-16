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
#include <algorithm>

#define COUT_WIDTH  12
#define MAX_PRO_LEN 10
#define GO_ACCEPT   200
#define TAIL        -1
#define GO_ERROR    -1

using std::cout;
using std::cin;
using std::endl;
using std::setw;


// type definition
struct Production {
    int current_{};
    int loc_{};
    Token left_;
    std::vector<Token> right_;

    Production() = default;

    Production(Token left, std::vector<Token> right) {
        current_ = 0;
        loc_ = 0;
        left_ = std::move(left);
        right_ = std::move(right);
    }

    bool operator==(const Production &prod) const {
        return this->current_ == prod.current_ &&
               this->loc_ == prod.loc_ &&
               this->left_ == prod.left_ &&
               this->right_ == prod.right_;
    }

    Production &operator=(const Production &prod) = default;
};

struct astNode {
    Token tok_;
    std::string value_;
    astNode *child[MAX_PRO_LEN];
};

typedef Token NextToken;
typedef std::pair<Production, NextToken>                 Item;
typedef std::vector<Item>                                Closure;
typedef std::map<Token, std::vector<int> >               LRTable;
typedef std::vector<std::pair<Token, std::string> >      SymbolTable;
typedef std::vector<Production>                          ProductionTable;


// global variables
SymbolTable     symbol_table;
ProductionTable productions;
LRTable         lr1_table;


// global functions
inline bool in(std::vector<Token> tok_vec, Token tok) {
    return std::any_of(tok_vec.begin(), tok_vec.end(), [&](const Token &t) { return t == tok; });
}

inline bool in(std::vector<Item> closure, Item item) {
    return std::any_of(closure.begin(), closure.end(), [&](const Item &i) {
        return i.second == item.second && i.first == item.first;
    });
}

inline bool in(std::vector<Closure> closure_table, Closure closure) {
    return std::any_of(closure_table.begin(), closure_table.end(), [&](const Closure &c) { return c == closure; });
}

// return the idx of closure in closure table
inline int closureToIdx(const std::vector<Closure> &closure_table, const Closure &closure) {
    for (int idx = 0; idx < closure_table.size(); ++idx) {
        if (closure_table[idx] == closure) {
            return idx;
        }
    }
    return GO_ERROR;
}

inline bool willReduce(const Production &prod) {
    return prod.current_ == prod.right_.size();
}

inline std::vector<Token> every(const std::vector<Token> &tok_vec = alphabet::total_token) {
    return tok_vec;
}

inline std::vector<Item> every(const std::vector<Item> &closure) {
    return closure;
}

inline std::vector<Token> closure_next_token(const Closure &closure) {
    std::vector<Token> next_tok_set;
    for (const auto &item: closure) {
        Production prod = item.first;
        if (!willReduce(prod)) {
            Token tok = prod.right_[prod.current_ + 1];
            if (!in(next_tok_set, tok)) {
                next_tok_set.emplace_back(tok);
            }
        } else {
            if (!in(next_tok_set, END_SIGN)) {
                next_tok_set.emplace_back(END_SIGN);
            }
        }
    }
    return next_tok_set;
}