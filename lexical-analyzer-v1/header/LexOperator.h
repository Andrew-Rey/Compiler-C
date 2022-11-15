#pragma once

#include "LexicalDefine.h"

#include <vector>
#include <fstream>

void dfaOperator(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(read_p, std::ios::beg);
    char c{};
    if (state == StateType::START) {
        state = StateType::ENTER_OP;
        while (read_p < file_size &&
               state != StateType::END_OP && state != StateType::ERROR && state != StateType::END) {
            file_read.read(&c, sizeof(char));
            read_p = file_read.tellg();
            switch (state) {
                case StateType::ENTER_OP:
                    if (std::isblank(c)) {
                        state = StateType::ENTER_OP;
                    } else if (isOperator(c)) {
                        buf.push_back(c);
                        state = StateType::END_OP;
                    } else {
                        file_read.seekg((long long) -sizeof(char), std::ios::cur);
                        read_p = file_read.tellg();
                        state = StateType::END;
                    }
                    break;
                default:
                    file_read.seekg((long long) -sizeof(char), std::ios::cur);
                    read_p = file_read.tellg();
                    state = StateType::END;
            }
        }
    }
    file_read.close();
}
