#pragma once

#include "Productions.h"
#include "Global.h"
#include "SynLRTable.h"

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
}