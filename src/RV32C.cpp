#include "../headers/RV32C.h"
#include "../headers/instMap32C.h"

void RV32C::extractImmediates(unsigned int instW)
{
    if (opcode == 0)
        imm = (((instW >> 5) & 0x1) << 2) |
              (((instW >> 10) & 0x7) << 3) |
              (((instW >> 6) & 0x1) << 6);

    else if (opcode == 1 && funct3 == 1) //C.JAL
    {

        //imm[11|4|9:8|10|6|7|3:1|5]
        imm = ((((instW >> 12) & 0x1) << 11) | //imm[11]
               (((instW >> 11) & 0x1) << 4) |  //imm[4]
               (((instW >> 9) & 0x3) << 8) |   //imm[8:9]
               (((instW >> 8) & 0x1) << 10) |  //imm[10]
               (((instW >> 7) & 0x1 << 6)) |   //imm[6]
               (((instW >> 6) & 0x1 << 7)) |   //imm[7]
               (((instW >> 3) & 0x7) << 1) |   //imm[3:1]
               (((instW >> 2) & 0x1) << 5))    //imm[5]
              << 1;                            //shft left by 1 to put 0 in as the first bit
    }
    else
        imm = (((instW >> 12) & 0x1) << 5) | ((instW >> 2) & 0xF);
}
bool RV32C::validateFuncts() { return true; }

void RV32C::extractFuncts(unsigned int instW)
{
    funct2 = (instW >> 5) & 0x3;
    funct3 = (instW >> 13);
    funct4 = (instW >> 12);
    funct6 = (instW >> 10);
}

void RV32C::extractRegs(unsigned int instW)
{
    unsigned int rs1, rs2;

    rs2 = (instW >> 2) & 0x7;
    rs1 = (instW >> 7) & 0x7;

    if (opcode == 0x2) //append two more bits to the left
    {
        rs2 = (rs2) | (((instW >> 5) & 0x3) << 3);
        rs1 = (rs1) | (((instW >> 10) & 0x3) << 3);
    }
}
void RV32C::printPrefix(unsigned int instA, unsigned int instW)
{
    unsigned int instHW = instW >> 4;
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(4) << instHW;
}
void RV32C::printInstruction()
{
    if (opcode == 0)
    {
        switch (funct3)
        {
        case 2:
            std::cout << "\tLW\t" << rs2 << ", " << imm << "(" << rs1 << ")\n";
            break;
        case 6:
            std::cout << "\tSW\t" << rs2 << ", " << imm << "(" << rs1 << ")\n";
            break;
        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
    else if (opcode == 1)
    {
        switch (funct3)
        {
        case 0:
            std::cout << "\tADDI\t" << rs1 << ", " << rs1 << ", " << imm << "\n";
            break;
        case 1:
            std::cout << "\tJAL\tra, " << imm << "\n"; //getABIName of x1
            break;
        case 3:
            std::cout << "\tLUI\t" << rs1 << ", " << imm << "\n";
            break;
        case 4:
            switch (funct6 & 0x3) //instW[10:11]
            {
            case 0:
                std::cout << "\tSRLI\t" << rs1 << ", " << rs1 << ", " << imm << "\n";
                break;
            case 1:
                std::cout << "\tSRAI\t" << rs1 << ", " << rs1 << ", " << imm << "\n";
                break;
            case 2:
                std::cout << "\tANDI\t" << rs1 << ", " << rs1 << ", " << imm << "\n";
                break;
            case 3:
                std::cout << "\t" << CA_instructions[funct2] << "\t" << rs1 << ", " << rs2 << ", " << imm << "\n";
                break;
            default:
                std::cout << "Unknown instruction\n";
                break;
            }
            break;

        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
    else if (opcode == 2)
    {
        switch (funct3)
        {
        case 0: //SLLI
            std::cout << "\tSLLI\t" << rs1 << ", " << rs1 << ", " << imm << "\n";
            break;
        case 4: // C. ADD, EBREAK, JALR
            if (rs1 != "zero" && rs2 != "zero")
                std::cout << "\tADD\t" << rs2 << ", " << rs1 << ", " << rs1;
            if (rs2 == "zero" && rs1 == "zero")
                std::cout << "\tEBREAK\t";
            if (rs2 == "zero" && rs1 != "zero")
                std::cout << "\tJALR\tra, 0(" << rs1 << ")";
            else
                std::cout << "Unknown instruction";

            std::cout << "\n";
            break;
        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
}
std::string RV32C::getABIName(unsigned int reg)
{
    if (reg > 9)
    {
        std::cout << "invalid register number";
        return "";
    }

    std::string ABIs[] = {"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5"};
    return ABIs[reg];
}

RV32C::RV32C() { instSize = 2; }
RV32C::~RV32C() {}

void RV32C::decodeWord(unsigned int instW, unsigned int pc)
{
    extractFuncts(instW);
    extractRegs(instW);
    printPrefix(instW, pc);
}