#pragma once
#ifndef RV_DECODER_H
#define RV_DECODER_H
#include "common.h"
#include "utils.h"

/*
 * Abstract class interface.
 * Provide interfacte for decoding int instruction word.
 * Children classes will implement the decoding details according the ISA.
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

protected:
    /****************
       pure virtuals
     ****************/

    //input: instruction word
    //output: opcode is set
    virtual void extract_opcode(unsigned int instW) = 0;

    //input: instruction word
    //output: I_imm, B_imm, J_imm, U_imm, S_imm are set
    virtual void extract_immediates(unsigned int instW) = 0;

    //input: instruction word
    //output: funct3 and funct7 values are set
    virtual void extract_functs(unsigned int instW) = 0;

    //precondition: opcode, functions, and regs are populated
    //function: check opcode, functions, and regs to ensure they have valid values
    //ouptut: returns true if valid, returns false otherwise.
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

    /****************
       non-pure virtual
     ****************/

    //input: takes address to where jump/branch will go
    //output: if doesn't exit, adds this address to lbl_map . else, does nothing.
    inline void generate_label(int address)
    {
        if (lbl_map.find(address) == lbl_map.end())
            lbl_map[address] = "L" + std::to_string(lbl_cntr++);
    }

public:
    RVDecoder();
    virtual ~RVDecoder();

    //input: instruction word, PC
    //output: calls private functions to decode instruction word.
    virtual void decode_word(unsigned int instW, unsigned int pc);

    //gets instruction size to change pc address accordingly.
    //inst_size will be overidden in chidlren classes
    inline unsigned int get_inst_size() { return this->inst_size; } //for pc address
};

#endif //RVDECODER_H
