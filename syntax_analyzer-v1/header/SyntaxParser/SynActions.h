#pragma once

#include "Productions.h"
#include "Global.h"

// action, the element in table
int reduce(int state, Token tok);

int go(int state, Token tok);

int error(int state, Token tok);

int accept(int state, Token tok);