#pragma once

#include <map>
#include <string>
typedef std::map<int, std::string> instruction_map;

instruction_map R_instructions = {{0, "ADD"},
                                  {32, "SUB"},
                                  {1, "SLL"},
                                  {2, "SLT"},
                                  {3, "SLTU"},
                                  {4, "XOR"},
                                  {5, "SRL"},
                                  {37, "SRA"},
                                  {6, "OR"},
                                  {7, "AND"}};

instruction_map I_instructions = {{0, "ADDI"},
                                  {1, "SLLI"},
                                  {2, "SLTI"},
                                  {3, "SLTIU"},
                                  {4, "XORI"},
                                  {5, "SRLI"},
                                  {37, "SRAI"},
                                  {6, "ORI"},
                                  {7, "ANDI"}};

instruction_map B_instructions = {{0, "BEQ"},
                                  {1, "BNE"},
                                  {4, "BLT"},
                                  {5, "BGT"},
                                  {6, "BLTU"},
                                  {7, "BGTU"}};

instruction_map S_instructions = {{0, "SB"},
                                  {1, "SH"},
                                  {2, "SW"}};
