#include "architecture.hpp"

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
		case W_IMM:{
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
			error("One or more arguments type incompatible with LD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Copy
		arg1 = arg2;
		return true;
		break;
	}
	case LDD: {
		if (arg1.type == C_REG && arg1.address && arg2.type == REG && !arg2.address)
		{
			//Copy
			arg1 = arg2;
			//Decrement
			arg2.w16(arg2.r16().to_ulong() - 1);
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1 = arg2;
			//Decrement
			arg1.w16(arg1.r16().to_ulong() - 1);
			return true;
		}
		else
		{
			error("One or more arguments type incompatible with LDD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		break;
	}
	case LDI: {
		if (arg1.type == C_REG && arg1.address && arg2.type == REG && !arg2.address)
		{
			//Copy
			arg1 = arg2;
			//Decrement
			arg2.w16(arg2.r16().to_ulong() + 1);
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1 = arg2;
			//Decrement
			arg1.w16(arg1.r16().to_ulong() + 1);
			return true;
		}
		else
		{
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
			error("One or more arguments type incompatible with LDHL instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		break;
	}
	case PUSH: {
		if (arg1.type != C_REG || arg1.address || arg2.type != NONE)
		{
			error("One or more arguments type incompatible with PUSH instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		else
		{
			word val = arg1.r16();
			data stack = SP.to_ulong();
			//Copy
			ram[stack] = (val >> BYTE_SIZE).to_ulong();
			ram[stack-1] = (val & (word)0xff).to_ulong();
			//Decrement
			SP = SP.to_ulong() - 2;
			return true;
		}
		break;
	}
	case POP: {
		if (arg1.type != C_REG || arg1.address || arg2.type != NONE)
		{
			error("One or more arguments type incompatible with PUSH instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}
		else
		{
			data stack = SP.to_ulong();
			arg1.w16( word(ram[stack].to_string() + ram[stack + 1].to_string()) );
			//Increment
			SP = SP.to_ulong() + 2;
			return true;
		}
		break;
	}
	case ADD: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
			error("One or more arguments type incompatible with ADD instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//Add
		data res = arg1.r8().to_ulong() + arg2.r8().to_ulong();
		arg1.w8(res & 0xffff);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		//TODO half carry
		Cflag(res > 0xff);

		return true;	
	}
	case ADC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
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
	case SUBC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
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
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			error("One or more arguments type incompatible with INC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//And
		byte res = arg1.r8().to_ulong()+1;
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(0);
		//TODO half carry
		//Cflag unaffected

		return true;
	}
	case DEC: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			error("One or more arguments type incompatible with DEC instruction: " + (string)arg1 + "," + (string)arg2);
			return false;
		}

		//And
		byte res = arg1.r8().to_ulong() - 1;
		arg1.w8(res);
		//Flags
		Zflag(res == 0);
		Nflag(1);
		//TODO half carry
		//Cflag unaffected

		return true;
	}
	default: {
		error("Invalid instruction: " + cmd);
		return false;
		break;
	}
	}
}