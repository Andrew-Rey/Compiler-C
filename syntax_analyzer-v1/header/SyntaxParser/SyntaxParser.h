#pragma once

#include "Productions.h"
#include "Global.h"
#include "SynLRTable.h"

std::stack<Token> signal_stack;
std::stack<int> state_stack;

// using generated symbol table and production table to generate AST
void parse();