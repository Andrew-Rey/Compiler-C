#pragma once

/*
 * this header is used to generate an LR(1) table
 * */

#include "Productions.h"
#include "Global.h"
#include "SynActions.h"

using alphabet::isTerminal;
using alphabet::isEps;

bool in(std::vector<Token> tok_vec, Token tok) {
    return std::any_of(tok_vec.begin(), tok_vec.end(), [&](const Token &t) { return t == tok; });
}

bool in(std::vector<Item> closure, Item item) {
    return std::any_of(closure.begin(), closure.end(), [&](const Item &i) {
        return i.second == item.second && i.first == item.first;
    });
}

void first_rec(const Token &tok, std::vector<Token> &res) {
    if (in(res, tok)) {
        return;
    }
    if (isEps(tok)) {
        res.emplace_back("eps");
        return;
    } else if (isTerminal(tok)) {
        res.emplace_back(tok);
        return;
    } else {
        std::vector<int> prod_idx = findAllProduction(productions, tok);
        for (auto idx: prod_idx) {
            Token tok_rec = productions[idx].right_.front();
            first_rec(tok_rec, res);
        }
    }
}

// attention the result is a vector, not a single token
std::vector<Token> first(std::vector<Token> tok_vec) {
    std::vector<Token> res;
    Token tok = tok_vec.front();
    first_rec(tok, res);
    return res;
}

std::vector<Token> stringFirst(const Item &item) {
    Production prod = item.first;
    // when production is not going to be reduced
    if (prod.current_ == prod.right_.size()) {
        Token next_tok = item.second;
        std::vector<Token> res;
        std::vector<Token> tok_vec;
        tok_vec.assign(prod.right_.begin() + prod.current_ + 1, prod.right_.end());
        tok_vec.push_back(next_tok);
        std::vector<Token> temp = first(tok_vec);
        // judge if the set is updated
        if (res.size() < temp.size()) {
            res = temp;
        }
        return res;
    }
    return {};
}

int generateClosure(Closure state) {
}

int nextClosure(int cur_idx, Token x);

std::vector<Closure> lr1Closure(ProductionTable table);

void generateLRTable();

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