#include "../headers/RV32I.h"
#include "../headers/instMap32I.h"

RV32I::RV32I()
{
    inst_size_ = 4;
}
RV32I::~RV32I() {}

void RV32I::read_instW(const char *memory, unsigned int pc)
{
    this->instW_ = (unsigned char)memory[pc] |
                   (((unsigned char)memory[pc + 1]) << 8) |
                   (((unsigned char)memory[pc + 2]) << 16) |
                   (((unsigned char)memory[pc + 3]) << 24);
}

void RV32I::extract_opcode()
{
    opcode_ = instW_ & 0x0000007F; //& with first 7 bits
}

void RV32I::print_prefix(unsigned int instA)
{
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW_;
}

void RV32I::extract_immediates()
{
    // — inst[31] — inst[30:25] inst[24:21] inst[20]
    I_imm_ = ((instW_ >> 20) & 0x7FF) | (((instW_ >> 31) ? 0xFFFFF800 : 0x0));

    // — inst[31] — inst[7] inst[30:25] inst[11:8]
    B_imm_ = (((instW_ >> 8) & 0xF) << 1) |
             (((instW_ >> 25) & 0x3F) << 5) |
             (((instW_ >> 7) & 0x1) << 10) |
             ((instW_ >> 31) ? 0xFFFFF800 : 0x0);

    S_imm_ = (((instW_ >> 25) & 0x3F) << 5) | (((instW_ >> 7) & 0x1F));

    U_imm = ((instW_ >> 12) & 0xFFFFF);

    J_imm_ = (((instW_ >> 21) & 0x3FF) << 1) |      //inst[30:25]  inst[25:21] 0
             (((instW_ >> 20) & 0x1) << 11) |       //inst[20]
             (((instW_ >> 12) & 0xFF) << 12) |      //inst [19:12]
             (((instW_ >> 31) ? 0xFFF00000 : 0x0)); //— inst[31] —
}

void RV32I::extract_functs()
{
    funct3_ = (instW_ >> 12) & 0x00000007;
    funct7_ = (instW_ >> 25) & 0x0000007F;
}

bool RV32I::validate()
{

    if (funct3_ > 7)
    {
        if (opcode_ == 0x33) //R type
            std::cout << "\tUnkown R Instruction \n";
        if (opcode_ == 0x13) //I type
            std::cout << "\tUnkown I Instruction \n";

        return false;
    }
    if (opcode_ == 0x63 && (funct3_ == 2 || funct3_ == 3 || funct3_ > 7)) //B type
    {
        std::cout << "\tUnkown B Instruction \n";
        return false;
    }
    if (opcode_ == 0x23 && funct3_ > 2)
    {
        std::cout << "\tUnkown S Instruction \n";
        return false;
    }
    return true;
}

std::string RV32I::get_ABI_name(unsigned int reg)
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

void RV32I::extract_regs()
{
    unsigned int rd_num, rs1_num, rs2_num;
    rd_num = (instW_ >> 7) & 0x0000001F;   // & with the following 5 bits
    rs1_num = (instW_ >> 15) & 0x0000001F; // the following 5 bits
    rs2_num = (instW_ >> 20) & 0x0000001F;

    this->rd_ = get_ABI_name(rd_num);
    this->rs1_ = get_ABI_name(rs1_num);
    this->rs2_ = get_ABI_name(rs2_num);
}

//utilitiy
//input: offset of jump/branch and pc
//output: lbl_addrs the pc should jump to
inline int get_label_adress(unsigned int offset, int pc)
{
    //handles the case where lbl_addrs is negative
    return (offset >> 31) ? pc - ((offset ^ 0xFFFFFFFF) + 1) : pc + offset;
}

void RV32I::print_instruction(int pc)
{
    int lbl_addrs; //used for Jump/Branch labels
    switch (opcode_)
    {
    case R_TYPE: // R Instructions
        std::cout << "\t" << R_instructions[(funct3_ | funct7_)] << "\t" << rd_ << ", " << rs1_ << ", " << rs2_ << "\n";
        break;
    case I_TYPE:
        if (funct3_ == 5)
            funct3_ = (funct3_ | funct7_);
        std::cout << "\t" << I_instructions[funct3_] << "\t" << rd_ << ", " << rs1_ << ", " << std::hex << "0x" << (int)I_imm_ << "\n";
        break;
    case B_TYPE: //B-Type
        lbl_addrs = get_label_adress(B_imm_, pc);
        generate_label(lbl_addrs);
        std::cout << "\t" << B_instructions[funct3_] << "\t" << rs1_ << ", " << rs2_ << ", " << std::hex << "0x" << (int)B_imm_ << "<" << lbl_map_[lbl_addrs] << ">\n";
        break;
    case S_TYPE:
        std::cout << "\t" << S_instructions[funct3_] << "\t" << rs2_ << ", " << S_imm_ << "(" << rs1_ << ")\n";
        break;
    case LUI:
        std::cout << "\tLUI\t" << rd_ << ", " << std::hex << "0x" << (int)U_imm << "\n";
        break;
    case AUIPC:
        std::cout << "\tAUIPC\t" << rd_ << ", " << std::hex << "0x" << (int)U_imm << "\n";
        break;
    case JAL:
        lbl_addrs = get_label_adress(J_imm_, pc);
        generate_label(lbl_addrs);
        std::cout << "\tJAL\t" << rd_ << ", " << std::hex << "0x" << (int)J_imm_ << "<" << lbl_map_[lbl_addrs] << ">\n";
        break;
    case JALR:
        lbl_addrs = get_label_adress(I_imm_, pc);
        generate_label(lbl_addrs);
        std::cout << "\tJALR\t" << rd_ << ", " << rs1_ << ", " << std::hex << "0x" << (int)I_imm_ << "<" << lbl_map_[lbl_addrs] << ">\n";
        break;
    case SYS_CALL:
        if (!I_imm_) //if last 12 bits == 0
            std::cout << "\tECALL\n";
        else
            std::cout << "\tEBREAK\n";
        break;
    case LOAD: //Load instructions (I-type) with different opcode
        std::cout << "\t" << load_instructions[funct3_] << "\t" << rd_ << ", 0x" << std::hex << (int)I_imm_ << "(" << rs1_ << ")\n";
        break;
    default:
        std::cout << "\tUnkown Instruction \n";
        break;
    }
}
