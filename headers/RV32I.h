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
    unsigned int funct7_;

    /*immediates*/
    unsigned int I_imm_, B_imm_, J_imm_, U_imm, S_imm_;

    /*destination register*/
    std::string rd_;

private:
    virtual void read_instW(const char *memory, unsigned int pc) override;

    virtual void extract_opcode() override;
    virtual void extract_immediates() override;
    virtual void extract_functs() override;
    virtual std::string get_ABI_name(unsigned int reg) override;
    virtual void extract_regs() override;

    virtual bool validate() override;

    virtual void print_prefix(unsigned int instA) override;
    virtual void print_instruction(int pc) override;

public:
    RV32I();
    virtual ~RV32I();
    virtual inline int get_decoder_type() { return 1; }
};

#endif //RV32I_H
