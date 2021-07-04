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
#include "../headers/RV32I.h"
#include "../headers/disassembler.h"

using namespace std;

unsigned int pc = 0x0;

char memory[8 * 1024]; // only 8KB of memory located at address 0

void emit_error(const char *s)
{
	cout << s;
	exit(0);
}

int main(int argc, char *argv[])
{

	if (argc < 2)
		emit_error("use: rvcdiss <machine_code_file_name>\n");

	Disassembler disassembler;
	disassembler.disassemble(argv[1]);
}
