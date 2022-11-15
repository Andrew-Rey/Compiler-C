#pragma once

#include "LexicalDefine.h"

#include <vector>
#include <fstream>
#include <string>
//
//void dfaExpr(std::string &file_in, StateType &state, std::vector<char> &buf, long long &read_p) {
//    std::ifstream file_read{file_in, std::ios::in | std::ios::binary | std::ios::ate};
//    long long file_size = file_read.tellg();
//    file_read.seekg(read_p, std::ios::beg);
//    char c{};
//    if (state == StateType::START) {
//        state = StateType::ENTER_EXPR;
//        while (read_p < file_size &&
//               state != StateType::END_EXPR && state != StateType::ERROR && state != StateType::END) {
//            file_read.read(&c, sizeof(char));
//            read_p = file_read.tellg();
//            switch (state) {
//                case StateType::ENTER_EXPR:
//                    if (std::isblank(c) || c == '\r' || c == '\n') {
//                        state = StateType::ENTER_EXPR;
//                    } else if () {
//                    }
//                    break;
//            }
//        }
//    }
//}