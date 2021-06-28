/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the RV32I "ADD", "SUB" and "ADDI" instructions only. 
	It prints "Unkown Instruction" for all other instructions!
	
	Usage example:
		$ rvcdiss t1.bin
	should print out:
		0x00000000	0x00100013	ADDI	x0, x0, 0x1
		0x00000004	0x00100093	ADDI	x1, x0, 0x1
		0x00000008	0x00100113	ADDI	x2, x0, 0x1
		0x0000000c	0x001001b3	ADD		x3, x0, x1
		0x00000010	0x00208233	ADD		x4, x1, x2
		0x00000014	0x004182b3	ADD		x5, x3, x4
		0x00000018	0x00100893	ADDI	x11, x0, 0x1
		0x0000001c	0x00028513	ADDI	xa, x5, 0x0
		0x00000020	0x00000073	Unkown Instruction 

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "stdlib.h"
#include "./instructionsMap.h"

using namespace std;

unsigned int pc = 0x0;

char memory[8 * 1024]; // only 8KB of memory located at address 0

void emitError(const char *s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW)
{
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

//for debugging
void printInfo(unsigned int opcode, unsigned int funct3, unsigned int funct7, unsigned int instWord)
{
	std::cout << std::ios::binary << "\n funct3: " << funct3
			  << "\n instruction word: " << (instWord >> 7)
			  << "\n funct7: " << funct7
			  << "\n opcode: " << opcode << "\n";
}

void getImmediates(unsigned int instW,
				   unsigned int &I_imm,
				   unsigned int &S_imm,
				   unsigned int &B_imm,
				   unsigned int &U_imm,
				   unsigned int &J_imm)
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
			(((instW >> 20) & 0x1) << 11) |	 //inst[20]
			(((instW >> 12) & 0xFF) << 12) | //inst [19:12]
			(((instW >> 31) ? 0xFFF : 0x0)); //— inst[31] —
}

void getFuncts(unsigned int instW, unsigned int &funct3, unsigned int &funct7)
{
	funct3 = (instW >> 12) & 0x00000007; // & with the following 3 bits
	funct7 = (instW >> 25) & 0x0000007F; // the last 7 bits
}

bool validateFunct3(unsigned int opcode, unsigned int funct3)
{
	if (funct3 > 7)
	{
		if (opcode == 0x33) //R type
			cout << "\tUnkown R Instruction \n";
		if (opcode == 0x13) //I type
			cout << "\tUnkown I Instruction \n";

		return false;
	}
	if (opcode == 0x63 && (funct3 == 2 || funct3 == 3 || funct3 > 7)) //B type
	{
		cout << "\tUnkown B Instruction \n";
		return false;
	}
	if (opcode == 0x23 && funct3 > 2)
	{
		cout << "\tUnkown S Instruction \n";
		return false;
	}
	return true;
}

std::string getRegABI(unsigned int reg)
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
		return "t" + to_string(reg - 5);
	if (reg > 27) //temporaries
		return "t" + to_string(reg - 25);

	if (reg > 9 && reg < 18) //a0-1
		return "a" + to_string(reg - 10);

	if (reg == 8 || reg == 9) //s0 & s1
		return "s" + to_string(reg - 8);

	return "s" + to_string(reg - 16); //s2-11
}

void getRegs(unsigned int instW, std::string &rd, std::string &rs1, std::string &rs2)
{
	unsigned int rd_num, rs1_num, rs2_num;
	rd_num = (instW >> 7) & 0x0000001F;	  // & with the following 5 bits
	rs1_num = (instW >> 15) & 0x0000001F; // the following 5 bits
	rs2_num = (instW >> 20) & 0x0000001F;

	rd = getRegABI(rd_num);
	rs1 = getRegABI(rs1_num);
	rs2 = getRegABI(rs2_num);
}

void instDecExec(unsigned int instWord)
{
	std::string rd, rs1, rs2;
	unsigned int funct3, funct7, opcode;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;

	unsigned int address;
	unsigned int instPC = pc - 4;

	/*parsing instWord*/
	opcode = instWord & 0x0000007F; //& with first 7 bits
	getFuncts(instWord, funct3, funct7);
	getRegs(instWord, rd, rs1, rs2);
	getImmediates(instWord, I_imm, S_imm, B_imm, U_imm, J_imm);

	printPrefix(instPC, instWord);

	if (!validateFunct3(opcode, funct3))
		return;

	switch (opcode)
	{
	case R_TYPE: // R Instructions
		cout << "\t" << R_instructions[(funct3 | funct7)] << "\t" << rd << ", " << rs1 << ", " << rs2 << "\n";
		break;
	case I_TYPE:
		cout << "\t" << I_instructions[(funct3 | funct7)] << "\t" << rd << ", " << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
		break;
	case B_TYPE: //B-Type
		cout << "\t" << B_instructions[funct3] << "\t" << rs1 << ", " << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
		break;
	case S_TYPE:
		cout << "\t" << S_instructions[funct3] << "\t" << rs2 << ", " << S_imm << "(" << rs1 << ")\n";
		break;
	case LUI:
		cout << "\tLUI\t" << rd << ", " << hex << "0x" << (int)U_imm << "\n";
		break;
	case AUIPC:
		cout << "\tAUIPC\t" << rd << ", " << hex << "0x" << (int)U_imm << "\n";
		break;
	case JAL:
		cout << "\tJAL\t" << rd << ", " << hex << "0x" << (int)J_imm << "\n";
		break;
	case JALR:
		cout << "\tJALR\t" << rd << ", " << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
		break;
	case SYS_CALL:
		if (!I_imm) //if last 12 bits == 0
			cout << "\tECALL\n";
		else
			cout << "\tEBREAK\n";
		break;
	case LOAD: //Load instructions (I-type) with different opcode
		cout << "\t" << load_instructions[funct3] << "\t" << rd << ", " << rs1 << "0x" << (int)I_imm << "\n";

	default:
		cout << "\tUnkown Instruction \n";
		break;
	}
	// printInfo(opcode, funct3, funct7, instWord);
}

int main(int argc, char *argv[])
{

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;

	if (argc < 2)
		emitError("use: rvcdiss <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();
		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char *)memory, fsize))
			emitError("Cannot read from input file\n");

		while (true)
		{
			instWord = (unsigned char)memory[pc] |
					   (((unsigned char)memory[pc + 1]) << 8) |
					   (((unsigned char)memory[pc + 2]) << 16) |
					   (((unsigned char)memory[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete simulator
			if (pc > 0x40)
				break; // stop when PC reached address 32
			instDecExec(instWord);
		}
	}
	else
		emitError("Cannot access input file\n");
}
