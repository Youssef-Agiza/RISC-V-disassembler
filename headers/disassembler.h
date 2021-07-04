#pragma once
#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "RV32I.h"
#include "RV32C.h"
#include "RVDecoder.h"
#include "common.h"
#include "utils.h"

class Disassembler
{
private:
    RVDecoder *decoder_;
    unsigned int pc_;
    char *memory_;
    unsigned int fsize_;

public:
    Disassembler();
    ~Disassembler();
    void disassemble(char *file_name);
    void change_decoder(RVDecoder *decoder);

    void read_file(char *file_name);
};

#endif