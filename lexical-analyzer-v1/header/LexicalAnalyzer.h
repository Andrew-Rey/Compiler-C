#pragma once

#include "LexicalDefine.h"
#include "LexIdentifier.h"
#include "LexDigits.h"
#include "LexComment.h"
#include "LexSeparator.h"
#include "LexOperator.h"

#include <vector>
#include <cstring>
#include <cassert>

typedef void(*dfa)(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p);

void lexAnalyzer(const std::string &file_in, const std::string &file_out) {
    std::string file_in_str = file_in;
    std::string file_out_str = file_out;
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(std::ios::beg);
    file_read.close();

    std::ofstream file_write{file_out, std::ios::out | std::ios::binary};
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

    while (read_p < file_size) {
        for (auto &cur_dfa: DFA) {
            cur_dfa(file_in_str, state, buffer, read_p);
            switch (state) {
                case StateType::END_ID: {
                    auto res_it = isReservedWord(buffer);
                    if (res_it == reserved_words.end()) {
                        file_write.write("Identifier: ", strlen("Identifier: ") * sizeof(char));
                    } else {
                        // reserved word
                        file_write.write("ReservedWord: ", strlen("ReservedWord: ") * sizeof(char));
                    }
                    file_write.write(buffer.data(), buffer.size() * sizeof(char));
                    file_write.write("\n", sizeof(char));
                    buffer.clear();
                    state = StateType::START;
                    break;
                }
                case StateType::END_SEP:
                    file_write.write(buffer.data(), buffer.size() * sizeof(char));
                    file_write.write("\n", sizeof(char));
                    buffer.clear();
                    state = StateType::START;
                    break;
                case StateType::END_OP:
                    file_write.write(buffer.data(), buffer.size() * sizeof(char));
                    file_write.write("\n", sizeof(char));
                    buffer.clear();
                    state = StateType::START;
                    break;
                case StateType::END_DIGITS:
                    file_write.write("Digits: ", strlen("Digits: ") * sizeof(char));
                    file_write.write(buffer.data(), buffer.size() * sizeof(char));
                    file_write.write("\n", sizeof(char));
                    buffer.clear();
                    state = StateType::START;
                    break;
                case StateType::END_CMT:
                    state = StateType::START;
                    break;
                case StateType::END:
                    if (read_p < file_size) {
                        state = StateType::START;
                    } else {
                        file_write.write("EOF", strlen("EOF") * sizeof(char));
                    }
                    break;
                case StateType::ERROR:
                    throw std::runtime_error("error");
                    break;
                default:
                    state = StateType::START;
                    break;
            }
        }
    }

    file_write.close();
}


