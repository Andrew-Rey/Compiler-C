#pragma once

#include "Global.h"


void printAst(const std::string &file_out, AstNode *root) {
    if (root == nullptr) {
        throw std::runtime_error("empty parse tree can't be printed!");
    }
    std::fstream write_file{file_out, std::ios::out};
    write_file << "the AST is: " << endl;
    write_file << "|-> " << GLOBAL_START << endl;
    std::stack<AstNode *> print_stack;
    std::stack<int> tab_stack;
    print_stack.push(root);
    tab_stack.push(0);
    while (!print_stack.empty() && !tab_stack.empty()) {
        AstNode *cur_node = print_stack.top();
        int tab_num = tab_stack.top();
        print_stack.pop();
        tab_stack.pop();
        int child_count = (int) cur_node->getChildCount();
        for (int child = child_count - 1; child >= 0; --child) {
            // push to stack with inverse order
            print_stack.push(cur_node->child_[child]);
            tab_stack.push(tab_num + 1);
        }
        for (int tabs = 0; tabs <= tab_num; ++tabs) {
            write_file << "| ";
        }
        write_file << "|-> " << cur_node->getToken();
        if (cur_node->getValue() != "") {
            write_file << " : [" << cur_node->getValue() << "]";
        }
        write_file << endl;
    }
    write_file.close();

    cout << "-> AST generated successfully <-" << endl;
}