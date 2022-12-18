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

    // comment
    ENTER_CMT, CMT_S1, INNER_CMT_S2, CMT_S3, END_CMT,

    // end
    END, ERROR
};

const std::set<std::string> reserved_words = {
        "int", "while", "if", "else", "return", "void", "break", "continue",
};

const std::set<char> separators = {
        '(', ')', '{', '}', '[', ']', ',', ';', ':', '.'
};

const std::set<char> operators = {
        '+', '-', '=', '*', '/', '!', '<', '>', '%'
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