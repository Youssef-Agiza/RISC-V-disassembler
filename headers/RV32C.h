#pragma once
#ifndef RV32C_H
#define RV32C_H
#include "RVDecoder.h"

class RV32C : public RVDecoder
{
private:
    unsigned int funct2, funct4, funct6, imm;

private:
    virtual void extract_opcode(unsigned int instW) override;
    virtual void extract_immediates(unsigned int instW) override;
    virtual void extract_functs(unsigned int instW) override;

    virtual bool validate() override;

    virtual std::string get_ABI_name(unsigned int reg) override;

    //rd will not be used here because it position in instW changes by changing the instruction.
    //instead, rs2 is used for the first register after opcode, and rs1 used for the second register.
    //i.e. rs2 --> (instW[2:6] or instW[2:4]) rs2-->(instW[7:11] or instW[7:10])
    virtual void extract_regs(unsigned int instW) override;
    virtual void print_prefix(unsigned int instA, unsigned int instW) override;

    virtual void print_instruction(int pc) override;

public:
    RV32C();
    ~RV32C();
};

#endif // RV32C_H
