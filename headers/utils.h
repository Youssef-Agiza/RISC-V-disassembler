#pragma once
#ifndef UTILS_H
#define UTILS_H
#include "common.h"

inline void emit_error(const char *message)
{
    std::cout << message;
    exit(0);
}

#endif //UTILS_H