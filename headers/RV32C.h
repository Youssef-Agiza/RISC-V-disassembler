#pragma once
#ifndef RV32C_H
#define RV32C_H
#include "RVDecoder.h"

class RV32C : public RVDecoder
{
protected:
    unsigned int funct2_, funct4_, funct6_, imm_;

protected:
    virtual void read_instW(const char *memory, unsigned int pc) override;

    virtual void extract_opcode() override;
    virtual void extract_immediates() override;
    virtual void extract_functs() override;
    virtual std::string get_ABI_name(unsigned int reg) override;

    //rd will not be used here because it position in instW changes by changing the instruction.
    //instead, rs2_ is used for the first register after opcode, and rs1_ used for the second register.
    //i.e. rs2_ --> (instW[2:6] or instW[2:4]) rs2_-->(instW[7:11] or instW[7:10])
    virtual void extract_regs() override;

    virtual bool validate() override;

    virtual void print_prefix(unsigned int instA) override;
    virtual void print_instruction(int pc) override;

public:
    RV32C();
    ~RV32C();

    virtual inline int get_decoder_type() { return 2; }
};

#endif // RV32C_H
