#pragma once
#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "RV32I.h"
#include "RV32C.h"
#include "RVDecoder.h"
#include "common.h"
#include "utils.h"
#include <typeinfo>

class Disassembler
{
private:
    RVDecoder *decoder_;
    unsigned int pc_;
    char *memory_;
    unsigned int fsize_;

    //changes docder_ to 32bit or 16bit according to opcode
    void check_inst_type();

    //sets decoder_ to decoder
    void change_decoder(RVDecoder *decoder);

    //reads binary file and stores it in memory_
    void read_file(char *file_name);

public:
    Disassembler();
    ~Disassembler();

    void disassemble(char *file_name);
};

#endif