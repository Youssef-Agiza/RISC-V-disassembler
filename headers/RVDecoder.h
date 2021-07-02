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
    unsigned int instSize;

    /*jump addresses are mapped to their label and stored here*/
    std::map<int, std::string> labels_map;
    unsigned int label_counter;

protected: //virtual functions
    //input: instruction word
    //output: I_imm, B_imm, J_imm, U_imm, S_imm are set
    virtual void extractImmediates(unsigned int instW) = 0;

    //input: instruction word
    //output: funct3 and funct7 values are set
    virtual void extractFuncts(unsigned int instW) = 0;

    virtual bool validateFuncts() = 0;

    //input: instruction word
    //output: rd, rs1, and rs2 values are set
    virtual void extractRegs(unsigned int instW) = 0;

    //input: reg number
    //output: ABI name
    virtual std::string getABIName(unsigned int reg) = 0;

    //prints instructionPC and instruction word.
    virtual void printPrefix(unsigned int instPC, unsigned int instW) = 0;

    //print instruction after opcode and functions are set.
    virtual void printInstruction(int pc) = 0;

    virtual void generateLabel(int address) { labels_map[address] = "L" + std::to_string(label_counter++); }

public:
    RVDecoder() : label_counter(1){};
    virtual ~RVDecoder() {}

    virtual void decodeWord(unsigned int instW, unsigned int pc) = 0;

    //gets instruction size to change pc address accordingly.
    //instSize will be overidden in chidlren classes
    unsigned int getInstSize() { return this->instSize; } //for pc address
};

#endif //RVDECODER_H
