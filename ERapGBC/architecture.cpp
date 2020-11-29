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
#define ROM_FILENAME "PokemonPicross.gbc"

Architecture* Architecture::singleton = NULL;

Architecture::Architecture()
{
	//Skip bios for now
	/*
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
	*/

	//Values after BIOS finished its checks
	PC = 0x100;
	AF = 0x11B0;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;
	SP = 0xFFFE;

	ram[P1] = 0x00;
	ram[SC] = 0x00;
	ram[TIMA] = 0x00;
	ram[0xFF06] = 0x00;
	ram[TAC] = 0x00;
	ram[0xFF10] = 0x80;
	ram[0xFF11] = 0xBF;
	ram[0xFF12] = 0xF3;
	ram[0xFF14] = 0xBF;
	ram[0xFF16] = 0x3F;
	ram[0xFF17] = 0x00;
	ram[0xFF19] = 0xBF;
	ram[0xFF1A] = 0x7F;
	ram[0xFF1B] = 0xFF;
	ram[0xFF1C] = 0x9F;
	ram[0xFF1E] = 0xBF;
	ram[0xFF20] = 0xFF;
	ram[0xFF21] = 0x00;
	ram[0xFF22] = 0x00;
	ram[0xFF23] = 0xBF;
	ram[0xFF24] = 0x77;
	ram[0xFF25] = 0xF3;
	ram[0xFF26] = 0xF1;
	ram[LCDC] = 0x91;
	ram[SCY] = 0x00;
	ram[SCX] = 0x00;
	ram[LYC] = 0x00;
	ram[0xFF47] = 0xFC;
	ram[0xFF48] = 0xFF;
	ram[0xFF49] = 0xFF;
	ram[WY] = 0x00;
	ram[WX] = 0x00;
	ram[IE] = 0x00;

	//Load rom
	ifstream rom(ROM_FILENAME, std::ios::in | std::ios::binary);
	if (!rom)
	{
		error("Can't open ROM file");
		return;
	}

	data addr;
	char buffer;
	for(addr=0; addr<0x4000 && !rom.eof(); addr++)
	{
		rom.read(&buffer, 1);
		ram[addr] = (byte)buffer;
	}
	rom.close();

	debug("ROM loaded!");
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
	cout << "PC:\t" << PC.to_string() << "  | " << to_hex(PC.to_ulong()) << endl;
	cout << "SP:\t" << SP.to_string() << "  | " << to_hex(SP.to_ulong()) << endl;
	cout << "A:\t" << A.to_string() << "  F:\t" << F.to_string() << "  | " << to_hex(((word)AF).to_ulong()) << endl;
	cout << "B:\t" << B.to_string() << "  C:\t" << C.to_string() << "  | " << to_hex(((word)BC).to_ulong()) << endl;
	cout << "D:\t" << D.to_string() << "  E:\t" << E.to_string() << "  | " << to_hex(((word)DE).to_ulong()) << endl;
	cout << "H:\t" << H.to_string() << "  L:\t" << L.to_string() << "  | " << to_hex(((word)HL).to_ulong()) << endl;
	cout << "Flags: Z:" << Zflag() << " N:" << Nflag() << " H:" << Hflag() << " C:" << Cflag() << endl;
	cout << "---------------------------------------------\n";
}

void Architecture::print_stack(unsigned int rows)
{
	cout << "\n################### STACK ###################\n";
	data addr = SP.to_ulong();

	printf("0x%.4x  | ", addr);
	cout << ram[addr].to_string() << "  <- SP" << endl;
	for (unsigned int i = 1; i < rows && addr+i <= 0xffff; i++)
	{
		printf("0x%.4x  | ", addr+i);
		cout << ram[addr + i].to_string() << endl;
	}
	cout << "---------------------------------------------\n";
}

