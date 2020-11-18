#include "architecture.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "utils.hpp"
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;

#define BIOS_FILENAME "gbc_bios.bin"

Architecture* Architecture::singleton = NULL;

Architecture::Architecture()
{
	ifstream bios(BIOS_FILENAME, std::ios::in | std::ios::binary);
	if (!bios)
	{
		error("Can't open BIOS file");
		return;
	}

	data addr = 0;
	char buffer;
	while (!bios.eof())
	{
		bios.read(&buffer, 1);
		ram[addr] = (byte)buffer;

		addr++;
	}
	bios.close();

	debug("BIOS loaded!");

	//Put default values in ram
	/*const byte nintendo_graphic[] = {
		0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E 
	};*/

	//Wrong to write it in memory like this, it should already be in the ROM
	//memcpy(ram + 0x0104, nintendo_graphic, sizeof(nintendo_graphic));
}

void Architecture::dump_ram()
{
	ofstream dump("dump.bin", std::ios::binary);

	if (!dump)
	{
		error("Could not create dump.bin file");
		return;
	}

	uint8_t buffer;
	for (int i = 0; i < RAM_SIZE; i++)
	{
		buffer = ram[i].to_ulong() & 0xff;
		dump.write((char*)(&buffer), sizeof(uint8_t));
	}

	dump.close();
}

void Architecture::print_registers()
{
	cout << "\n################# REGISTERS #################\n";
	cout << "PC:\t" << PC.to_string() << " | " << to_hex(PC.to_ulong()) << endl;
	cout << "SP:\t" << SP.to_string() << " | " << to_hex(SP.to_ulong()) << endl;
	cout << "A:\t" << A.to_string() << "\tF:\t" << F.to_string() << " | " << to_hex(((word)AF).to_ulong()) << endl;
	cout << "B:\t" << B.to_string() << "\tC:\t" << C.to_string() << " | " << to_hex(((word)BC).to_ulong()) << endl;
	cout << "D:\t" << D.to_string() << "\tE:\t" << E.to_string() << " | " << to_hex(((word)DE).to_ulong()) << endl;
	cout << "H:\t" << H.to_string() << "\tL:\t" << L.to_string() << " | " << to_hex(((word)HL).to_ulong()) << endl;
	cout << "Flags: Z:" << Zflag() << " N:" << Nflag() << " H:" << Hflag() << " C:" << Cflag() << endl;
	cout << "---------------------------------------------\n";
}

void Architecture::print_stack(unsigned int rows)
{
	cout << "\n################### STACK ###################\n";
	data addr = SP.to_ulong();

	printf("%#.4x | ", addr);
	cout << ram[addr].to_string() << "  <- SP" << endl;
	for (unsigned int i = 1; i < rows && addr+i <= 0xffff; i++)
	{
		printf("%#.4x | ", addr+i);
		cout << ram[addr + i].to_string() << endl;
	}
	cout << "---------------------------------------------\n";
}

void Architecture::print_ram(data addr, unsigned int rows)
{
	cout << "\n###################  RAM  ###################\n";
	for (unsigned int i = 0; i < rows && addr + i <= 0xffff; i++)
	{
		printf("%#.4x | ", addr + i);
		printf("%#.2x\n", ram[addr + i].to_ulong());
	}
	cout << "---------------------------------------------\n";
}

void Architecture::print_instruction(data address, Instruction i)
{
	cout << "\n################ INSTRUCTION ################\n";
	printf("%#.4x | ", address);

	cout << cmd_codes[i.cmd] << " ";

	if (i.arg1.address)
		cout << "(";

	switch (i.arg1.type)
	{
	case IMM:
	case W_IMM:
		cout << to_hex(i.arg1.value.immediate);
		break;
	case REG: {
		if (i.arg1.value.reg == &A)
			cout << "A";
		else if (i.arg1.value.reg == &F)
			cout << "F";
		else if (i.arg1.value.reg == &B)
			cout << "B";
		else if (i.arg1.value.reg == &C)
			cout << "C";
		else if (i.arg1.value.reg == &D)
			cout << "D";
		else if (i.arg1.value.reg == &E)
			cout << "E";
		else if (i.arg1.value.reg == &H)
			cout << "H";
		else if (i.arg1.value.reg == &L)
			cout << "L";
		break;
	}
	case W_REG: {
		if (i.arg1.value.w_reg == &SP)
			cout << "SP";
		else if (i.arg1.value.w_reg == &PC)
			cout << "PC";
		break;
	}
	case C_REG: {
		if (i.arg1.value.c_reg == &AF)
			cout << "AF";
		else if (i.arg1.value.c_reg == &BC)
			cout << "BC";
		else if (i.arg1.value.c_reg == &DE)
			cout << "DE";
		else if (i.arg1.value.c_reg == &HL)
			cout << "HL";
		break;
	}
	case NONE:
		cout << "-";
	}

	if (i.arg1.address)
		cout << ")";

	cout << ", ";

	if (i.arg2.address)
		cout << "(";

	switch (i.arg2.type)
	{
	case IMM:
	case W_IMM:
		cout << to_hex(i.arg2.value.immediate);
		break;
	case REG: {
		if (i.arg2.value.reg == &A)
			cout << "A";
		else if (i.arg2.value.reg == &F)
			cout << "F";
		else if (i.arg2.value.reg == &B)
			cout << "B";
		else if (i.arg2.value.reg == &C)
			cout << "C";
		else if (i.arg2.value.reg == &D)
			cout << "D";
		else if (i.arg2.value.reg == &E)
			cout << "E";
		else if (i.arg2.value.reg == &H)
			cout << "H";
		else if (i.arg2.value.reg == &L)
			cout << "L";
		break;
	}
	case W_REG: {
		if (i.arg2.value.w_reg == &SP)
			cout << "SP";
		else if (i.arg2.value.w_reg == &PC)
			cout << "PC";
		break;
	}
	case C_REG: {
		if (i.arg2.value.c_reg == &AF)
			cout << "AF";
		else if (i.arg2.value.c_reg == &BC)
			cout << "BC";
		else if (i.arg2.value.c_reg == &DE)
			cout << "DE";
		else if (i.arg2.value.c_reg == &HL)
			cout << "HL";
		break;
	}
	case NONE:
		cout << "-";
	}

	if (i.arg2.address)
		cout << ")";

	cout << endl;
}
