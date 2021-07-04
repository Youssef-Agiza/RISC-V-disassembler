#pragma once
#ifndef RV_DECODER_H
#define RV_DECODER_H
#include "common.h"

/*
Abstract class interface. Responsible for decoding instruction word.
*/
class RVDecoder
{
protected:
    unsigned int opcode, funct3;

    /*immediates*/
    unsigned int I_imm, B_imm, J_imm, U_imm, S_imm;

    //registers
    std::string rs1, rs2, rd;

    //instruction size. overridden in children classes to change pc
    unsigned int inst_size;

    /*jump addresses are mapped to their label and stored here*/
    std::map<int, std::string> lbl_map;
    unsigned int lbl_cntr; //label counter

protected: //virtual functions
    //input: instruction word
    //output: I_imm, B_imm, J_imm, U_imm, S_imm are set
    virtual void extract_immediates(unsigned int instW) = 0;

    //input: instruction word
    //output: funct3 and funct7 values are set
    virtual void extract_functs(unsigned int instW) = 0;

    virtual bool validate() = 0;

    //input: instruction word
    //output: rd, rs1, and rs2 values are set
    virtual void extract_regs(unsigned int instW) = 0;

    //input: reg number
    //output: ABI name
    virtual std::string get_ABI_name(unsigned int reg) = 0;

    //prints instructionPC and instruction word.
    virtual void print_prefix(unsigned int instPC, unsigned int instW) = 0;

    //print instruction after opcode and functions are set.
    virtual void print_instruction(int pc) = 0;

    inline void generate_label(int address)
    {
        if (lbl_map.find(address) == lbl_map.end())
            lbl_map[address] = "L" + std::to_string(lbl_cntr++);
    }

public:
    RVDecoder() : lbl_cntr(1){};
    virtual ~RVDecoder() {}

    virtual void decode_word(unsigned int instW, unsigned int pc) = 0;

    //gets instruction size to change pc address accordingly.
    //inst_size will be overidden in chidlren classes
    inline unsigned int get_inst_size() { return this->inst_size; } //for pc address
};

#endif //RVDECODER_H
