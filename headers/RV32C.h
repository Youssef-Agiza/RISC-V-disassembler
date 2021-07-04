#pragma once
#ifndef RV32C_H
#define RV32C_H
#include "RVDecoder.h"

class RV32C : public RVDecoder
{
private:
    unsigned int funct2, funct4, funct6, imm;

private:
    virtual void extract_immediates(unsigned int instW) override;

    virtual void extract_functs(unsigned int instW) override;
    virtual bool validate() override;

    virtual std::string get_ABI_name(unsigned int reg) override;

    virtual void extract_regs(unsigned int instW) override;
    virtual void print_prefix(unsigned int instA, unsigned int instW) override;

    virtual void print_instruction(int pc) override;

public:
    RV32C();
    ~RV32C();
    virtual void decode_word(unsigned int instW, unsigned int pc) override;
};

#endif // RV32C_H
