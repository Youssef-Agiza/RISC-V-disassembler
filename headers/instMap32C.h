#pragma once
#ifndef INST_MAP_32C
#define INST_MAP_32C

#include "common.h"

//key --> funct2, value--> incstruction
instruction_map CA_instructions = {{3, "C.AND"},
                                   {2, "C.OR"},
                                   {1, "C.XOR"},
                                   {0, "C.SUB"}};

#endif