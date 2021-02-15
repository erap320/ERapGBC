#pragma once
#include "exceptions.hpp"
#include <iostream>
#include <ostream>
#include "types.hpp"
#include "registers.hpp"
#include "utils.hpp"
#include "cartridge.hpp"
#include <chrono>
using std::string;
using std::to_string;

#define RAM_SIZE 65536
#define W_BANK_SIZE 0x1000
#define V_BANK_SIZE 0x2000

//Forward definition
class Architecture;

//Word registers composed of 2 byte registers
//The class allows to read and write directly from/to the two base registers
class CombinedRegister {
public:
	byte* high;
	byte* low;

	//Define assignment of a word
	CombinedRegister& operator=(const word& w) // copy assignment
	{
		*high = (w >> BYTE_SIZE).to_ulong();
		*low = (w & (word)0xff).to_ulong();
		return *this;
	}

	//Define cast to word for when the register has to be read
	operator bitset<WORD_SIZE>() const { return word(high->to_string() + low->to_string()); }

	//Define cast to string for printing
	operator string() const { return ((word)(*this)).to_string(); }

	//Define interaction with streams for printing
	friend std::ostream& operator<<(std::ostream& output, const CombinedRegister& cr) {
		return output << (string)cr;
	}
};

//Type of argument passed to an instruction
enum ArgType {
	NONE,			/* Empty argument, not used by this instruction */

	// Byte types
	IMM,			/* Immediate */
	REG,			/* Register */

	// Word types
	W_REG,			/* Word register */
	C_REG,			/* Combined register */
	W_IMM			/* Immediate word */
};

const string arg_codes[] = { "NONE", "IMM", "REG", "W_REG", "C_REG", "W_IMM" };

//Reference to the actual argument, depends on the type
union ArgValue {
	byte* reg;
	word* w_reg;
	CombinedRegister* c_reg;
	data immediate;
};

enum Addressing {
	NO_ADDR=0,
	ADDR=1
};

//Argument passed to an instruction
struct Argument {
	ArgType type;
	ArgValue value;
	Addressing address;

	Argument(byte& b, Addressing a = NO_ADDR, ArgType t = REG)
	{
		type = t;
		value.reg = &b;
		address = a;
	}

	Argument(word& w, Addressing a = NO_ADDR, ArgType t = W_REG)
	{
		type = t;
		value.w_reg = &w;
		address = a;
	}

	Argument(CombinedRegister& cr, Addressing a = NO_ADDR, ArgType t = C_REG)
	{
		type = t;
		value.c_reg = &cr;
		address = a;
	}

	Argument(data d, ArgType t, Addressing a = NO_ADDR)
	{
		type = t;
		value.immediate = d;
		address = a;
	}

	Argument(ArgType t, data d=0, Addressing a = NO_ADDR)
	{
		type = t;
		value.immediate = d;
		address = a;
	}

	Argument()
	{
		type = NONE;
		value.immediate = 0;
		address = NO_ADDR;
	}

	//Read 8 bits from argument
	byte r8();

	//Write 8 bits to argument
	void w8(byte val);

	//Read 16 bits from argument
	word r16();

	//Write 16 bits to argument
	void w16(word val);

	//Copy the value of one argument to the other,
	//respecting the size of the data it refers to
	void copy(Argument& other)
	{
		//Size of the two arguments should always be the same
		if ( (this->type >= W_REG && !this->address) || (other.type >= W_REG && !other.address) ) //All 16 bit types come after W_REG
			this->w16(other.r16());
		else
			this->w8(other.r8());
	}

	//Define cast to string for printing
	operator string() const {
		string result = "[";

		switch (type)
		{
		case IMM:
		case W_IMM: {
			if (address)
				result += "(";

			result += "Value: ";
			result += to_hex(value.immediate, type==IMM);
			
			if (address)
				result += ")";

			result += " ";
			break;
		}
		}

		result += "Type: "+ arg_codes[type] +"; Addr: " + (address ? "Y" : "N" ) +"]";
		return result;
	}
};

enum Command {
	LD, LDD, LDI, LDH, LDHL,
	PUSH, POP,
	ADD, ADC, SUB, SBC,
	AND, OR, XOR, CP, CPL,
	INC, DEC,
	SWAP, DAA,
	CCF, SCF,
	NOP, HALT, STOP,
	DI, EI,
	RLCA, RLA, RRCA, RRA, RLC, RL, RRC, RR,
	SLA, SRA, SRL,
	BIT, SET, RES,
	JP, JPNZ, JPZ, JPNC, JPC,
	JR, JRNZ, JRZ, JRNC, JRC,
	CALL, CALLNZ, CALLZ, CALLNC, CALLC,
	RET, RETNZ, RETZ, RETNC, RETC, RETI,
	RST,
	ERR
};

#pragma optimize( "", off )
//The first element of this array is always ruined by compiler optimization
//I have no idea why
const string cmd_codes[] =
{	"LD", "LDD", "LDI", "LDH", "LDHL", "PUSH", "POP", "ADD", "ADC",
	"SUB", "SBC", "AND", "OR", "XOR", "CP", "CPL", "INC", "DEC", "SWAP",
	"DAA", "CCF", "SCF", "NOP", "HALT", "STOP", "DI", "EI", "RLCA", "RLA",
	"RRCA", "RRA", "RLC", "RL", "RRC", "RR", "SLA", "SRA", "SRL", "BIT", "SET",
	"RES", "JP", "JP(NZ)", "JP(Z)", "JP(NC)", "JP(C)", "JR", "JR(NZ)", "JR(Z)", "JR(NC)", "JR(C)",
	"CALL", "CALL(NZ)", "CALL(Z)", "CALL(NC)", "CALL(C)", "RET", "RET(NZ)", "RET(Z)", "RET(NC)",
	"RET(C)", "RETI", "RST", "ERR"
};
#pragma optimize( "", on )

