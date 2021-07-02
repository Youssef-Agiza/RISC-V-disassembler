#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>

//maps for RV32I instructions to get the instructions directly using funct3 and funct7

//key--> (funct3|funct7). value-->instruction name
typedef std::map<unsigned int, std::string> instruction_map;

#endif