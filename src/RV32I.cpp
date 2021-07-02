#include "RV32I.h"
#include "../headers/instMap32I.h"

RV32I::RV32I()
{
    instSize = 4;
}
RV32I::~RV32I() {}

void RV32I::printPrefix(unsigned int instA, unsigned int instW)
{
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void RV32I::extractImmediates(unsigned int instW)
{
    // — inst[31] — inst[30:25] inst[24:21] inst[20]
    I_imm = ((instW >> 20) & 0x7FF) | (((instW >> 31) ? 0xFFFFF800 : 0x0));

    // — inst[31] — inst[7] inst[30:25] inst[11:8]
    B_imm = (((instW >> 8) & 0xF) << 1) |
            (((instW >> 25) & 0x3F) << 5) |
            (((instW >> 7) & 0x1) << 10) |
            ((instW >> 31) ? 0xFFFFF800 : 0x0);

    S_imm = (((instW >> 25) & 0x3F) << 5) | (((instW >> 7) & 0x1F));

    U_imm = ((instW >> 12) & 0xFFFFF);

    J_imm = (((instW >> 21) & 0x3FF) << 1) | //inst[30:25]  inst[25:21] 0
            (((instW >> 20) & 0x1) << 11) |  //inst[20]
            (((instW >> 12) & 0xFF) << 12) | //inst [19:12]
            (((instW >> 31) ? 0xFFF : 0x0)); //— inst[31] —
}

void RV32I::extractFuncts(unsigned int instW)
{
    funct3 = (instW >> 12) & 0x00000007;
    funct7 = (instW >> 25) & 0x0000007F;
}

bool RV32I::validateFuncts()
{

    if (funct3 > 7)
    {
        if (opcode == 0x33) //R type
            std::cout << "\tUnkown R Instruction \n";
        if (opcode == 0x13) //I type
            std::cout << "\tUnkown I Instruction \n";

        return false;
    }
    if (opcode == 0x63 && (funct3 == 2 || funct3 == 3 || funct3 > 7)) //B type
    {
        std::cout << "\tUnkown B Instruction \n";
        return false;
    }
    if (opcode == 0x23 && funct3 > 2)
    {
        std::cout << "\tUnkown S Instruction \n";
        return false;
    }
    return true;
}

std::string RV32I::getABIName(unsigned int reg)
{
    if (reg > 31)
    {
        std::cout << "invalid register number";
        return "";
    }

    std::string initial_ABIs[] = {"zero", "ra", "sp", "gp", "tp"};
    if (reg < 5)
        return initial_ABIs[reg];

    if ((reg > 4 && reg < 8)) //temporaries
        return "t" + std::to_string(reg - 5);
    if (reg > 27) //temporaries
        return "t" + std::to_string(reg - 25);

    if (reg > 9 && reg < 18) //a0-7
        return "a" + std::to_string(reg - 10);

    if (reg == 8 || reg == 9) //s0 & s1
        return "s" + std::to_string(reg - 8);

    return "s" + std::to_string(reg - 16); //s2-11
}

void RV32I::extractRegs(unsigned int instW)
{
    unsigned int rd_num, rs1_num, rs2_num;
    rd_num = (instW >> 7) & 0x0000001F;   // & with the following 5 bits
    rs1_num = (instW >> 15) & 0x0000001F; // the following 5 bits
    rs2_num = (instW >> 20) & 0x0000001F;

    rd = getABIName(rd_num);
    rs1 = getABIName(rs1_num);
    rs2 = getABIName(rs2_num);
}

void RV32I::printInstruction()
{

    switch (opcode)
    {
    case R_TYPE: // R Instructions
        std::cout << "\t" << R_instructions[(funct3 | funct7)] << "\t" << rd << ", " << rs1 << ", " << rs2 << "\n";
        break;
    case I_TYPE:
        if (funct3 == 5)
            funct3 = (funct3 | funct7);
        std::cout << "\t" << I_instructions[funct3] << "\t" << rd << ", " << rs1 << ", " << std::hex << "0x" << (int)I_imm << "\n";
        break;
    case B_TYPE: //B-Type
        std::cout << "\t" << B_instructions[funct3] << "\t" << rs1 << ", " << rs2 << ", " << std::hex << "0x" << (int)B_imm << "\n";
        break;
    case S_TYPE:
        std::cout << "\t" << S_instructions[funct3] << "\t" << rs2 << ", " << S_imm << "(" << rs1 << ")\n";
        break;
    case LUI:
        std::cout << "\tLUI\t" << rd << ", " << std::hex << "0x" << (int)U_imm << "\n";
        break;
    case AUIPC:
        std::cout << "\tAUIPC\t" << rd << ", " << std::hex << "0x" << (int)U_imm << "\n";
        break;
    case JAL:
        std::cout << "\tJAL\t" << rd << ", " << std::hex << "0x" << (int)J_imm << "\n";
        break;
    case JALR:
        std::cout << "\tJALR\t" << rd << ", " << rs1 << ", " << std::hex << "0x" << (int)I_imm << "\n";
        break;
    case SYS_CALL:
        if (!I_imm) //if last 12 bits == 0
            std::cout << "\tECALL\n";
        else
            std::cout << "\tEBREAK\n";
        break;
    case LOAD: //Load instructions (I-type) with different opcode
        std::cout << "\t" << load_instructions[funct3] << "\t" << rd << ", 0x" << std::hex << (int)I_imm << "(" << rs1 << ")\n";
        break;
    default:
        std::cout << "\tUnkown Instruction \n";
        break;
    }
}

void RV32I::decodeWord(unsigned int instW, unsigned int pc)
{
    unsigned int instPC = pc - 4;

    /*parsing instWord*/
    opcode = instW & 0x0000007F; //& with first 7 bits
    extractFuncts(instW);
    extractRegs(instW);
    extractImmediates(instW);

    printPrefix(instPC, instW);

    if (!validateFuncts())
        return;

    printInstruction();
    // printInfo(opcode, funct3, funct7, instW);
}