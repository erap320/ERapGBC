#include "architecture.hpp"
using std::cout;
using std::endl;

//Used to implement various features
//triggered when writing to registers
void address_checks(data address, byte val)
{
	Architecture* arch = Architecture::instance();

	switch (address)
	{
	case SVBK: //Working RAM Banks
		arch->swapWorkingBank((val & (byte)7).to_ulong());
		break;
	case VBK: //Video RAM Banks
		arch->swapVideoBank((val & (byte)1).to_ulong());
		break;
	case DMA: //Direct Memory Access
		arch->runDMA(val);
		break;
	case BCPD: //Color palette data write
	{
		byte specification = arch->ram[BCPS];
		unsigned short color = ((specification & (byte)7) >> 1).to_ulong();
		unsigned short number = ((specification & (byte)0x3F) >> 3).to_ulong();
		arch->colorPalettes[number][color][specification[0]] = val;

		if (specification[7])
			arch->ram[BCPS] = specification.to_ulong() + 1;
		break;
	}
	case OCPD: //OBJ palette data write
	{
		byte specification = arch->ram[OCPS];
		unsigned short color = ((specification & (byte)7) >> 1).to_ulong();
		unsigned short number = ((specification & (byte)0x3F) >> 3).to_ulong();
		arch->objPalettes[number][color][specification[0]] = val;

		if (specification[7])
			arch->ram[OCPS] = specification.to_ulong() + 1;
		break;
	}
	case CART_BANK_TYPE:
	{
		//Don't really care during emulation
		//If 1 is written it also enables RAM banks
		//but we always allow both types
		break;
	}
	case CART_RAM_ENABLE:
	{
		//Don't care about this either
		//In emulation there are no hardware problems
		//for which it could be disabled
		break;
	}
	case LCDC:
	{
		arch->screenOn = val[7];
		//Reset timer when we turn on the screen again
		if (arch->screenOn)
			arch->time = 0;
		break;
	}
	case HDMA5:
	{
		data source = (arch->ram[HDMA2] & (byte)0xff00).to_ulong();
		source += (arch->ram[HDMA1] << BYTE_SIZE).to_ulong();
		
		data destination = (arch->ram[HDMA1] & (byte)0xff00).to_ulong();
		destination += ((arch->ram[HDMA1] & (byte)0x1f) << BYTE_SIZE).to_ulong() + 0x8000;

		arch->runVDMA(source, destination);
		break;
	}
	case P1:
	{
		if (!arch->ram[P1][4]) //Read DPAD
		{
			arch->ram[P1][0] = !arch->right;
			arch->ram[P1][1] = !arch->left;
			arch->ram[P1][2] = !arch->up;
			arch->ram[P1][3] = !arch->down;
		}
		if (!arch->ram[P1][5]) //Read A,B,Select;Start
		{
			arch->ram[P1][0] = !arch->Abtn;
			arch->ram[P1][1] = !arch->Bbtn;
			arch->ram[P1][2] = !arch->select;
			arch->ram[P1][3] = !arch->start;
		}
	}
	default:
	{
		if (address >= CART_ROM_BANK && address <= CART_ROM_BANK + 0x0FFF)
		{
			switch (arch->cart.controller) {
			case NO_CONTROLLER:
				break;
			default:
				arch->swapCartROMBank(val, arch->highROMBankNum);
				break;
			}
		}
		else if (address >= CART_ROM_9TH_BIT_BANK && address <= CART_ROM_9TH_BIT_BANK + 0x0FFF)
		{
			switch (arch->cart.controller) {
			case NO_CONTROLLER:
				break;
			default:
				arch->swapCartROMBank(arch->lowROMBankNum, val);
				break;
			}
		}
		else if (address >= CART_RAM_BANK && address <= CART_RAM_BANK + 0x1FFF) {
			arch->swapCartRAMBank(val);
		}
		else if (address >= 0xE000 && address <= 0xFE00)
			arch->ram[address - 0x2000] = val;
		else if (address >= 0xC000 && address <= 0xDE00)
			arch->ram[address + 0x2000] = val;
		else if (address >= 0x8000 && address <= 0x9FFF)
		{
			arch->videoBanksDirty = true;
		}

		break;
	}
	}
}

