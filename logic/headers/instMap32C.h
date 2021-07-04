#pragma once
#ifndef INST_MAP_32C
#define INST_MAP_32C

#include "common.h"

//key --> funct2, value--> incstruction
instruction_map CA_instructions = {{0, "C.SUB"},
                                   {1, "C.XOR"},
                                   {2, "C.OR"},
                                   {3, "C.AND"}};

instruction_map CB_instructions = {{0, "C.SRLI"},
                                   {1, "C.SRAI"},
                                   {2, "C.ANDI"}};

#endif