#pragma once
#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
#include <fstream>
#include <iostream>
#include <iomanip>

#include "RV32I.h"
#include "RVDecoder.h"

class Disassembler
{
private:
    RVDecoder *decoder_;
    unsigned int pc_;
    char *memory_;

    void emitError(const char *message) const;

public:
    Disassembler();
    ~Disassembler();
    void disassemble(char *file_name);
    void changeDecoder(RVDecoder *decoder);

    void readFile(char *file_name);
};

#endif