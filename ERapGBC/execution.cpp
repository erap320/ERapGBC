#include "architecture.hpp"
using std::cout;
using std::endl;

byte Argument::r8()
{
	if (address)
	{
		switch (type)
		{
		case NONE: {
			error("Can't read from NONE argument");
			return 0;
			break;
		}
		case IMM: {
			return Architecture::instance()->ram[0xff00 + value.immediate & 0xff];
			break;
		}
		case REG: {
			return Architecture::instance()->ram[0xff00 + value.reg->to_ulong()];
			break;
		}
		case W_REG: {
			return Architecture::instance()->ram[value.w_reg->to_ulong()];
			break;
		}
		case C_REG: {
			return Architecture::instance()->ram[((word)*value.c_reg).to_ulong()];
			break;
		}
		case W_IMM: {
			return Architecture::instance()->ram[value.immediate & 0xffff];
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			return 0;
			break;
		}
		}
	}
	else
	{
		switch (type)
		{
		case NONE: {
			error("Can't read from NONE argument");
			return 0;
			break;
		}
		case IMM: {
			return value.immediate & 0xff;
			break;
		}
		case REG: {
			return *value.reg;
			break;
		}
		case C_REG:
		case W_REG:
		case W_IMM: {
			//An argument of 16 bits should always be read as a word
			error("Trying to read a byte from a word");
			return 0;
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			return 0;
			break;
		}
		}
	}
}

void Argument::w8(byte val)
{
	if (address)
	{
		switch (type)
		{
		case NONE: {
			error("Can't write value to NONE argument: " + val.to_string());
			break;
		}
		case IMM: {
			Architecture::instance()->ram[0xff00 + value.immediate & 0xff] = val;
			break;
		}
		case REG: {
			Architecture::instance()->ram[0xff00 + value.reg->to_ulong()] = val;
			break;
		}
		case W_REG: {
			Architecture::instance()->ram[value.w_reg->to_ulong()] = val;
			break;
		}
		case C_REG: {
			Architecture::instance()->ram[((word)*value.c_reg).to_ulong()] = val;
			break;
		}
		case W_IMM: {
			Architecture::instance()->ram[value.immediate & 0xffff] = val;
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			break;
		}
		}
	}
	else
	{
		switch (type)
		{
		case NONE: {
			error("Can't write value to NONE argument: " + val.to_string());
			break;
		}
		case IMM:
		case W_IMM: {
			error("Can't write value to IMM argument: " + val.to_string());
			break;
		}
		case REG: {
			*(value.reg) = val;
			break;
		}
		case W_REG:
		case C_REG: {
			//An argument of 16 bits should always be written as a word
			error("Trying to write a byte to a word");
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			break;
		}
		}
	}
}

word Argument::r16()
{
	if (address)
	{
		switch (type)
		{
		case NONE: {
			error("Can't read from NONE argument");
			return 0;
			break;
		}
		case IMM: {
			Architecture* arch = Architecture::instance();
			data addr = 0xff00 + value.immediate & 0xff;
			return word(arch->ram[addr].to_string() + arch->ram[addr + 1].to_string());
			break;
		}
		case REG: {
			Architecture* arch = Architecture::instance();
			data addr = 0xff00 + value.reg->to_ulong();
			return word(arch->ram[addr].to_string() + arch->ram[addr + 1].to_string());
			break;
		}
		case W_REG: {
			Architecture* arch = Architecture::instance();
			data addr = value.w_reg->to_ulong();
			return word(arch->ram[addr].to_string() + arch->ram[addr + 1].to_string());
			break;
		}
		case C_REG: {
			Architecture* arch = Architecture::instance();
			data addr = ((word)*value.c_reg).to_ulong();
			return word(arch->ram[addr].to_string() + arch->ram[addr + 1].to_string());
			break;
		}
		case W_IMM: {
			Architecture* arch = Architecture::instance();
			data addr = value.immediate & 0xffff;
			return word(arch->ram[addr].to_string() + arch->ram[addr + 1].to_string());
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			return 0;
			break;
		}
		}
	}
	else
	{
		switch (type)
		{
		case NONE: {
			error("Can't read from NONE argument");
			return 0;
			break;
		}
		case IMM:
		case REG: {
			//An argument of only 8 bits should always be read as a byte
			error("Trying to read a word from a byte");
			return 0;
			break;
		}
		case W_REG: {
			return *value.w_reg;
			break;
		}
		case C_REG: {
			return *value.c_reg;
			break;
		}
		case W_IMM: {
			return value.immediate & 0xffff;
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			return 0;
			break;
		}
		}
	}
}

