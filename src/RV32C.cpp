#include "../headers/RV32C.h"
#include "../headers/instMap32C.h"

RV32C::RV32C() { inst_size_ = 2; }
RV32C::~RV32C() {}

void RV32C::read_instW(const char *memory, unsigned int pc)
{
    this->instW_ = (unsigned char)memory[pc] |
                   (((unsigned char)memory[pc + 1]) << 8);
}

void RV32C::extract_opcode()
{
    opcode_ = instW_ & 0x3;
}

void RV32C::extract_immediates()
{
    if (opcode_ == 0) //SW & LW
        //inst[6] inst[12:10] inst[2] 0 0
        imm_ = ((((instW_ >> 5) & 0x1) << 2) |
                (((instW_ >> 10) & 0x7) << 3) |
                (((instW_ >> 6) & 0x1) << 6))
               << 2;

    else if (opcode_ == 1 && funct3_ == 1) //C.JAL
    {

        //imm[11|4|9:8|10|6|7|3:1|5]
        imm_ = ((((instW_ >> 12) & 0x1) << 11) | //imm[11]
                (((instW_ >> 11) & 0x1) << 4) |  //imm[4]
                (((instW_ >> 9) & 0x3) << 8) |   //imm[8:9]
                (((instW_ >> 8) & 0x1) << 10) |  //imm[10]
                (((instW_ >> 7) & 0x1 << 6)) |   //imm[6]
                (((instW_ >> 6) & 0x1 << 7)) |   //imm[7]
                (((instW_ >> 3) & 0x7) << 1) |   //imm[3:1]
                (((instW_ >> 2) & 0x1) << 5))    //imm[5]
               << 1;                             //scale by 2
    }
    else // ((opcode_ == 1 && funct != 1) || opcode_ == 2)
        imm_ = (((instW_ >> 12) & 0x1) << 5) | ((instW_ >> 2) & 0xF);
}

void RV32C::extract_functs()
{
    funct2_ = (instW_ >> 5) & 0x3;
    funct3_ = (instW_ >> 13);
    funct4_ = (instW_ >> 12);
    funct6_ = (instW_ >> 10);
}

void RV32C::extract_regs()
{
    unsigned int rs1_num, rs2_num;

    rs2_num = (instW_ >> 2) & 0x7;
    rs1_num = (instW_ >> 7) & 0x7;

    if (opcode_ == 0x2 || (opcode_ == 0x1 && funct3_ == 0)) //append two more bits to the left for CR, CI fromats
    {
        rs2_num = (rs2_num) | (((instW_ >> 5) & 0x3) << 3);
        rs1_num = (rs1_num) | (((instW_ >> 10) & 0x3) << 3);
    }
    rs1_ = get_ABI_name(rs1_num);
    rs2_ = get_ABI_name(rs2_num);
}
std::string RV32C::get_ABI_name(unsigned int reg)
{

    if ((opcode_ == 0x2 || (opcode_ == 0x1 && funct3_ == 0)) && (reg < 31)) //CR, CI formats use 32 bit registers
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

    else if (reg < 8) // CL, CS, CA, and CB formats.
    {
        std::string ABIs[] = {"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5"};
        return ABIs[reg];
    }
    else
    {
        std::cout << "\ninvalid register number: " << reg << "\n";
        return "";
    }
}

bool RV32C::validate()
{
    bool invalid_shift = (imm_ == 0) || (imm_ << 4 != 0);             //invalid= shamt==0 || shamt[5]==1
    bool invalid_addi = (rs1_ == "zero" || imm_ == 0);                //(rd==x0 || nzimm==0)
    bool invalid_lui = (rs1_ == "zero" || rs1_ == "ra" || imm_ == 0); //rd=={x0,x1} || nzimm==0

    //flags to return true or false
    bool unknown_inst = false;
    std::string invalid_inst;

    //SW, LW
    if (opcode_ == 0)
        unknown_inst = (funct3_ != 2 && funct3_ != 6);

    //C.ADDI, C.LUI, C.SRLI, C.SRAI
    if (opcode_ == 1)
    {
        unknown_inst = (funct3_ != 0 && funct3_ != 3 && funct3_ != 4);

        if (funct3_ == 0 && invalid_addi) //C.ADDI
            invalid_inst = "C.ADDI\n";

        if (funct3_ == 3 && invalid_lui) //C.LUI
            invalid_inst = "C.LUI\n";

        if (funct3_ == 4) //SRLI & SRAI
        {

            if (funct6_ & 0x3 == 0 && invalid_shift)
                invalid_inst = "C.SRLI\n";

            if (funct6_ & 0x3 == 1 && invalid_shift)
                invalid_inst = "C.SRAI\n";
        }
    }

    //SLLI,ADD,EBREAK,JALR
    if (opcode_ == 2)
    {
        unknown_inst = (funct3_ != 0 && funct3_ != 4);

        if (funct3_ == 0 && invalid_shift)
            invalid_inst = "C.SRLI\n";
    }

    if (unknown_inst)
    {
        std::cout << "Unknwon RVC instruction: opcode = 0x" << std::hex << opcode_ << ".\n";
        exit(1);
    }

    if (!invalid_inst.empty())
        std::cout << "Invalid instruction input for " << invalid_inst << ".\n";

    return (invalid_inst.empty() && !unknown_inst);
}

void RV32C::print_prefix(unsigned int instA)
{
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(4) << instW_;
}
void RV32C::print_instruction(int pc)
{
    if (opcode_ == 0)
    {
        switch (funct3_)
        {
        case 2:
            std::cout << "\tC.LW\t" << rs2_ << ", " << imm_ << "(" << rs1_ << ")\n";
            break;
        case 6:
            std::cout << "\tC.SW\t" << rs2_ << ", " << imm_ << "(" << rs1_ << ")\n";
            break;
        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
    else if (opcode_ == 1)
    {
        switch (funct3_)
        {
        case 0:
            std::cout << "\tC.ADDI\t" << rs1_ << ", 0x" << imm_ << "\n";
            break;
        case 1:
            std::cout << "\tC.JAL\t0x" << imm_ << "\n";
            break;
        case 3:
            std::cout << "\tC.LUI\t" << rs1_ << ", 0x" << imm_ << "\n";
            break;
        case 4:
            //funct6_ &0x3 --> instW[10:11]
            if (funct6_ & 0x3 == 3) //C.AND, C.OR, C.XOR, C.SUB
                std::cout << "\t" << CA_instructions[funct2_] << "\t" << rs1_ << ", " << rs2_ << "\n";
            else //C.SRLI, C.SRAI, C.ANDI
                std::cout << "\t" << CB_instructions[funct6_ & 0x3] << "\t" << rs1_ << ", 0x" << imm_ << "\n";
            break;

        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
    else if (opcode_ == 2)
    {
        switch (funct3_)
        {
        case 0: //SLLI
            std::cout << "\tC.SLLI\t" << rs1_ << ", " << imm_ << "\n";
            break;
        case 4: // C. ADD, EBREAK, JALR
            if (rs1_ != "zero" && rs2_ != "zero")
                std::cout << "\tC.ADD\t" << rs1_ << ", " << rs2_ << "\n";

            else if (rs2_ == "zero" && rs1_ == "zero")
                std::cout << "\tC.EBREAK\t\n";

            else if (rs2_ == "zero" && rs1_ != "zero")
                std::cout << "\tC.JALR\t " << rs1_ << "\n";

            else
                std::cout << "Unknown instruction\n";

            break;
        default:
            std::cout << "Unknown instruction\n";
            break;
        }
    }
}
