#pragma once

/*
 * this header is used to generate an LR(1) table
 * */

#include "Productions.h"
#include "Global.h"
#include <fstream>

using alphabet::isTerminal;
using alphabet::isEps;
using alphabet::tokenCount;


std::vector<Token> first(const Token &tok) {
    if (isTerminal(tok)) {
        // X = x
        return {tok};
    } else {
        // non-terminal
        std::vector<Token> first_set{};
        std::vector<int> prod_idx = findAllProduction(productions, tok);
        for (const auto &idx: prod_idx) {
            Production prod = productions[idx];
            Token candidate = prod.right_.front();
            if (isTerminal(candidate) && !in(first_set, candidate)) {
                // X -> a1 A2 ...
                first_set.emplace_back(prod.right_.front());
            } else {
                // the most common situation: X -> A1 A2 ...
                std::vector<Token> temp_first{};
                int i = 0;  // iteration, i-th non-terminal
                while (true) {
                    temp_first = first(candidate);
                    if (!in(temp_first, EMPTY_STR) || i == prod.right_.size()) {
                        break;
                    }
                    for (const auto &tf: temp_first) {
                        if (!in(first_set, tf) && tf != EMPTY_STR) {
                            first_set.emplace_back(tf);
                        }
                    }
                }
                if (in(temp_first, EMPTY_STR) &&
                    !in(first_set, EMPTY_STR) &&
                    i == prod.right_.size()) {
                    first_set.emplace_back(EMPTY_STR);
                }
            }
        }
        cout << "OK1" << endl;
        return first_set;
    }
}

std::vector<Token> itemFirst(const Item &item) {
    std::vector<Token> first_set{};
    Production prod = item.first;
    NextToken follow_tok = item.second;
    if (willReduce(prod)) {  // TODO: after ValueDef? following = while?
        return first_set;
    } else {
        NextToken next_tok;
        if (prod.current_ + 1 == prod.right_.size()) {
            // if reaches to the tail of production, the next token is empty string
            next_tok = EMPTY_STR;
            first_set.push_back(follow_tok);
        } else {
            next_tok = prod.right_.at(prod.current_ + 1);
            std::vector<Token> temp_first = first(next_tok);
            for (const auto &t: temp_first) {
                if (t != EMPTY_STR) {
                    first_set.push_back(t);
                } else {
                    first_set.push_back(follow_tok);
                }
            }
        }
        cout << "OK2" << endl;
        return first_set;
    }
}


// conduct 1 iteration
void generateClosure(Closure &state) {
    std::vector<Item> temp_closure = state;
    for (const auto &item: state) {
        Production current_prod = item.first;
        Token current_next_tok = item.second;
        // not reduce
        if (willReduce(current_prod)) {
            break;
        }
        Token candidate_tok = current_prod.right_[current_prod.current_];
        std::vector<int> candidate_prod_idx = findAllProduction(productions, candidate_tok);
        std::vector<Token> candidate_next = itemFirst(item);
        cout << "OK3" << endl;
        if (temp_closure.size() == 27) {
            cout << "there bugs" << endl;
        }
        for (auto idx: candidate_prod_idx) {
            for (const auto &next_tok: candidate_next) {
                Item temp_item;
                temp_item.first = productions[idx];
                temp_item.second = next_tok;
                if (!in(temp_closure, temp_item)) {
                    temp_closure.push_back(temp_item);
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
    std::vector<Item> it_closure = findCurrentProduction(current_closure, x);
    for (auto item: it_closure) {
        if (!willReduce(item.first)) {
            item.first.current_ += 1;
            new_closure.push_back(item);
        }
    }
    return generateClosureWrapper(new_closure);
}

void lr1Closure(std::vector<Closure> &closure_table) {
    std::vector<Closure> temp_table = closure_table;
    for (const auto &closure: closure_table) {
        for (const auto &tok: every()) {
            Closure next_closure = nextClosure(closure, tok);
            if (!in(closure_table, next_closure) && !next_closure.empty()) {
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

void printLR1Table();

void printClosureTable(const std::vector<Closure> &ct);

void generateLRTable(const ProductionTable &table) {
    std::vector<Closure> closure_table = lr1ClosureWrapper(table);
    printClosureTable(closure_table);
    // init table to error
    for (const auto &t: every()) {
        lr1_table[t] = std::vector<int>(closure_table.size(), GO_ERROR);
    }
    // actions
    auto go = [&](const Closure &cur_clo, const Token &cur_tok) {
        return closureToIdx(closure_table, nextClosure(cur_clo, cur_tok));
    };
    auto reduce = [&](const Production &prod) {
        std::vector<int> prod_vec_idx = findAllProduction(productions, prod.left_);
        int res = GO_ERROR;
        for (int i: prod_vec_idx) {
            if (prod.left_ == productions[i].left_ &&
                prod.right_ == productions[i].right_) {
                // use [i] to reduce
                res = GO_REDUCE - i;
                break;
            }
        }
        return res;
    };
    // fill the table
    for (int cur_state = 0; cur_state < closure_table.size(); ++cur_state) {
        for (const auto &item: every(closure_table[cur_state])) {
            Production prod = item.first;
            if (!willReduce(prod)) {
                Token cur_tok = prod.right_[prod.current_];
                lr1_table[cur_tok].at(cur_state) = go(closure_table[cur_state], cur_tok);
            } else {
                // will be reduced
                if (prod.left_ != GLOBAL_START) {
                    // not S prime
                    lr1_table[item.second].at(cur_state) = reduce(prod);
                } else if (prod.right_.size() == 1 && prod.right_[0] == START_SIGN && item.second == END_SIGN) {
                    lr1_table[item.second].at(cur_state) = GO_ACCEPT;
                } else {
                    lr1_table[item.second].at(cur_state) = GO_ERROR;
                }
            }
        }
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

void printLR1Table() {
    std::fstream write_file{"../output/LR1Table.txt", std::ios::out};
    if (!lr1_table.empty()) {
        write_file << "The LR1 table is:" << endl;
        for (const auto &tok: every()) {
            write_file << setw(COUT_WIDTH) << tok << " ";
            for (const auto &act: lr1_table[tok]) {
                write_file << setw(4) << act << " ";
            }
            write_file << endl;
        }
    }
    write_file.close();
}

void printClosureTable(const std::vector<Closure> &ct) {
    std::fstream write_file{"../output/LR1Closure.txt", std::ios::out};
    if (!ct.empty()) {
        int c_count = 0;
        for (const auto &c: ct) {
            write_file << setw(2) << c_count << " ";
            c_count += 1;
            for (const auto &i: c) {
                Production prod = i.first;
                write_file << "\t" << prod.left_ << " -> ";
                int dot = 0;
                for (const auto &s: prod.right_) {
                    write_file << s << " ";
                    dot += 1;
                    if (dot == prod.current_) {
                        write_file << "¡¤";
                    }
                }
                write_file << "\t" << "{";
                for (const auto &s: i.second) {
                    write_file << s << " ";
                }
                write_file << "}";
                write_file << endl;
            }
            write_file << endl;
        }
    }
    write_file.close();
}