void Argument::w16(word val)
{
	if (address)
	{
		switch (type)
		{
		case NONE: {
			error("Can't write value to NONE argument: " + val.to_string());
			break;
		}
		case IMM: {
			Architecture* arch = Architecture::instance();
			data addr = 0xff00 + value.immediate & 0xff;
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			break;
		}
		case REG: {
			Architecture* arch = Architecture::instance();
			data addr = 0xff00 + value.reg->to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			break;
		}
		case W_REG: {
			Architecture* arch = Architecture::instance();
			data addr = value.w_reg->to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			break;
		}
		case C_REG: {
			Architecture* arch = Architecture::instance();
			data addr = ((word)*value.c_reg).to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			break;
		}
		case W_IMM: {
			Architecture* arch = Architecture::instance();
			data addr = value.immediate & 0xffff;
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			break;
		}
		}
	}
	else
	{
		switch (type)
		{
		case NONE: {
			error("Can't write value to NONE argument: " + val.to_string());
			break;
		}
		case IMM:
		case W_IMM: {
			error("Can't write value to IMM argument: " + val.to_string());
			break;
		}
		case REG: {
			//An argument of only 8 bits should always be written as a byte
			error("Trying to write a word to a byte");
			break;
		}
		case W_REG: {
			*value.w_reg = val;
			break;
		}
		case C_REG: {
			*value.c_reg = val;
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			break;
		}
		}
	}
}

bool Architecture::exec(Instruction i)
{
	return exec(i.cmd, i.arg1, i.arg2);
}

