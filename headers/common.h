#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <string>

//maps for RV32I instructions to get the instructions directly using funct3 and funct7

//key--> a combination of functs in instruction word.
//value-->instruction name
//e.g. in RV32I, key--> (funct3| funct7)
typedef std::map<unsigned int, std::string> instruction_map;

#endif