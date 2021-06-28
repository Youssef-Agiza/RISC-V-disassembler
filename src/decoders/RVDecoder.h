#pragma once
#ifndef RVDECODER_H
#define RVDECODER_H
#include <iostream>
#include <string>

class RVDecoder
{
protected:
    unsigned int opcode, funct3, funct7;

    /*immediates*/
    unsigned int I_imm, B_imm, J_imm, U_imm, S_imm;

    //registers
    std::string rs1, rs2, rd;

    static unsigned int instSize; //

    virtual void getImmediates(unsigned int instW) = 0;
    virtual void getFuncts(unsigned int instW) = 0;
    virtual bool validateFunct3() = 0;
    virtual std::string getABIName(unsigned int reg) = 0;
    virtual void getRegs(unsigned int instW) = 0;
    virtual void printPrefix(unsigned int instPC, unsigned int instW) = 0;

public:
    RVDecoder();
    virtual ~RVDecoder() {}

    virtual void decodeWord(unsigned int instW, unsigned int pc) = 0;
};

#endif //RVDECODER_H