bool Architecture::exec(Command cmd, Argument arg1, Argument arg2)
{
	switch (cmd)
	{
	case LDH:
	case LD: {
		if (arg1.type == NONE || arg2.type == NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with LD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Copy
		arg1.copy(arg2);
		return true;
		break;
	}
	case LDD: {
		if (arg1.type == C_REG && arg1.address && arg2.type == REG && !arg2.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			arg2.w16(arg2.r16().to_ulong() - 1);
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			arg1.w16(arg1.r16().to_ulong() - 1);
			return true;
		}
		else
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with LDD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		break;
	}
	case LDI: {
		if (arg1.type == C_REG && arg1.address && arg2.type == REG && !arg2.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			arg2.w16(arg2.r16().to_ulong() + 1);
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			arg1.w16(arg1.r16().to_ulong() + 1);
			return true;
		}
		else
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with LDI instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		break;
	}
	case LDHL: {
		if (arg1.type == W_REG && !arg1.address && arg2.type == IMM && !arg2.address)
		{
			//Copy
			HL = arg1.r16();
			//Decrement
			data res = arg2.r16().to_ulong() + arg2.r8().to_ulong();
			arg2.w16(res & 0xffff);
			//Flags
			Zflag(0);
			Nflag(0);
			//TODO half carry
			Cflag(res > 0xffff); //TODO check

			return true;
		}
		else
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with LDHL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		break;
	}
	case PUSH: {
		if (arg1.type != C_REG || arg1.address || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with PUSH instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		else
		{
			word val = arg1.r16();
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (val >> BYTE_SIZE).to_ulong();
			ram[stack - 1] = (val & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;
			return true;
		}
		break;
	}
	case POP: {
		if (arg1.type != C_REG || arg1.address || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with PUSH instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		else
		{
			data stack = SP.to_ulong();
			arg1.w16(word(ram[stack].to_string() + ram[stack + 1].to_string()));
			//Increment
			SP = SP.to_ulong() + 2;
			return true;
		}
		break;
	}
	case ADD: {
		if (arg1.type == NONE || arg2.type == NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with ADD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Add
		data res = arg1.r8().to_ulong() + arg2.r8().to_ulong();

		switch (arg1.type)
		{
		case REG:
			arg1.w8(res & 0xff);
		case W_REG:
		case C_REG:
			arg1.w16(res & 0xffff);
		}

		//Flags
		switch (arg1.type)
		{
		case REG:
			Zflag(res == 0);
		case W_REG:
			Zflag(0);
		}
		Nflag(0);
		//TODO half carry
		switch (arg1.type)
		{
		case REG:
			Cflag(res > 0xff);
		case C_REG:
		case W_REG:
			Cflag(res > 0xffff);
		}

		return true;
	}
	case ADC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with ADC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Add
		data res = arg1.r8().to_ulong() + arg2.r8().to_ulong() + (unsigned int)Cflag();
		arg1.w8(res & 0xffff);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		//TODO half carry
		Cflag(res > 0xff);

		return true;
	}
	case SUB: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SUB instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Sub
		data res = A.to_ulong() - arg1.r8().to_ulong();
		A = res & 0xffff;
		//Flags
		Zflag(res == 0);
		Nflag(1);
		//TODO half carry
		Cflag(A[7] != 0 || arg1.r8()[7] != 1);

		return true;
	}
	case SBC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SUBC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Sub
		data res = arg1.r8().to_ulong() - arg2.r8().to_ulong() - Cflag();
		arg1.w8(res & 0xffff);
		//Flags
		Zflag(res == 0);
		Nflag(1);
		//TODO half carry
		Cflag(arg1.r8()[7] != 0 || arg2.r8()[7] != 1);

		return true;
	}
	case AND: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with AND instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//And
		byte res = A & arg1.r8();
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(1);
		Cflag(0);

		return true;
	}
	case OR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with OR instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Or
		byte res = A | arg1.r8();
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(0);

		return true;
	}
	case XOR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with XOR instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Xor
		byte res = A ^ arg1.r8();
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(0);

		return true;
	}
	case CP: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CP instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Flags
		Zflag(A == arg1.r8());
		Nflag(1);
		//TODO half carry
		Cflag(A.to_ulong() < arg1.r8().to_ulong());

		return true;
	}
	case INC: {
		if (arg1.type == NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with INC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Increase
		data res = arg1.r8().to_ulong() + 1;
		switch (arg1.type)
		{
		case REG:
			arg1.w8(res & 0xff);
		case W_REG:
		case C_REG:
			arg1.w16(res & 0xffff);
		}
		//Flags
		if (arg1.type == REG)
		{
			Zflag(res == 0);
			Nflag(0);
			//TODO half carry
		}
		//Cflag unaffected

		return true;
	}
	case DEC: {
		if (arg1.type == NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with DEC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Decrease
		data res = arg1.r8().to_ulong() - 1;
		switch (arg1.type)
		{
		case REG:
			arg1.w8(res & 0xff);
		case W_REG:
		case C_REG:
			arg1.w16(res & 0xffff);
		}
		//Flags
		Zflag(res == 0);
		if (arg1.type == REG)
		{
			Zflag(res == 0);
			Nflag(1);
			//TODO half carry
		}
		//TODO half carry
		//Cflag unaffected

		return true;
	}
	case SWAP: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SWAP instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Swap
		byte res = arg1.r8() & (byte)0xf0;
		res >>= 4;
		res = res.to_ulong() + ((arg1.r8() & (byte)0x0f) << 4).to_ulong();
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(0);

		return true;
	}
	case DAA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with DAA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		warning("DAA not implemented yet");

		return true;
	}
	case CPL: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CPL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Not
		A = ~A;
		//Flags
		//Zflag not affected
		Nflag(1);
		Hflag(1);
		//Cflag not affected

		return true;
	}
	case CCF: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CCF instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Flags
		//Zflag not affected
		Nflag(0);
		Hflag(0);
		Cflag(!Cflag());

		return true;
	}
	case SCF: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SCF instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Flags
		//Zflag not affected
		Nflag(0);
		Hflag(0);
		Cflag(1);

		return true;
	}
	case NOP: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with NOP instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Nothing :)

		return true;
	}
	case HALT: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with HALT instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//TODO implement halt
		//Power down CPU until an interrupt occur

		return true;
	}
	case STOP: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with STOP instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//TODO implement stop
		//Halt CPU & LCD display until button pressed

		return true;
	}
	case DI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with DI instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//TODO implement DI
		//Interrupts are disabled after  instruction after DI is executed

		return true;
	}
	case EI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with EI instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//TODO implement EI
		//Interrupts are enabled after  instruction after EI is executed

		return true;
	}
	case RLCA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RLCA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate left
		byte res = A << 1;
		res[0] = A[7];
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(A[7]);

		return true;
	}
	case RLA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RLA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate left
		byte res = A << 1;
		res[0] = Cflag();
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(A[7]);

		return true;
	}
	case RRCA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RRCA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate right
		byte res = A >> 1;
		res[7] = A[0];
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(A[0]);

		return true;
	}
	case RRA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RRA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate right
		byte res = A >> 1;
		res[7] = Cflag();
		A = res;
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(A[0]);

		return true;
	}
	case RLC: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RLC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate left
		byte old = arg1.r8();
		byte res = old << 1;
		res[0] = old[7];
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[7]);

		return true;
	}
	case RL: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate left
		byte old = arg1.r8();
		byte res = old << 1;
		res[0] = Cflag();
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[7]);

		return true;
	}
	case RRC: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RRC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate right
		byte old = arg1.r8();
		byte res = old >> 1;
		res[7] = old[0];
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[0]);

		return true;
	}
	case RR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RR instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Rotate right
		byte old = arg1.r8();
		byte res = old >> 1;
		res[7] = Cflag();
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[0]);

		return true;
	}
	case SLA: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SLA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Shift left
		byte old = arg1.r8();
		byte res = old << 1;
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[7]);

		return true;
	}
	case SRA: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SRA instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Shift right
		byte old = arg1.r8();
		byte res = old >> 1;
		res[7] = old[7];
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[0]);

		return true;
	}
	case SRL: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SRL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Shift right
		byte old = arg1.r8();
		byte res = old >> 1;
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		Hflag(0);
		Cflag(old[0]);

		return true;
	}
	case BIT: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with BIT instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();

		//Flags
		Zflag(!(arg2.r8()[bit]));
		Nflag(0);
		Hflag(1);
		//Cflag not affected

		return true;
	}
	case SET: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with SET instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();
		byte res = arg2.r8();
		res[bit] = 1;

		arg2.w8(res);

		return true;
	}
	case RES: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RES instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();
		byte res = arg2.r8();
		res[bit] = 0;

		arg2.w8(res);

		return true;
	}
	case JP: {
		if ((arg1.type != W_IMM && (arg1.type != C_REG || !arg1.address)) || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JP instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		PC = arg1.r16();

		return true;
	}
	case JPNZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JP(NZ) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Zflag())
			PC = arg1.r16();

		return true;
	}
	case JPZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JP(Z) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Zflag())
			PC = arg1.r16();

		return true;
	}
	case JPNC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JP(NC) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Cflag())
			PC = arg1.r16();

		return true;
	}
	case JPC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JP(C) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Cflag())
			PC = arg1.r16();

		return true;
	}
	case JR: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JR instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Cflag())
			PC = PC.to_ulong() + (arg1.r16().to_ulong() & 0xffff);

		return true;
	}
	case JRNZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JR(NZ) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Zflag())
			PC = PC.to_ulong() + (arg1.r16().to_ulong() & 0xffff);

		return true;
	}
	case JRZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JR(Z) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Zflag())
			PC = PC.to_ulong() + (arg1.r16().to_ulong() & 0xffff);

		return true;
	}
	case JRNC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JR(NC) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Cflag())
			PC = PC.to_ulong() + (arg1.r16().to_ulong() & 0xffff);

		return true;
	}
	case JRC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with JR(C) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Cflag())
			PC = PC.to_ulong() + (arg1.r16().to_ulong() & 0xffff);

		return true;
	}
	case CALL: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CALL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Push
		word savedPC = PC.to_ulong() + 1;
		data stack = SP.to_ulong();
		//Copy
		ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
		ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
		//Decrement
		SP = SP.to_ulong() - 2;

		//Jump
		PC = arg1.r16();

		return true;
	}
	case CALLNZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CALL(NZ) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Zflag())
		{
			//Push
			word savedPC = PC.to_ulong() + 1;
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
			ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;

			//Jump
			PC = arg1.r16();
		}

		return true;
	}
	case CALLZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CALL(Z) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Zflag())
		{
			//Push
			word savedPC = PC.to_ulong() + 1;
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
			ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;

			//Jump
			PC = arg1.r16();
		}

		return true;
	}
	case CALLNC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CALL(NC) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Cflag())
		{
			//Push
			word savedPC = PC.to_ulong() + 1;
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
			ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;

			//Jump
			PC = arg1.r16();
		}

		return true;
	}
	case CALLC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with CALL(C) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Cflag())
		{
			//Push
			word savedPC = PC.to_ulong() + 1;
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
			ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;

			//Jump
			PC = arg1.r16();
		}

		return true;
	}
	case RST: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RST instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Push
		word savedPC = PC;
		data stack = SP.to_ulong();
		//Copy
		ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
		ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
		//Decrement
		SP = SP.to_ulong() - 2;

		//Jump
		PC = arg1.r8().to_ulong();

		return true;
	}
	case RET: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Pop
		data stack = SP.to_ulong();
		PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
		//Increment
		SP = SP.to_ulong() + 2;

		return true;
	}
	case RETNZ: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET(NZ) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Zflag())
		{
			//Pop
			data stack = SP.to_ulong();
			PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
			//Increment
			SP = SP.to_ulong() + 2;
		}

		return true;
	}
	case RETZ: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET(Z) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Zflag())
		{
			//Pop
			data stack = SP.to_ulong();
			PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
			//Increment
			SP = SP.to_ulong() + 2;
		}

		return true;
	}
	case RETNC: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET(NC) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (!Cflag())
		{
			//Pop
			data stack = SP.to_ulong();
			PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
			//Increment
			SP = SP.to_ulong() + 2;
		}

		return true;
	}
	case RETC: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET(C) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		if (Cflag())
		{
			//Pop
			data stack = SP.to_ulong();
			PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
			//Increment
			SP = SP.to_ulong() + 2;
		}

		return true;
	}
	case RETI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			printf("%#.4x | ", PC.to_ulong());
			error("One or more arguments type incompatible with RET(NZ) instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Pop
		data stack = SP.to_ulong();
		PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
		//Increment
		SP = SP.to_ulong() + 2;

		//TODO implement EI

		return true;
	}
	default: {
		printf("%#.4x | ", PC.to_ulong());
		error("Invalid instruction: " + to_string(cmd));
		return false;
		break;
	}
	}
}

bool Architecture::step(bool debug)
{
	//Fetch and decode instruction
	data address = PC.to_ulong();
	Instruction instr = disasm(address);

	//Execute
	bool result = exec(instr);

	if (debug || !result)
	{
		cout << "\n################ INSTRUCTION ################\n";
		printf("%#.4x | ", address);
		cout << (string)instr << endl;

		print_registers();

		print_stack(5);
	}

	//Increase program counter
	PC = (address + instr.length) & 0xffff;

	return result;
}