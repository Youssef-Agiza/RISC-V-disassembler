#pragma once
#ifndef RV32I_H
#define RV32I_H
#include "../headers/RVDecoder.h"

//better readability
enum OP_CODES
{
    R_TYPE = 0x33,
    I_TYPE = 0x13,
    B_TYPE = 0x63,
    S_TYPE = 0x23,
    LUI = 0x37,
    AUIPC = 0x17,
    JAL = 0x6F,
    JALR = 0x67,
    SYS_CALL = 0x73,
    LOAD = 0x3,
};

class RV32I : public RVDecoder
{
private:
    unsigned int funct7;

private:
    virtual void extract_immediates(unsigned int instW) override;

    virtual void extract_functs(unsigned int instW) override;

    virtual bool validate() override;

    virtual std::string get_ABI_name(unsigned int reg) override;

    virtual void extract_regs(unsigned int instW) override;
    virtual void print_prefix(unsigned int instA, unsigned int instW) override;

    virtual void print_instruction(int pc) override;

public:
    RV32I();
    virtual ~RV32I();

    virtual void decode_word(unsigned int instW, unsigned int pc) override;
};

#endif //RV32I_H
