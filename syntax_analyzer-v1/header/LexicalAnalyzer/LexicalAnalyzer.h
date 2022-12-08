#pragma once

#include "LexicalDefine.h"
#include "LexIdentifier.h"
#include "LexDigits.h"
#include "LexComment.h"
#include "LexSeparator.h"
#include "LexOperator.h"
#include "../Global.h"

#include <vector>
#include <cstring>
#include <cassert>

using alphabet::reserve_;
using alphabet::non_reserve_;

typedef void(*dfa)(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p);

void addSymbolTable(const std::vector<char> &buffer) {}

void lexAnalyzer(const std::string &file_in) {
    std::string file_in_str = file_in;
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(std::ios::beg);
    file_read.close();

    long long read_p = std::ios::beg;
    StateType state = StateType::START;
    std::vector<char> buffer;
    std::vector<dfa> DFA;
    DFA.push_back(dfaIdentifier);
    DFA.push_back(dfaDigits);
    DFA.push_back(dfaComment);
    DFA.push_back(dfaSeparator);
    DFA.push_back(dfaOperator);

    int line_num = 0;

    auto alpha_find = [&](const std::string &candidate) {
        bool res = false;
        for (auto &it: reserve_) {
            const auto &temp = it;
            if (temp == candidate) {
                res = true;
                break;
            }
        }
        return res;
    };

    while (read_p < file_size) {
        for (auto &cur_dfa: DFA) {
            cur_dfa(file_in_str, state, buffer, read_p);
            switch (state) {

                case StateType::END_ID: {
                    auto res_it = isReservedWord(buffer);
                    if (res_it == reserved_words.end()) {
                        symbol_table.push_back(std::pair<Token, std::string>(
                                "Identifier",
                                std::string(buffer.begin(), buffer.end()))
                        );
                        state = StateType::START;
                    } else {
                        // reserved word
                        const std::string candidate = std::string(buffer.begin(), buffer.end());
                        if (alpha_find(candidate)) {
                            symbol_table.push_back(std::pair<Token, std::string>(
                                    candidate,
                                    candidate)
                            );
                            state = StateType::START;
                        } else {
                            state = StateType::ERROR;
                        }
                    }
                    buffer.clear();
                    break;
                }

                case StateType::END_SEP: {
                    const std::string candidate = std::string(buffer.begin(), buffer.end());
                    if (alpha_find(candidate)) {
                        symbol_table.push_back(std::pair<Token, std::string>(
                                candidate,
                                candidate)
                        );
                        state = StateType::START;
                    } else {
                        state = StateType::ERROR;
                    }
                    buffer.clear();
                    break;
                }

                case StateType::END_OP: {
                    const std::string candidate = std::string(buffer.begin(), buffer.end());
                    if (alpha_find(candidate)) {
                        symbol_table.push_back(std::pair<Token, std::string>(
                                candidate,
                                candidate)
                        );
                        state = StateType::START;
                    } else {
                        state = StateType::ERROR;
                    }
                    buffer.clear();
                    break;
                }

                case StateType::END_DIGITS: {
                    symbol_table.push_back(std::pair<Token, std::string>(
                            "Digits",
                            std::string(buffer.begin(), buffer.end()))
                    );
                    buffer.clear();
                    state = StateType::START;
                    break;
                }

                case StateType::END_CMT: {
                    state = StateType::START;
                    break;
                }

                case StateType::END: {
                    if (read_p < file_size) {
                        state = StateType::START;
                    }
                    break;
                }

                case StateType::ERROR: {
                    throw std::runtime_error("error");
                }

                default: {
                    state = StateType::START;
                    break;
                }
            }
        }
    }
    file_read.close();
}


