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
    unsigned int opcode_, funct3_;

    //registers
    std::string rs1_, rs2_;

    //instruction size. overridden in children classes to change pc
    unsigned int instW_, inst_size_;

    /*jump addresses are mapped to their label and stored here*/
    std::map<int, std::string> lbl_map_;
    static unsigned int lbl_cntr_; //label counter

protected:
    /****************
       pure virtuals
     ****************/

    //output: opcode_ is set
    virtual void extract_opcode() = 0;

    //output: all immediates are set
    virtual void extract_immediates() = 0;

    //output: function values are set
    virtual void extract_functs() = 0;

    //precondition: opcode_, functions, and regs are populated
    //function: check opcode_, functions, and regs to ensure they have valid values
    //ouptut: returns true if valid, returns false otherwise.
    virtual bool validate() = 0;

    //output: rd_, rs1_, and rs2_ values are set
    virtual void extract_regs() = 0;

    //input: reg number
    //output: return ABI name
    virtual std::string get_ABI_name(unsigned int reg) = 0;

    //prints instructionPC and instruction word.
    virtual void print_prefix(unsigned int instPC) = 0;

    //print instruction after opcode_ and functions are set.
    virtual void print_instruction(int pc) = 0;

    //utilitiy: handles negative offset
    //input: offset of jump/branch and pc
    //output: lbl_addrs the pc should jump to
    virtual int get_label_address(unsigned int offset, int pc) = 0;

    /****************
       non-pure virtual
     ****************/

    //input: takes address to where jump/branch will go
    //output: if doesn't exit, adds this address to lbl_map_ . else, does nothing.
    inline void generate_label(int address)
    {
        if (lbl_map_.find(address) == lbl_map_.end())
            lbl_map_[address] = "L" + std::to_string(lbl_cntr_++);
    }

public:
    RVDecoder();
    virtual ~RVDecoder();

    //input: memory has the data in the binary file, pc--> program counter
    //output: reads instruction word and store it in instW_.
    virtual void read_instW(const char *memory, unsigned int pc) = 0;

    //input: instruction word, PC
    //output: calls private functions to decode instruction word.
    virtual void decode_word(unsigned int pc);

    //gets instruction size to change pc address accordingly.
    //inst_size_ will be overidden in chidlren classes
    inline unsigned int get_inst_size() { return this->inst_size_; } //for pc address

    //returns 1 if the current object is RV32I, and returns 2 if the calling object is RV32C
    virtual int get_decoder_type() = 0;
};

#endif //RVDECODER_H
