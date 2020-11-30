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
			return Architecture::instance()->ram[0xff00 + (value.immediate & 0xff)];
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
			data address = 0xff00 + (value.immediate & 0xff);
			Architecture::instance()->ram[address] = val;

			//RAM echo emulation
			if (address >= 0xE000 && address <= 0xFE00)
				Architecture::instance()->ram[address - 0x2000] = val;
			else if (address >= 0xC000 && address <= 0xDE00)
				Architecture::instance()->ram[address + 0x2000] = val;
			else if (address == SVBK)
				Architecture::instance()->swapWorkingBank( (val & (byte)0x7).to_ulong() );
			
			break;
		}
		case REG: {
			data address = 0xff00 + value.reg->to_ulong();
			Architecture::instance()->ram[address] = val;

			//RAM echo emulation
			if (address >= 0xE000 && address <= 0xFE00)
				Architecture::instance()->ram[address - 0x2000] = val;
			else if (address >= 0xC000 && address <= 0xDE00)
				Architecture::instance()->ram[address + 0x2000] = val;
			else if (address == SVBK)
				Architecture::instance()->swapWorkingBank((val & (byte)0x7).to_ulong());

			break;
		}
		case W_REG: {
			data address = value.w_reg->to_ulong();
			Architecture::instance()->ram[address] = val;

			//RAM echo emulation
			if (address >= 0xE000 && address <= 0xFE00)
				Architecture::instance()->ram[address - 0x2000] = val;
			else if (address >= 0xC000 && address <= 0xDE00)
				Architecture::instance()->ram[address + 0x2000] = val;
			else if (address == SVBK)
				Architecture::instance()->swapWorkingBank((val & (byte)0x7).to_ulong());

			break;
		}
		case C_REG: {
			data address = ((word)*value.c_reg).to_ulong();
			Architecture::instance()->ram[address] = val;

			//RAM echo emulation
			if (address >= 0xE000 && address <= 0xFE00)
				Architecture::instance()->ram[address - 0x2000] = val;
			else if (address >= 0xC000 && address <= 0xDE00)
				Architecture::instance()->ram[address + 0x2000] = val;
			else if (address == SVBK)
				Architecture::instance()->swapWorkingBank((val & (byte)0x7).to_ulong());

			break;
		}
		case W_IMM: {
			data address = value.immediate & 0xffff;
			Architecture::instance()->ram[address] = val;

			//RAM echo emulation
			if (address >= 0xE000 && address <= 0xFE00)
				Architecture::instance()->ram[address - 0x2000] = val;
			else if (address >= 0xC000 && address <= 0xDE00)
				Architecture::instance()->ram[address + 0x2000] = val;
			else if (address == SVBK)
				Architecture::instance()->swapWorkingBank((val & (byte)0x7).to_ulong());

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
			data addr = 0xff00 + (value.immediate & 0xff);
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
			data addr = 0xff00 + (value.immediate & 0xff);
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();

			//RAM echo emulation
			if (addr >= 0xE000 && addr <= 0xFE00)
			{
				addr -= 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}
			else if (addr >= 0xC000 && addr <= 0xDE00)
			{
				addr += 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}

			break;
		}
		case REG: {
			Architecture* arch = Architecture::instance();
			data addr = 0xff00 + value.reg->to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();

			//RAM echo emulation
			if (addr >= 0xE000 && addr <= 0xFE00)
			{
				addr -= 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}
			else if (addr >= 0xC000 && addr <= 0xDE00)
			{
				addr += 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}

			break;
		}
		case W_REG: {
			Architecture* arch = Architecture::instance();
			data addr = value.w_reg->to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();

			//RAM echo emulation
			if (addr >= 0xE000 && addr <= 0xFE00)
			{
				addr -= 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}
			else if (addr >= 0xC000 && addr <= 0xDE00)
			{
				addr += 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}

			break;
		}
		case C_REG: {
			Architecture* arch = Architecture::instance();
			data addr = ((word)*value.c_reg).to_ulong();
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();

			//RAM echo emulation
			if (addr >= 0xE000 && addr <= 0xFE00)
			{
				addr -= 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}
			else if (addr >= 0xC000 && addr <= 0xDE00)
			{
				addr += 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}

			break;
		}
		case W_IMM: {
			Architecture* arch = Architecture::instance();
			data addr = value.immediate & 0xffff;
			arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
			arch->ram[addr + 1] = (val & (word)0xff).to_ulong();

			//RAM echo emulation
			if (addr >= 0xE000 && addr <= 0xFE00)
			{
				addr -= 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}
			else if (addr >= 0xC000 && addr <= 0xDE00)
			{
				addr += 0x2000;
				arch->ram[addr] = (val >> BYTE_SIZE).to_ulong();
				arch->ram[addr + 1] = (val & (word)0xff).to_ulong();
			}

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

//Defined to provide a common error message to all instructions
void instruction_error(data address, Command cmd, Argument arg1, Argument arg2)
{
	printf("0x%.4x | ", address);
	error("One or more arguments type incompatible with " + cmd_codes[cmd] + " instruction: " + (string)arg1 + "," + (string)arg2);
}

bool Architecture::exec(Command cmd, Argument arg1, Argument arg2)
{
	switch (cmd)
	{
	case LDH:
	case LD: {
		if (arg1.type == NONE || arg2.type == NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
			*(arg1.value.c_reg) = ((word) * (arg1.value.c_reg)).to_ulong() - 1;
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			*(arg2.value.c_reg) = ((word) * (arg2.value.c_reg)).to_ulong() - 1;
			return true;
		}
		else
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
			*(arg1.value.c_reg) = ((word) * (arg1.value.c_reg)).to_ulong() + 1;
			return true;
		}
		else if (arg2.type == C_REG && arg2.address && arg1.type == REG && !arg1.address)
		{
			//Copy
			arg1.copy(arg2);
			//Decrement
			*(arg2.value.c_reg) = ((word) * (arg2.value.c_reg)).to_ulong() + 1;
			return true;
		}
		else
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}
		break;
	}
	case PUSH: {
		if (arg1.type != C_REG || arg1.address || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case ADC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case SUB: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case SBC: {
		if (arg1.type != REG || arg1.address || (arg2.type >= W_REG && !arg2.address) || arg2.type == NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case AND: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case OR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case XOR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case CP: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Flags
		Zflag(A == arg1.r8());
		Nflag(1);
		//TODO half carry
		Cflag(A.to_ulong() < arg1.r8().to_ulong());

		return true;
		break;
	}
	case INC: {
		if (arg1.type == NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Increase
		data res;
		switch (arg1.type)
		{
		case REG: {
			res = arg1.r8().to_ulong() + 1;
			arg1.w8(res & 0xff);
			break;
		}
		case W_REG:
		case C_REG:
			res = arg1.r16().to_ulong() + 1;
			arg1.w16(res & 0xffff);
			break;
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
		break;
	}
	case DEC: {
		if (arg1.type == NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Decrease
		data res;
		switch (arg1.type)
		{
		case REG: {
			res = arg1.r8().to_ulong() - 1;
			arg1.w8(res & 0xff);
			break;
		}
		case W_REG:
		case C_REG: {
			res = arg1.r16().to_ulong() - 1;
			arg1.w16(res & 0xffff);
			break;
		}
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
		break;
	}
	case SWAP: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case DAA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//TODO implement DAA
		warning("DAA not implemented yet");

		//Flags
		Zflag(A == 0);
		//Nflag not affected;
		Hflag(0);
		//TODO Cflag

		return true;
		break;
	}
	case CPL: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case CCF: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Flags
		//Zflag not affected
		Nflag(0);
		Hflag(0);
		Cflag(!Cflag());

		return true;
		break;
	}
	case SCF: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Flags
		//Zflag not affected
		Nflag(0);
		Hflag(0);
		Cflag(1);

		return true;
		break;
	}
	case NOP: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Nothing :)

		return true;
		break;
	}
	case HALT: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		IN_HALT = true;

		return true;
		break;
	}
	case STOP: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		IN_STOP = true;

		return true;
		break;
	}
	case DI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		IME = false;

		return true;
		break;
	}
	case EI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		IME = true;

		return true;
		break;
	}
	case RLCA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RLA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RRCA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RRA: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RLC: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RL: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RRC: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RR: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case SLA: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case SRA: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case SRL: {
		if ((arg1.type >= W_REG && !arg1.address) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case BIT: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();

		//Flags
		Zflag(!(arg2.r8()[bit]));
		Nflag(0);
		Hflag(1);
		//Cflag not affected

		return true;
		break;
	}
	case SET: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();
		byte res = arg2.r8();
		res[bit] = 1;

		arg2.w8(res);

		return true;
		break;
	}
	case RES: {
		if (arg1.type != IMM || arg1.address || (arg2.type >= W_REG && !arg2.address))
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		unsigned short bit = (arg1.r8() & (byte)0x7).to_ulong();
		byte res = arg2.r8();
		res[bit] = 0;

		arg2.w8(res);

		return true;
		break;
	}
	case JP: {
		if ((arg1.type != W_IMM && (arg1.type != C_REG || !arg1.address)) || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		PC = arg1.r16();

		return true;
		break;
	}
	case JPNZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (!Zflag())
			PC = arg1.r16();

		return true;
		break;
	}
	case JPZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (Zflag())
			PC = arg1.r16();

		return true;
		break;
	}
	case JPNC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (!Cflag())
			PC = arg1.r16();

		return true;
		break;
	}
	case JPC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (Cflag())
			PC = arg1.r16();

		return true;
		break;
	}
	case JR: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		signed char delta = arg1.r8().to_ulong();
		PC = PC.to_ulong() + delta;

		return true;
		break;
	}
	case JRNZ: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (!Zflag())
		{
			signed char delta = arg1.r8().to_ulong();
			PC = PC.to_ulong() + delta;
		}

		return true;
		break;
	}
	case JRZ: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (Zflag())
		{
			signed char delta = arg1.r8().to_ulong();
			PC = PC.to_ulong() + delta;
		}

		return true;
		break;
	}
	case JRNC: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (!Cflag())
		{
			signed char delta = arg1.r8().to_ulong();
			PC = PC.to_ulong() + delta;
		}

		return true;
		break;
	}
	case JRC: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		if (Cflag())
		{
			signed char delta = arg1.r8().to_ulong();
			PC = PC.to_ulong() + delta;
		}

		return true;
		break;
	}
	case CALL: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Push
		word savedPC = PC.to_ulong() + 3; //Because the CALL instruction is 3 bytes long
		data stack = SP.to_ulong();
		//Copy
		ram[stack] = (savedPC >> BYTE_SIZE).to_ulong();
		ram[stack - 1] = (savedPC & (word)0xff).to_ulong();
		//Decrement
		SP = SP.to_ulong() - 2;

		//Jump
		PC = arg1.r16();

		return true;
		break;
	}
	case CALLNZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case CALLZ: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case CALLNC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case CALLC: {
		if (arg1.type != W_IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RST: {
		if (arg1.type != IMM || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RET: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Pop
		data stack = SP.to_ulong();
		PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
		//Increment
		SP = SP.to_ulong() + 2;

		return true;
		break;
	}
	case RETNZ: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RETZ: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RETNC: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RETC: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
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
		break;
	}
	case RETI: {
		if (arg1.type != NONE || arg2.type != NONE)
		{
			instruction_error(PC.to_ulong(), cmd, arg1, arg2);
			return false;
		}

		//Pop
		data stack = SP.to_ulong();
		PC = word(ram[stack].to_string() + ram[stack + 1].to_string());
		//Increment
		SP = SP.to_ulong() + 2;

		IME = true;

		return true;
		break;
	}
	default: {
		error("Invalid instruction: " + cmd_codes[cmd]);
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

	//Increase program counter
	PC = (address + instr.length()) & 0xffff;

	//Execute
	bool result = exec(instr);

	if (debug || !result)
	{
		print_registers();
		
		if (PC.to_ulong() != address + instr.length())
			print_instructions(PC.to_ulong(), 5);
		else
			print_instructions(address, 5);

		print_stack(5);
	}

	return result;
}