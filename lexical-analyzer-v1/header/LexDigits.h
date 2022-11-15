#pragma once

#include "LexicalDefine.h"

#include <fstream>
#include <string>
#include <vector>

void dfaDigits(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(read_p, std::ios::beg);
    char c{};
    if (state == StateType::START) {
        state = StateType::ENTER_DIGITS;
        while (read_p < file_size &&
               state != StateType::END_DIGITS && state != StateType::ERROR && state != StateType::END) {
            file_read.read(&c, sizeof(char));
            read_p = file_read.tellg();
            switch (state) {
                case StateType::ENTER_DIGITS:
                    if (std::isblank(c) || c == '\r' || c == '\n') {
                        state = StateType::ENTER_DIGITS;
                    } else if (std::isdigit(c)) {
                        buf.push_back(c);
                        state = StateType::IN_DIGITS;
                        if ((int) c == 0) {
                            state = StateType::END_DIGITS;
                        }
                    } else {
                        file_read.seekg((long long) -sizeof(char), std::ios::cur);
                        read_p = file_read.tellg();
                        state = StateType::END;
                    }
                    break;
                case StateType::IN_DIGITS:
                    if (std::isdigit(c)) {
                        buf.push_back(c);
                        state = StateType::IN_DIGITS;
                    } else if (std::isblank(c) || isSeparator(c)) {
                        if (isSeparator(c)) {
                            file_read.seekg((long long) -sizeof(char), std::ios::cur);
                            read_p = file_read.tellg();
                        }
                        state = StateType::END_DIGITS;
                    } else {
                        // ERROR
                        state = StateType::ERROR;
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
