#pragma once
#include <iostream>
#include <ostream>
#include <bitset>
#include "utils.hpp"
using std::bitset;
using std::string;
using std::to_string;

#define RAM_SIZE 65536
#define BYTE_SIZE 8
#define WORD_SIZE 16
typedef bitset<BYTE_SIZE> byte;
typedef bitset<WORD_SIZE> word;

//Because it's the default smallest numeric type in which a bitset exports numbers
typedef unsigned long data;

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
	W_IMM,			/* Immediate word */
};

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
		if (this->type >= W_REG || other.type >= W_REG) //All 16 bit types come after W_REG
			this->w16(other.r16());
		else
			this->w8(other.r8());
	}

	//Define cast to string for printing
	operator string() const { return "[Type: "+to_string(type)+"; Address: "+to_string(address)+"]"; }
};

enum Command {
	LD, LDD, LDI, LDH, LDHL,
	PUSH, POP,
	ADD, ADC, SUB, SUBC,
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
	RST
};

class Instruction {
public:
	Command cmd;
	Argument arg1;
	Argument arg2;
	//Length of the binary instruction in bytes
	unsigned short length;

	Instruction(Command c, Argument a1 = Argument{ NONE }, Argument a2 = Argument{ NONE }, unsigned short len = 0)
	{
		cmd = c;
		arg1 = a1;
		arg2 = a2;
		length = len;
	}

	void setLength(unsigned short len)
	{
		length = len;
	}

	//Define cast to string for printing
	operator string() const { return "{Command: " + to_string(cmd) + "; Arg1: " + (string)arg1 + "; Arg2: " + (string)arg2 + "}"; }
};

Instruction disasm(data address, byte* const& ram);

//Architecture singleton
class Architecture
{
	static Architecture* singleton;

public:
	byte A, F, B, C, D, E, H, L;
	word PC = 0x100;
	word SP = 0xFFFE;

	// Combined registers definitions
	CombinedRegister AF = { &A, &F };
	CombinedRegister BC = { &B, &C };
	CombinedRegister DE = { &D, &E };
	CombinedRegister HL = { &H, &L };

	byte ram[RAM_SIZE];

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

	//Execute one instruction pointed by PC
	bool step(bool debug);

	//Write the content of the ram in a .bin file
	void dump_ram();

	//Print the contents of the cpu in the terminal
	void print_registers();
	
	//Print the first elements of the stack in the terminal
	void print_stack(unsigned int rows);

	static Architecture* instance()
	{
		if (singleton == NULL)
			singleton = new Architecture;

		return singleton;
	}
};