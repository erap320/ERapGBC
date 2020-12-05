#include "architecture.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
using std::ofstream;
using std::cout;
using std::endl;

#define BIOS_FILENAME "gbc_bios.bin"

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
}

void Architecture::loadROM(string romFileName)
{
	if (cart.loadROM(romFileName))
	{
		for (data addr = 0; addr < ROM_BANK_SIZE; addr++)
		{
			//Put ROM Bank0 in ram, starting address 0x0000
			ram[addr] = cart.romBanks[0][addr];
			//Put ROM Bank1 (set as current in architecture.hpp) in ram, starting address 0x4000
			ram[0x4000 + addr] = cart.romBanks[currentROMBank][addr];
		}

		for (data addr = 0; addr < RAM_BANK_SIZE; addr++)
		{
			//Put RAM Bank0 (set as current in architecture.hpp) in ram, starting address 0xA000
			ram[0xA000 + addr] = cart.ramBanks[currentRAMBank][addr];
		}

		debug("ROM loaded!");
	}
	else {
		error("Error while loading ROM file " + romFileName);
	}
}

void Architecture::swapWorkingBank(unsigned short selected)
{
	if (selected > 7)
		error("Wrong Working RAM bank chosen, no swapping was performed: "+selected);

	data base = 0xD000;
	
	//Decrement to align with the array index
	//If it's 0 it already selects bank 1, so we can leave it
	if(selected > 0)
		selected--;

	for (data addr = 0; addr <= 0xFFF; addr++)
	{
		//Store the current state of the bank
		workingBanks[currentWorkingBank][addr] = ram[base + addr];
		//Replace the values in ram with the ones of the selected bank
		ram[base + addr] = workingBanks[selected][addr];
	}

	currentWorkingBank = selected;
}

void Architecture::swapVideoBank(unsigned short selected)
{
	if (selected > 1)
		error("Wrong Video RAM bank chosen, no swapping was performed: "+selected);

	data base = 0x8000;

	//Decrement to align with the array index
	//If it's 0 it already selects bank 1, so we can leave it
	if (selected > 0)
		selected--;

	for (data addr = 0; addr <= 0x1FFF; addr++)
	{
		//Store the current state of the bank
		videoBanks[currentVideoBank][addr] = ram[base + addr];
		//Replace the values in ram with the ones of the selected bank
		ram[base + addr] = videoBanks[selected][addr];
	}

	currentVideoBank = selected;
	videoBanksDirty = false;
}

void Architecture::updateVideoBank()
{
	if (videoBanksDirty)
	{
		data base = 0x8000;
		for (data addr = 0; addr <= 0x1FFF; addr++)
		{
			//Store the current state of the bank
			videoBanks[currentVideoBank][addr] = ram[base + addr];
		}
		videoBanksDirty = false;
	}
}

//Function to perform bank switching
void Architecture::swapCartROMBank(unsigned int selected)
{
	if(selected > CART_ROM_BANKS-1)
		error("Wrong Cartridge ROM bank chosen, no swapping was performed: " + selected);

	//Can't swap with bank0, bank1 will be selected
	if (selected == 0)
		selected = 1;

	for (data addr = 0; addr < ROM_BANK_SIZE; addr++)
	{
		//No need to copy data back as ROM can't be modified
		//Put ROM Bank in ram, starting address 0x4000
		ram[0x4000 + addr] = cart.romBanks[selected][addr];
	}

	currentROMBank = selected;
	debug("Swapped cart ROM bank " + to_hex(selected), PC);
}

//Function to perform bank switching
void Architecture::swapCartRAMBank(unsigned short selected)
{
	if (selected > CART_RAM_BANKS - 1)
		error("Wrong Cartridge RAM bank chosen, no swapping was performed: " + selected);

	data internalAddr;
	for (data addr = 0; addr < RAM_BANK_SIZE; addr++)
	{
		internalAddr = 0xA000 + addr;
		//Copy the content of the current bank to the cartridge
		cart.ramBanks[currentRAMBank][addr] = ram[internalAddr];
		//Put RAM Bank in ram, starting address 0xA000
		ram[internalAddr] = cart.ramBanks[selected][addr];
	}

	currentRAMBank = selected;
	debug("Swapped cart RAM bank "+to_hex(selected), PC);
}

void Architecture::runDMA(byte a)
{
	data address = (word(a.to_ulong()) << BYTE_SIZE).to_ulong();
	for (data i=0; i <= 0x9F; i++)
		ram[0xFE00 + i] = ram[address + i];

	debug("DMA from " + to_hex(address) + " to 0xfe00", PC);
}

