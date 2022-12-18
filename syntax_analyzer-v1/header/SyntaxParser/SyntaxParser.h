#pragma once

#include "Productions.h"
#include "Global.h"
#include "SynLRTable.h"
#include "SynAst.h"

std::stack<Token> token_stack;
std::stack<int> state_stack;

// using generated symbol table and production table to generate AST

/**
 * SymbolTable: vector -> pair<Token, string>
 * index: symbol_table[idx].first
 * the lexical analysis result can be got using `readToken()`
 */
void parse() {
    std::vector<Token> read_tok = readTokens();
    token_stack.push(END_SIGN);
    state_stack.push(0);
    bool accept = false;
    bool error = false;
    int action = GO_ERROR;
    int read_p = 0;

    while (!accept && !error) {
        if (read_p == read_tok.size()) {
            break;
        }
        token_stack.push(read_tok[read_p]);
        Token top_tok = token_stack.top();
        int top_stat = state_stack.top();
        action = lr1_table[top_tok][top_stat];
        if (action >= GO_SWITCH) {
            // do switching
            int next_stat = action;
            state_stack.push(next_stat);
            read_p += 1;
        }
        if (action <= GO_REDUCE) {
            // do reducing
            int prod_idx = reduceIndex(action);
            int reduce_len = (int) productions[prod_idx].right_.size();
            Token reduce_res = productions[prod_idx].left_;
            auto *parent = new AstNode(reduce_res);
            std::vector<AstNode *> children;
            for (int i = 0; i < reduce_len; ++i) {
                auto *child = new AstNode(top_tok);
                children.push_back(child);
                token_stack.pop();
                top_tok = token_stack.top();
            }
            parent->connectChild(children);
            read_tok[read_p] = reduce_res;
        }
        if (action == GO_ERROR) {
            // do error
            error = true;
        }
        if (action == GO_ACCEPT) {
            // do accept
            accept = true;
        }
    }

    if (error) {
        cout << "error occurred in: " << read_p << endl;
    }
    if (accept) {
        cout << "parse finished successfully!" << endl;
        printAst();
    }
}