void Architecture::print_ram(data addr, unsigned int rows)
{
	cout << "\n###################  RAM  ###################\n";
	for (unsigned int i = 0; i < rows && addr + i <= 0xffff; i++)
	{
		printf("0x%.4x  | ", addr + i);
		printf("0x%.2x \n", ram[addr + i].to_ulong());
	}
	cout << "---------------------------------------------\n";
}

void Architecture::print_instructions(data address, unsigned int rows)
{
	Instruction instr = Instruction{ERR};

	data currentPC = PC.to_ulong();

	cout << "\n################ INSTRUCTIONS ###############\n";
	for (unsigned int i=0; i < rows && address <= 0xffff; i++)
	{
		instr = disasm(address);

		printf("0x%.4x | ", address);

		cout << cmd_codes[instr.cmd] << " ";

		if (instr.arg1.address)
			cout << "(";

		switch (instr.arg1.type)
		{
		case IMM:
			cout << to_hex(instr.arg1.value.immediate, true);
			break;
		case W_IMM:
			cout << to_hex(instr.arg1.value.immediate);
			break;
		case REG: {
			if (instr.arg1.value.reg == &A)
				cout << "A";
			else if (instr.arg1.value.reg == &F)
				cout << "F";
			else if (instr.arg1.value.reg == &B)
				cout << "B";
			else if (instr.arg1.value.reg == &C)
				cout << "C";
			else if (instr.arg1.value.reg == &D)
				cout << "D";
			else if (instr.arg1.value.reg == &E)
				cout << "E";
			else if (instr.arg1.value.reg == &H)
				cout << "H";
			else if (instr.arg1.value.reg == &L)
				cout << "L";
			break;
		}
		case W_REG: {
			if (instr.arg1.value.w_reg == &SP)
				cout << "SP";
			else if (instr.arg1.value.w_reg == &PC)
				cout << "PC";
			break;
		}
		case C_REG: {
			if (instr.arg1.value.c_reg == &AF)
				cout << "AF";
			else if (instr.arg1.value.c_reg == &BC)
				cout << "BC";
			else if (instr.arg1.value.c_reg == &DE)
				cout << "DE";
			else if (instr.arg1.value.c_reg == &HL)
				cout << "HL";
			break;
		}
		case NONE:
			cout << "-";
		}

		if (instr.arg1.address)
			cout << ")";

		cout << ", ";

		if (instr.arg2.address)
			cout << "(";

		switch (instr.arg2.type)
		{
		case IMM:
		case W_IMM:
			cout << to_hex(instr.arg2.value.immediate);
			break;
		case REG: {
			if (instr.arg2.value.reg == &A)
				cout << "A";
			else if (instr.arg2.value.reg == &F)
				cout << "F";
			else if (instr.arg2.value.reg == &B)
				cout << "B";
			else if (instr.arg2.value.reg == &C)
				cout << "C";
			else if (instr.arg2.value.reg == &D)
				cout << "D";
			else if (instr.arg2.value.reg == &E)
				cout << "E";
			else if (instr.arg2.value.reg == &H)
				cout << "H";
			else if (instr.arg2.value.reg == &L)
				cout << "L";
			break;
		}
		case W_REG: {
			if (instr.arg2.value.w_reg == &SP)
				cout << "SP";
			else if (instr.arg2.value.w_reg == &PC)
				cout << "PC";
			break;
		}
		case C_REG: {
			if (instr.arg2.value.c_reg == &AF)
				cout << "AF";
			else if (instr.arg2.value.c_reg == &BC)
				cout << "BC";
			else if (instr.arg2.value.c_reg == &DE)
				cout << "DE";
			else if (instr.arg2.value.c_reg == &HL)
				cout << "HL";
			break;
		}
		case NONE:
			cout << "-";
		}

		if (instr.arg2.address)
			cout << ")";

		if (address == currentPC)
			cout << "\t<- PC";
		
		cout << endl;

		//Increase address
		address += instr.length();
	}
}
