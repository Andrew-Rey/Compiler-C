#pragma once

#include "LexicalDefine.h"

#include <vector>
#include <fstream>
#include <string>

void dfaComment(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(read_p, std::ios::beg);
    char c{};
    if (state == StateType::START) {
        state = StateType::ENTER_CMT;
        while (read_p < file_size &&
               state != StateType::END_CMT && state != StateType::ERROR && state != StateType::END) {
            file_read.read(&c, sizeof(char));
            read_p = file_read.tellg();
            switch (state) {
                case StateType::ENTER_CMT:
                    if (std::isblank(c) || c == '\r' || c == '\n') {
                        state = StateType::ENTER_CMT;
                    } else if (c == '/') {
                        // overlook the comment, no need to input into buf
                        state = StateType::CMT_S1;
                    } else {
                        file_read.seekg((long long) -sizeof(char), std::ios::cur);
                        read_p = file_read.tellg();
                        state = StateType::END;
                    }
                    break;
                case StateType::CMT_S1:
                    if (c == '*') {
                        state = StateType::IN_CMT_S2;
                    } else {
                        file_read.seekg((long long) -sizeof(char), std::ios::cur);
                        read_p = file_read.tellg();
                        state = StateType::END;
                    }
                    break;
                case StateType::IN_CMT_S2:
                    if (c == '*') {
                        state = StateType::IN_CMT_S3;
                    } else {
                        state = StateType::IN_CMT_S2;
                    }
                    break;
                case StateType::IN_CMT_S3:
                    if (c == '/') {
                        state = StateType::END_CMT;
                    } else if (c == '*') {
                        state = StateType::IN_CMT_S3;
                    } else {
                        state = StateType::IN_CMT_S2;
                    }
                    break;
                default:
                    file_read.seekg((long long) -sizeof(char), std::ios::cur);
                    read_p = file_read.tellg();
                    state = StateType::END;
            }
        }
    }
}
