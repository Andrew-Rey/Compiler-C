#pragma once

/*
 * this header is used to generate an LR(1) table
 * */

#include "Productions.h"
#include "Global.h"
#include "SynActions.h"

using alphabet::isTerminal;
using alphabet::isEps;
using alphabet::every;
using alphabet::tokenCount;

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
    if (!willReduce(prod)) {
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

// conduct 1 iteration
void generateClosure(Closure &state) {
    std::vector<Item> temp_closure = state;
    for (auto item: state) {
        Production current_prod = item.first;
        Token current_next_tok = item.second;
        // not reduce
        if (willReduce(current_prod)) {
            break;
        }
        Token candidate_tok = current_prod.right_[current_prod.current_];
        std::vector<int> candidate_prod_idx = findAllProduction(productions, candidate_tok);
        std::vector<Token> candidate_next = stringFirst(item);
        for (auto idx: candidate_prod_idx) {
            for (const auto &next_tok: candidate_next) {
                Item temp_item;
                temp_item.first = productions[idx];
                item.second = next_tok;
                if (!in(temp_closure, item)) {
                    temp_closure.push_back(item);
                }
            }
        }
    }
    state = temp_closure;
}

Closure generateClosureWrapper(Closure state) {
    Closure former_closure = state;
    while (true) {
        generateClosure(state);
        if (state.size() == former_closure.size()) {
            break;
        } else if (state.size() > former_closure.size()) {
            former_closure = state;
        } else {
            throw std::runtime_error("error in newClosure!");
        }
    }
    return state;
}

Closure nextClosure(const Closure &current_closure, const Token &x) {
    Closure new_closure;
    Closure it_closure = findCurrentProduction(current_closure, x);
    for (auto item: it_closure) {
        if (!willReduce(item.first)) {
            item.first.current_ += 1;
            new_closure.push_back(item);
        }
    }
    return new_closure;
}

void lr1Closure(std::vector<Closure> &closure_table) {
    std::vector<Closure> temp_table = closure_table;
    for (const auto &closure: closure_table) {
        for (const auto &tok: every()) {
            Closure next_closure = nextClosure(closure, tok);
            if (!in(closure_table, closure) && !closure.empty()) {
                temp_table.push_back(next_closure);
            }
        }
    }
    closure_table = temp_table;
}

std::vector<Closure> lr1ClosureWrapper(const ProductionTable &table) {
    Item init_item = std::pair<Production, NextToken>(Production(GLOBAL_START, {START_SIGN}), END_SIGN);
    std::vector<Closure> closure_table = {generateClosureWrapper(Closure{init_item})};
    std::vector<Closure> former_table = closure_table;
    while (true) {
        lr1Closure(closure_table);
        if (closure_table.size() == former_table.size()) {
            break;
        } else if (closure_table.size() > former_table.size()) {
            former_table = closure_table;
        } else {
            throw std::runtime_error("error in lr1ClosureWrapper!");
        }
    }
    return closure_table;
}

void generateLRTable(const ProductionTable &table) {
    std::vector<Closure> closure_table = lr1ClosureWrapper(table);
    // init table to error
    for (int i = 0; i < closure_table.size(); ++i) {
        std::vector<std::map<Token, Action> > init_vec;
        for (auto tok: every()) {
            std::map<Token, Action> init_map;
            init_map[tok] = error;
            init_vec.push_back(init_map);
        }
    }
    // fill the table
    for (int row = 0; row < closure_table.size(); ++row) {

    }
}

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