byte Argument::r8()
{
	if (address)
	{
		switch (type)
		{
		case NONE: {
			error("Can't read from NONE argument");
			throw WrongExecutionException();
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
			throw WrongExecutionException();
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
			throw WrongExecutionException();
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
			throw ReadWriteException();
			return 0;
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			throw WrongExecutionException();
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
			throw WrongExecutionException();
			break;
		}
		case IMM: {
			data address = 0xff00 + (value.immediate & 0xff);
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (val & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = val;
			}

			address_checks(address, val);
			
			break;
		}
		case REG: {
			data address = 0xff00 + value.reg->to_ulong();
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (val & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = val;
			}

			address_checks(address, val);

			break;
		}
		case W_REG: {
			data address = value.w_reg->to_ulong();
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (val & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = val;
			}

			address_checks(address, val);

			break;
		}
		case C_REG: {
			data address = ((word)*value.c_reg).to_ulong();
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (val & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = val;
			}

			address_checks(address, val);

			break;
		}
		case W_IMM: {
			data address = value.immediate & 0xffff;
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (val & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = val;
			}

			address_checks(address, val);

			break;
		}
		default: {
			error("Invalid argument type: " + type);
			throw WrongExecutionException();
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
			throw WrongExecutionException();
			break;
		}
		case IMM:
		case W_IMM: {
			error("Can't write value to IMM argument: " + val.to_string());
			throw WrongExecutionException();
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
			throw ReadWriteException();
			break;
		}
		default: {
			error("Invalid argument type: " + type);
			throw WrongExecutionException();
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
			throw WrongExecutionException();
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
			throw WrongExecutionException();
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
			throw WrongExecutionException();
			return 0;
			break;
		}
		case IMM:
		case REG: {
			//An argument of only 8 bits should always be read as a byte
			error("Trying to read a word from a byte");
			throw ReadWriteException();
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
			throw WrongExecutionException();
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
			throw WrongExecutionException();
			break;
		}
		case IMM: {
			data source = val.to_ulong();
			byte low = source & 0xff;
			byte high = (source & 0xffff) >> 8;

			data address = 0xff00 + (value.immediate & 0xff);

			//Write the low byte
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (low & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = low;
			}

			address_checks(address, low);

			//Write the high byte
			address += 1;
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (high & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = high;
			}

			address_checks(address, high);

			break;
		}
		case REG:
		case W_REG:
		case C_REG:
		case W_IMM: {
			data source = val.to_ulong();
			byte low = source & 0xff;
			byte high = (source & 0xffff) >> 8;

			data address = value.immediate & 0xffff;

			//Write the low byte
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (low & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = low;
			}

			address_checks(address, low);

			//Write the high byte
			address += 1;
			if (address == P1)
			{
				Architecture* arch = Architecture::instance();
				arch->ram[P1] = (arch->ram[P1] & (byte)0xCF) | (low & (byte)0x30);
			}
			else if (address > 0x7FFF)
			{
				Architecture::instance()->ram[address] = low;
			}

			address_checks(address, low);

			break;
		}
		default: {
			error("Invalid argument type: " + type);
			throw WrongExecutionException();
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
			throw WrongExecutionException();
			break;
		}
		case IMM:
		case W_IMM: {
			error("Can't write value to IMM argument: " + val.to_string());
			throw WrongExecutionException();
			break;
		}
		case REG: {
			//An argument of only 8 bits should always be written as a byte
			error("Trying to write a word to a byte");
			throw ReadWriteException();
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
			throw WrongExecutionException();
			break;
		}
		}
	}
}

bool Architecture::exec(Instruction i)
{
	return exec(i.cmd, i.arg1, i.arg2);
}

