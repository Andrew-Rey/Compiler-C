#pragma once

#include <string>
#include <set>
#include <vector>

enum struct StateType {
    // start
    START,

    // separator
    ENTER_SEP, END_SEP,

    // operator
    ENTER_OP, END_OP,

    // identifier
    ENTER_ID, IN_ID, END_ID,

    // digits
    ENTER_DIGITS, IN_DIGITS, END_DIGITS,

    // operation expression
    ENTER_EXPR, IS_MUL_EXPR, EXPR_S2, EXPR_S3, IS_ADD_EXPR, EXPR_S5, END_EXPR,

    // relation expression
    ENTER_UNEQ_EXPR, ENTER_EQ_EXPR, UNEQ_EXPR_S1, IS_GL_EXPR, IS_UNEQ_EXPR, EQ_EXPR_S1, IS_EQ_EXOR, IS_NOEQ_EXPR,
    END_UNEQ_EXPR, END_EQ_EXPR,

    // comment
    ENTER_CMT, CMT_S1, INNER_CMT_S2, CMT_S3, END_CMT,

    // end
    END, ERROR
};

const std::set<std::string> reserved_words = {
        "int", "while", "if", "else", "return", "void"
};

const std::set<char> separators = {
        '(', ')', '{', '}', '[', ']', ',', ';', ':', '.'
};

const std::set<char> operators = {
        '+', '-', '=', '*', '/', '!', '<', '>'
};

bool isSeparator(char c) {
    if (separators.find(c) != separators.end()) { return true; }
    else { return false; }
}

bool isOperator(char c) {
    if (operators.find(c) != operators.end()) { return true; }
    else { return false; }
}

std::set<std::string>::iterator isReservedWord(std::vector<char> buf) {
    std::string word;
    word.assign(buf.begin(), buf.end());
    auto result_it = reserved_words.find(word);
    return result_it;
}