#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cctype>
#include <fstream>
#include <cstring>

typedef enum {
    START, INNUM, INID, INEQ, INNEQ, INLE, INRE, INLS, INCMT, ENDCMT, DONE
} StateType;

void Save(char c, char *buf);

void getNext();