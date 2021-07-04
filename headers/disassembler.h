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
    unsigned int fsize_;

    static void emit_error(const char *message);

public:
    Disassembler();
    ~Disassembler();
    void disassemble(char *file_name);
    void change_decoder(RVDecoder *decoder);

    void read_file(char *file_name);
};

#endif