void Architecture::runVDMA(data src, data dst)
{
	data size = ((ram[HDMA5] & (byte)0x7F).to_ulong() + 1) * 16;
	for (data i = 0; i <= size; i++)
		ram[dst + i] = ram[src + i];

	//Finished
	ram[HDMA5] = 0xFF;

	debug("VDMA from " + to_hex(src) + " to " + to_hex(dst), PC);
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
	out_mutex.lock();
	cout << "\n################# REGISTERS #################\n";
	cout << "PC:\t" << PC.to_string() << "  | " << to_hex(PC.to_ulong()) << endl;
	cout << "SP:\t" << SP.to_string() << "  | " << to_hex(SP.to_ulong()) << endl;
	cout << "A:\t" << A.to_string() << "  F:\t" << F.to_string() << "  | " << to_hex(((word)AF).to_ulong()) << endl;
	cout << "B:\t" << B.to_string() << "  C:\t" << C.to_string() << "  | " << to_hex(((word)BC).to_ulong()) << endl;
	cout << "D:\t" << D.to_string() << "  E:\t" << E.to_string() << "  | " << to_hex(((word)DE).to_ulong()) << endl;
	cout << "H:\t" << H.to_string() << "  L:\t" << L.to_string() << "  | " << to_hex(((word)HL).to_ulong()) << endl;
	cout << "Flags: Z:" << Zflag() << " N:" << Nflag() << " H:" << Hflag() << " C:" << Cflag() << " IME:" << IME << endl;
	cout << "---------------------------------------------\n";
	out_mutex.unlock();
}

void Architecture::print_stack(unsigned int rows)
{
	out_mutex.lock();
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
	out_mutex.unlock();
}

void Architecture::print_ram(data addr, unsigned int rows)
{
	out_mutex.lock();
	cout << "\n###################  RAM  ###################\n";
	for (unsigned int i = 0; i < rows && addr + i <= 0xffff; i++)
	{
		printf("0x%.4x  | ", addr + i);
		printf("0x%.2x \n", ram[addr + i].to_ulong());
	}
	cout << "---------------------------------------------\n";
	out_mutex.unlock();
}

//Gives a string representation of the argument if it is an address
string addrName(data addr, bool byte, bool indirect)
{
	if (byte && indirect)
	{
		byte = false;
		addr += 0xFF00;
	}

	switch (addr)
	{
	default:
		return to_hex(addr, byte);
		break;
	case 0xFF00:
		return "P1";
		break;
	case 0xFF01:
		return "SB";
		break;
	case 0xFF02:
		return "SC";
		break;
	case 0xFF04:
		return "DIV";
		break;
	case 0xFF05:
		return "TIMA";
		break;
	case 0xFF06:
		return "TMA";
		break;
	case 0xFF07:
		return "TAC";
		break;
	case 0xFF4D:
		return "KEY1";
		break;
	case 0xFF56:
		return "RP";
		break;
	case 0xFF4F:
		return "VBK";
		break;
	case 0xFF70:
		return "SVBK";
		break;
	case 0xFF0F:
		return "IF";
		break;
	case 0xFFFF:
		return "IE";
		break;
	case 0xFF40:
		return "LCDC";
		break;
	case 0xFF41:
		return "STAT";
		break;
	case 0xFF42:
		return "SCY";
		break;
	case 0xFF43:
		return "SCX";
		break;
	case 0xFF44:
		return "LY";
		break;
	case 0xFF45:
		return "LYC";
		break;
	case 0xFF46:
		return "DMA";
		break;
	case 0xFF47:
		return "BGP";
		break;
	case 0xFF48:
		return "OBP0";
		break;
	case 0xFF49:
		return "OBP1";
		break;
	case 0xFF4A:
		return "WY";
		break;
	case 0xFF4B:
		return "WX";
		break;
	case 0xFF51:
		return "HDMA1";
		break;
	case 0xFF52:
		return "HDMA2";
		break;
	case 0xFF53:
		return "HDMA3";
		break;
	case 0xFF54:
		return "HDMA4";
		break;
	case 0xFF55:
		return "HDMA5";
		break;
	case 0xFF68:
		return "BCPS";
		break;
	case 0xFF69:
		return "BCPD";
		break;
	case 0xFF6A:
		return "OCPS";
		break;
	case 0xFF6B:
		return "OCPD";
		break;
	case 0xFE00:
		return "OAM";
		break;
	}
}

void Architecture::print_instructions(data address, unsigned int rows)
{
	Instruction instr = Instruction{ERR};

	data currentPC = PC.to_ulong();

	out_mutex.lock();
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
			cout << addrName(instr.arg1.value.immediate, true, instr.arg1.address);
			break;
		case W_IMM:
			cout << addrName(instr.arg1.value.immediate, false, instr.arg1.address);
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
			cout << addrName(instr.arg2.value.immediate, true, instr.arg2.address);
			break;
		case W_IMM:
			cout << addrName(instr.arg2.value.immediate, false, instr.arg2.address);
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
			cout << "   <- PC";
		
		cout << endl;

		//Increase address
		address += instr.length();
	}
	out_mutex.unlock();
}
