#pragma once
#ifndef RV32C_H
#define RV32C_H
#include "RVDecoder.h"

class RV32C : public RVDecoder
{
private:
    unsigned int funct2, funct4, funct6, imm;

private:
    virtual void extractImmediates(unsigned int instW) override;

    virtual void extractFuncts(unsigned int instW) override;
    virtual bool validateFuncts() override;

    virtual std::string getABIName(unsigned int reg) override;

    virtual void extractRegs(unsigned int instW) override;
    virtual void printPrefix(unsigned int instA, unsigned int instW) override;

    virtual void printInstruction() override;

public:
    RV32C();
    ~RV32C();
    virtual void decodeWord(unsigned int instW, unsigned int pc) override;
};

#endif // RV32C_H
