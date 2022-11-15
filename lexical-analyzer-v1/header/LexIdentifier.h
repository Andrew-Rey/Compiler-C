#pragma once

#include "LexicalDefine.h"

#include <string>
#include <fstream>
#include <vector>

void dfaIdentifier(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(read_p, std::ios::beg);
    char c{};
    if (state == StateType::START) {
        state = StateType::ENTER_ID;
        while (read_p < file_size &&
               state != StateType::END_ID && state != StateType::ERROR && state != StateType::END) {
            file_read.read(&c, sizeof(char));
            read_p = file_read.tellg();
            switch (state) {
                case StateType::ENTER_ID:
                    if (std::isblank(c) || c == '\r' || c == '\n') {
                        state = StateType::ENTER_ID;
                    } else if (std::isalpha(c) || c == '_') {
                        buf.push_back(c);
                        state = StateType::IN_ID;
                    } else {
                        file_read.seekg((long long) -sizeof(char), std::ios::cur);
                        read_p = file_read.tellg();
                        state = StateType::END;
                    }
                    break;
                case StateType::IN_ID:
                    if (std::isalpha(c) || std::isdigit(c) || c == '_') {
                        buf.push_back(c);
                        state = StateType::IN_ID;
                    } else if (std::isblank(c) || isSeparator(c)) {
                        if (isSeparator(c)) {
                            file_read.seekg((long long) -sizeof(char), std::ios::cur);
                            read_p = file_read.tellg();
                        }
                        state = StateType::END_ID;
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