data Architecture::step(bool& debugger)
{
	if (IN_STOP)
	{
		//Change processor speed
		ram[KEY1][7] = ram[KEY1][0];
		doubleSpeed = ram[KEY1][0];

		//Stay in STOP mode until a button is pressed
		/*if (ram[IF][4])
			IN_STOP = false;*/

		IN_STOP = false;

		//And advance time
		if (doubleSpeed)
			time += 2;
		else
			time += 4;

		lcdc();

		data address = PC.to_ulong();
		Instruction instr = disasm(address);
		if (debugger)
		{
			print_registers();

			if (PC.to_ulong() != address + instr.length())
				print_instructions(PC.to_ulong(), 5);
			else
				print_instructions(address, 5);

			print_stack(5);
		}

		return true;
	}
	else if (IN_HALT)
	{
		//Stay in HALT mode until an interrupt happens
		if ( (ram[IF] & ram[IE] & (byte)0x1f) != 0)
			IN_HALT = false;

		//And advance time
		if (doubleSpeed)
			time += 2;
		else
			time += 4;

		lcdc();

		data address = PC.to_ulong();
		Instruction instr = disasm(address);
		if (debugger)
		{
			print_registers();

			if (PC.to_ulong() != address + instr.length())
				print_instructions(PC.to_ulong(), 5);
			else
				print_instructions(address, 5);

			print_stack(5);
		}

		return true;
	}
	else
	{
		//Manage interrupts if there is any
		if (IME)
		{
			byte interrupts = ram[IF] & ram[IE] & (byte)0x1f; //IF masked with IE

			//One of the interrupts will be executed
			if (interrupts.to_ulong() != 0)
			{
				IME = false;

				//Push PC
				//Decrement
				data stack = SP.to_ulong() - 2;
				SP = stack;
				//Copy
				ram[stack + 1] = (PC >> BYTE_SIZE).to_ulong();
				ram[stack] = (PC & (word)0xff).to_ulong();

				if (interrupts[0]) //Vblank
				{
					debug("Vblank interrupt", PC);
					PC = 0x40;
					ram[IF][0] = false;
				}
				else if (interrupts[1]) //LCDC
				{
					debug("LCDC interrupt", PC);
					PC = 0x48;
					ram[IF][1] = false;
				}
				else if (interrupts[2]) //Timer overflow
				{
					debug("Timer interrupt", PC);
					PC = 0x50;
					ram[IF][2] = false;
				}
				else if (interrupts[3]) //Serial transfer completion
				{
					debug("Serial interrupt", PC);
					//Serial transfer not implemented
					//Should never happen, but we pretend it can, just in case
					ram[IF][3] = false;
				}
				else if (interrupts[4]) //Buttons
				{
					debug("Joypad interrupt", PC);
					PC = 0x60;
					ram[IF][4] = false;
				}
			}
		}

		//Fetch and decode instruction
		data address = PC.to_ulong();
		Instruction instr = disasm(address);

		//Debug output for notable instructions
		switch (instr.cmd)
		{
		case EI:
		case DI:
		case RETI:
			info(cmd_codes[instr.cmd], PC);
			break;
		}

		//Increase program counter
		PC = (address + instr.length()) & 0xffff;

		//cout << to_hex(address) << endl;

		bool result = false;
		try {
			//Execute
			result = exec(instr);
		}
		catch (const ReadWriteException& e)
		{
			error("Read/Write size mismatch while executing instruction", (word)address);
			debugger = true;
		}
		catch (const WrongExecutionException& e)
		{
			error("Error while executing instruction " + cmd_codes[instr.cmd], (word)address);
			debugger = true;
		}
		catch (const UninmplementedException& e)
		{
			error("Unimplemented feature encountered during execution", (word)address);
			debugger = true;
		}
		catch (const ArchitectureException& e)
		{
			error("Inconsistent operation encountered during execution", (word)address);
			debugger = true;
		}

		//Convert machine cycles to clock cycles
		//And advance time
		if (doubleSpeed)
			time += instr.delay * 2;
		else
			time += instr.delay * 4;

		//Manage display
		lcdc();

		if (debugger || !result)
		{
			print_registers();

			if (PC.to_ulong() != address + instr.length())
				print_instructions(PC.to_ulong(), 5);
			else
				print_instructions(address, 5);

			print_stack(5);
		}

		//Perform the change if a previoud DI or EI instruction scheduled it
		if (instr.cmd != DI && instr.cmd != EI)
			IME = scheduledIME;

		return PC.to_ulong();
	}
}