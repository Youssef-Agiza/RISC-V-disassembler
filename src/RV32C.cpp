#include "../headers/RV32C.h"
#include "../headers/instMap32C.h"

void RV32C::extract_opcode(unsigned int instW)
{
    opcode = instW & 0x3;
}

void RV32C::extract_immediates(unsigned int instW)
{
    if (opcode == 0) //SW & LW
        //inst[6] inst[12:10] inst[2] 0 0
        imm = ((((instW >> 5) & 0x1) << 2) |
               (((instW >> 10) & 0x7) << 3) |
               (((instW >> 6) & 0x1) << 6))
              << 2;

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
              << 1;                            //scale by 2
    }
    else // ((opcode == 1 && funct != 1) || opcode == 2)
        imm = (((instW >> 12) & 0x1) << 5) | ((instW >> 2) & 0xF);
}
bool RV32C::validate()
{

    /*
SW
LW


ADDI
JAL
LUI
SRLI
SRAI
ANDI
AND
OR
XOR
SUB


SLLI
ADD
EBREAK
JALR
*/
    return true;
}

void RV32C::extract_functs(unsigned int instW)
{
    funct2 = (instW >> 5) & 0x3;
    funct3 = (instW >> 13);
    funct4 = (instW >> 12);
    funct6 = (instW >> 10);
}

void RV32C::extract_regs(unsigned int instW)
{
    unsigned int rs1_num, rs2_num;

    rs2_num = (instW >> 2) & 0x7;
    rs1_num = (instW >> 7) & 0x7;

    if (opcode == 0x2) //append two more bits to the left
    {
        rs2_num = (rs2_num) | (((instW >> 5) & 0x3) << 3);
        rs1_num = (rs1_num) | (((instW >> 10) & 0x3) << 3);
    }
    rs1 = get_ABI_name(rs1_num);
    rs2 = get_ABI_name(rs2_num);
}
void RV32C::print_prefix(unsigned int instA, unsigned int instW)
{
    unsigned int instHW = instW >> 4;
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(4) << instHW;
}
void RV32C::print_instruction(int pc)
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
            std::cout << "\tJAL\tra, " << imm << "\n"; //get_ABI_name of x1
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
std::string RV32C::get_ABI_name(unsigned int reg)
{

    if (opcode && (reg < 31)) //CR, CI formats
    {

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

    else if (opcode && (reg < 8)) // CL, CS, CA, and CB formats.
    {
        std::string ABIs[] = {"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5"};
        return ABIs[reg];
    }
    else
    {
        std::cout << "\ninvalid register number\n";
        return "";
    }
}

RV32C::RV32C() { inst_size = 2; }
RV32C::~RV32C() {}
