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
    virtual void extractImmediates(unsigned int instW) override;

    virtual void extractFuncts(unsigned int instW) override;

    virtual bool validateFuncts() override;

    virtual std::string getABIName(unsigned int reg) override;

    virtual void extractRegs(unsigned int instW) override;
    virtual void printPrefix(unsigned int instA, unsigned int instW) override;

    virtual void printInstruction() override;

public:
    RV32I();
    virtual ~RV32I();

    virtual void decodeWord(unsigned int instW, unsigned int pc) override;
};

#endif //RV32I_H