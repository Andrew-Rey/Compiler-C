#pragma once

#include "LexicalDefine.h"

#include <vector>
#include <fstream>

void dfaSeparator(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
    long long file_size = file_read.tellg();
    file_read.seekg(read_p, std::ios::beg);
    char c{};
    if (state == StateType::START) {
        state = StateType::ENTER_SEP;
        while (read_p < file_size &&
               state != StateType::END_SEP && state != StateType::ERROR && state != StateType::END) {
            file_read.read(&c, sizeof(char));
            read_p = file_read.tellg();
            switch (state) {
                case StateType::ENTER_SEP:
                    if (std::isblank(c)) {
                        state = StateType::ENTER_SEP;
                    } else if (isSeparator(c)) {
                        buf.push_back(c);
                        state = StateType::END_SEP;
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
