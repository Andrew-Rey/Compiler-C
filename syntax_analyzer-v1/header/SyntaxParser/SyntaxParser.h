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
    read_tok.emplace_back(END_SIGN);
//    std::vector<AstNode* > root;
    token_stack.push(END_SIGN);
    state_stack.push(0);
    bool accept = false;
    bool error = false;
    int action = GO_ERROR;
    int read_p = 0;

    while (!accept && !error) {
        int top_stat = state_stack.top();
        // get the reading token first but do not push to stack
        Token next_tok = read_tok[read_p];
        action = lr1_table[next_tok][top_stat];
        if (action >= GO_SWITCH) {
            // do switch
            int next_stat = action;
            token_stack.push(next_tok);
            state_stack.push(next_stat);
            read_p += 1;
        }
        if (action <= GO_REDUCE) {
            // do reduce
            // the reading token can not be pushed into stack
            // get the reduced result and put it into reading line, before the reading token
            // so the read_p should minus 1
            int prod_idx = reduceIndex(action);
            int reduce_len = (int) productions[prod_idx].right_.size();
            Token reduce_res = productions[prod_idx].left_;
            // generate AST node
//            auto *parent = new AstNode(reduce_res);
//            std::vector<AstNode *> children;
            for (int i = 0; i < reduce_len; ++i) {
//                next_tok = token_stack.top();
//                auto *child = new AstNode(next_tok);
//                children.push_back(child);
//                root.push_back(child);
                token_stack.pop();
                state_stack.pop();
            }
//            parent->connectChild(children);
            // update stack and reading line
            read_p -= 1;
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

    if (error  || read_p < read_tok.size() - 1) {
        cout << "error occurred in: " << read_p << endl;
    }
    if (accept) {
        cout << "parse finished successfully!" << endl;
//        printAst(root);
    }
}

/**
 * this function is a wrapper of `parse`,
 * which realizes reading from existed lr1 table to main memory
 */
void parseOffLine(const std::string &file_in) {
    std::fstream file_read{file_in, std::ios::in};
    std::string temp;
    Token tok;
    while (file_read >> temp) {
        if (in(alphabet::total_token, temp)) {
            tok = temp;
            continue;
        }
        lr1_table[tok].push_back(std::stoi(temp));
    }
    parse();
}