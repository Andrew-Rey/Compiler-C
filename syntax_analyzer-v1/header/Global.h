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

/** define the length
 * print, production length
 */
#define COUT_WIDTH  12
#define MAX_PRO_LEN 10

/** define the base pointers
 * reduce production mapping: 0th prod -> -3
 * state goto mapping: 0th state -> 0
 */
#define GO_SWITCH   0
#define GO_ERROR    -1
#define GO_ACCEPT   -2
#define GO_REDUCE   -3

using std::cout;
using std::cin;
using std::endl;
using std::setw;


// type definition
struct Production {
    int current_{};
    Token left_;
    std::vector<Token> right_;

    Production() = default;

    Production(Token left, std::vector<Token> right) {
        current_ = 0;
        left_ = std::move(left);
        right_ = std::move(right);
    }

    bool operator==(const Production &prod) const {
        return this->current_ == prod.current_ &&
               this->left_ == prod.left_ &&
               this->right_ == prod.right_;
    }

    Production &operator=(const Production &prod) = default;
};

struct AstNode {
    Token tok_{};
    int height_{};
    AstNode *child[MAX_PRO_LEN];

    AstNode() = default;

    AstNode(Token tok) {
        this->tok_ = std::move(tok);
        this->height_ = 0;
        for (auto c: this->child) {
            c = nullptr;
        }
    }

    void calculateHeight() {
        for (auto c: this->child) {
            if (c == nullptr) {
                break;
            }
            if (this->height_ < c->height_) {
                this->height_ = c->height_;
            }
        }
        this->height_ += 1;
    }

    void connectChild(std::vector<AstNode*> &children) {
        int tail = (int) children.size();
        if (tail == 0) {
            return;
        } else {
            for (int i = 0; i < tail; ++i) {
                this->child[i] = children.back();
                children.pop_back();
            }
        }
        this->calculateHeight();
    }
};

typedef Token                                            NextToken;
typedef std::pair<Production, NextToken>                 Item;
typedef std::vector<Item>                                Closure;
typedef std::map<Token, std::vector<int> >               LRTable;
typedef std::vector<std::pair<Token, std::string> >      SymbolTable;
typedef std::vector<Production>                          ProductionTable;
typedef std::map<Token, std::vector<Token> >             FirstSet;


// global variables
SymbolTable     symbol_table;
ProductionTable productions;
FirstSet        first_set;
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

inline int reduceIndex(const int& act) {
    return GO_REDUCE - act;
}

inline std::vector<Token> closure_next_token(const Closure &closure) {
    std::vector<Token> next_tokens;
    for (const auto& item: closure) {
        Production prod = item.first;
        if (willReduce(prod)) {
            if (!in(next_tokens, END_SIGN)) {
                next_tokens.emplace_back(END_SIGN);
            }
        } else {
            if (!in(next_tokens, prod.right_.at(prod.current_))) {
                next_tokens.emplace_back(prod.right_.at(prod.current_));
            }
        }
    }
    return next_tokens;
}

inline std::vector<Token> readTokens() {
    if (!symbol_table.empty()) {
        std::vector<Token> read_tokens;
        for (const auto &item: symbol_table) {
            read_tokens.push_back(item.first);
        }
        return read_tokens;
    } else {
        throw std::runtime_error("empty symbol table!");
    }
}