class Instruction {
public:
	Command cmd;
	Argument arg1;
	Argument arg2;
	unsigned short delay;

	Instruction(Command c, unsigned short del, Argument a1 = Argument{ NONE }, Argument a2 = Argument{ NONE })
	{
		cmd = c;
		arg1 = a1;
		arg2 = a2;
		delay = del;
	}

	//Length of the binary instruction in bytes
	//Defined in disasm.cpp
	unsigned short length();

	//Define cast to string for printing
	operator string() const { return "{Command: " + cmd_codes[cmd] + "; Arg1: " + (string)arg1 + "; Arg2: " + (string)arg2 + "}"; }
};

Instruction disasm(data address);

//To record screen related settings for each line
struct LineSettings {
	unsigned short scx, scy, wx, wy;
	bool winEnabled, spritesEnabled;
	byte lcdc;
};

//Architecture singleton
class Architecture
{
	static Architecture* singleton;

	Architecture();

public:
	data time = 0; //Counted in clock cycles (NOP = 4)

	byte A, F, B, C, D, E, H, L;
	word PC, SP;

	// Combined registers definitions
	CombinedRegister AF = { &A, &F };
	CombinedRegister BC = { &B, &C };
	CombinedRegister DE = { &D, &E };
	CombinedRegister HL = { &H, &L };

	//Flags to indicate CPU states
	bool IN_STOP = false;
	bool IN_HALT = false;
	bool IME = true;		//Interrupt Master Enable
	bool scheduledIME = true; //Used to perform the 1-instr delay of EI and DI

	//Common address space
	byte ram[RAM_SIZE];

	//Working RAM banks
	//0xC000 - 0xCFFF is always Bank0
	//0xD000 - 0xDFFF is switched with the SVBK register
	byte workingBanks[7][W_BANK_SIZE];
	unsigned int currentWorkingBank = 0; //0 is Bank1

	//Function to perform bank switching
	void swapWorkingBank(unsigned short selected);

	//Video RAM banks
	//0x8000 - 0x9800 is switched with the VBK register
	byte videoBanks[2][V_BANK_SIZE];
	unsigned int currentVideoBank = 0;
	bool videoBanksDirty = false;

	//Function to perform bank switching
	void swapVideoBank(unsigned short selected);

	//Update the content of the video bank storage from memory
	void updateVideoBank();

	//Function to perform Direct Memory Access
	void runDMA(byte a);

	//Function to perform New Direct Memory Access
	void runVDMA(data src, data dst);

	//Color palettes
	//8 palettes
	//4 colors
	//2 - high (0) and low (1)
	byte colorPalettes[8][4][2];

	//OBJ palettes
	//8 palettes
	//4 colors
	//2 - high (0) and low (1)
	byte objPalettes[8][4][2];

	//Last available buttons values
	bool Abtn, Bbtn, start, select, up, down, left, right;

	//Game cartridge
	Cartridge cart;
	unsigned int currentROMBank = 1;
	unsigned short currentRAMBank = 0;

	//Function to perform bank switching
	void swapCartROMBank(unsigned int selected);

	//Function to perform bank switching
	void swapCartRAMBank(unsigned short selected);

	//Convenient bool that can be consulted to know
	//if the processor is in double speed mode
	bool doubleSpeed = false;

	//Convenient bool that can be consulted to know
	//if the LCDC is turned on or off
	bool screenOn = true;

	//Functions to access and modify flag register
	bool Cflag() { return F[4]; }
	bool Hflag() { return F[5]; }
	bool Nflag() { return F[6]; }
	bool Zflag() { return F[7]; }
	
	void Cflag(bool x) { F[4] = x; }
	void Hflag(bool x) { F[5] = x; }
	void Nflag(bool x) { F[6] = x; }
	void Zflag(bool x) { F[7] = x; }

	//Function that executes instructions
	//Returns false if there was an error while executing the instruction
	bool exec(Instruction i);
	bool exec(Command cmd, Argument arg1 = Argument{ NONE }, Argument arg2 = Argument{ NONE });

	//Manage display related aspects
	void lcdc();
	LineSettings lineSet[160];
	unsigned short lcdcMode = 0;
	std::chrono::steady_clock::time_point lastVBlank;

	//Execute one instruction pointed by PC
	data step(bool& debug);

	//Write the content of the ram in a .bin file
	void dump_ram();

	//Write the content of video ram banks in a .bin file
	void dump_video();

	//Print the contents of the cpu in the terminal
	void print_registers();
	
	//Print the first elements of the stack in the terminal
	void print_stack(unsigned int rows);

	//Print a number of memory cells
	void print_ram(data address, unsigned int rows);

	//Print a human readable instruction
	void print_instructions(data address, unsigned int rows);
	
	//Print color palettes
	void print_palettes();
	
	//Print currently used banks
	void print_banks();

	static Architecture* instance()
	{
		if (singleton == NULL)
			singleton = new Architecture();

		return singleton;
	}

	//Loads a rom file in memory
	void loadROM(string romFileName);
};