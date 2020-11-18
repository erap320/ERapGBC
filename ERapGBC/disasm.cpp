#include "architecture.hpp"

Instruction disasm(data address)
{
	Architecture* a = Architecture::instance();

	unsigned short opcode = a->ram[address].to_ulong();
	switch (opcode)
	{
	default:
		return Instruction{ ERR, Argument{NONE}, Argument{NONE}, 1 };
		break;
	case 0x06:
		return Instruction{ LD, Argument{a->B}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x0E:
		return Instruction{ LD, Argument{a->C}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x16:
		return Instruction{ LD, Argument{a->D}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x1E:
		return Instruction{ LD, Argument{a->E}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x26:
		return Instruction{ LD, Argument{a->H}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x2E:
		return Instruction{ LD, Argument{a->L}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x7F:
		return Instruction{ LD, Argument{a->A}, Argument{a->A}, 1 };
		break;
	case 0x78:
		return Instruction{ LD, Argument{a->A}, Argument{a->B}, 1 };
		break;
	case 0x79:
		return Instruction{ LD, Argument{a->A}, Argument{a->C}, 1 };
		break;
	case 0x7A:
		return Instruction{ LD, Argument{a->A}, Argument{a->D}, 1 };
		break;
	case 0x7B:
		return Instruction{ LD, Argument{a->A}, Argument{a->E}, 1 };
		break;
	case 0x7C:
		return Instruction{ LD, Argument{a->A}, Argument{a->H}, 1 };
		break;
	case 0x7D:
		return Instruction{ LD, Argument{a->A}, Argument{a->L}, 1 };
		break;
	case 0x7E:
		return Instruction{ LD, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x40:
		return Instruction{ LD, Argument{a->B}, Argument{a->B}, 1 };
		break;
	case 0x41:
		return Instruction{ LD, Argument{a->B}, Argument{a->C}, 1 };
		break;
	case 0x42:
		return Instruction{ LD, Argument{a->B}, Argument{a->D}, 1 };
		break;
	case 0x43:
		return Instruction{ LD, Argument{a->B}, Argument{a->E}, 1 };
		break;
	case 0x44:
		return Instruction{ LD, Argument{a->B}, Argument{a->H}, 1 };
		break;
	case 0x45:
		return Instruction{ LD, Argument{a->B}, Argument{a->L}, 1 };
		break;
	case 0x46:
		return Instruction{ LD, Argument{a->B}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x47:
		return Instruction{ LD, Argument{a->B}, Argument{a->A}, 1 };
		break;
	case 0x48:
		return Instruction{ LD, Argument{a->C}, Argument{a->B}, 1 };
		break;
	case 0x49:
		return Instruction{ LD, Argument{a->C}, Argument{a->C}, 1 };
		break;
	case 0x4A:
		return Instruction{ LD, Argument{a->C}, Argument{a->D}, 1 };
		break;
	case 0x4B:
		return Instruction{ LD, Argument{a->C}, Argument{a->E}, 1 };
		break;
	case 0x4C:
		return Instruction{ LD, Argument{a->C}, Argument{a->H}, 1 };
		break;
	case 0x4D:
		return Instruction{ LD, Argument{a->C}, Argument{a->L}, 1 };
		break;
	case 0x4E:
		return Instruction{ LD, Argument{a->C}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x4F:
		return Instruction{ LD, Argument{a->C}, Argument{a->A}, 1 };
		break;
	case 0x50:
		return Instruction{ LD, Argument{a->D}, Argument{a->B}, 1 };
		break;
	case 0x51:
		return Instruction{ LD, Argument{a->D}, Argument{a->C}, 1 };
		break;
	case 0x52:
		return Instruction{ LD, Argument{a->D}, Argument{a->D}, 1 };
		break;
	case 0x53:
		return Instruction{ LD, Argument{a->D}, Argument{a->E}, 1 };
		break;
	case 0x54:
		return Instruction{ LD, Argument{a->D}, Argument{a->H}, 1 };
		break;
	case 0x56:
		return Instruction{ LD, Argument{a->D}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x57:
		return Instruction{ LD, Argument{a->D}, Argument{a->A}, 1 };
		break;
	case 0x58:
		return Instruction{ LD, Argument{a->E}, Argument{a->B}, 1 };
		break;
	case 0x59:
		return Instruction{ LD, Argument{a->E}, Argument{a->C}, 1 };
		break;
	case 0x5A:
		return Instruction{ LD, Argument{a->E}, Argument{a->C}, 1 };
		break;
	case 0x5B:
		return Instruction{ LD, Argument{a->E}, Argument{a->E}, 1 };
		break;
	case 0x5C:
		return Instruction{ LD, Argument{a->E}, Argument{a->H}, 1 };
		break;
	case 0x5D:
		return Instruction{ LD, Argument{a->E}, Argument{a->L}, 1 };
		break;
	case 0x5E:
		return Instruction{ LD, Argument{a->E}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x5F:
		return Instruction{ LD, Argument{a->E}, Argument{a->A}, 1 };
		break;
	case 0x60:
		return Instruction{ LD, Argument{a->H}, Argument{a->B}, 1 };
		break;
	case 0x61:
		return Instruction{ LD, Argument{a->H}, Argument{a->C}, 1 };
		break;
	case 0x62:
		return Instruction{ LD, Argument{a->H}, Argument{a->D}, 1 };
		break;
	case 0x63:
		return Instruction{ LD, Argument{a->H}, Argument{a->E}, 1 };
		break;
	case 0x64:
		return Instruction{ LD, Argument{a->H}, Argument{a->H}, 1 };
		break;
	case 0x65:
		return Instruction{ LD, Argument{a->H}, Argument{a->L}, 1 };
		break;
	case 0x66:
		return Instruction{ LD, Argument{a->H}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x67:
		return Instruction{ LD, Argument{a->H}, Argument{a->A}, 1 };
		break;
	case 0x68:
		return Instruction{ LD, Argument{a->L}, Argument{a->B}, 1 };
		break;
	case 0x69:
		return Instruction{ LD, Argument{a->L}, Argument{a->C}, 1 };
		break;
	case 0x6A:
		return Instruction{ LD, Argument{a->L}, Argument{a->D}, 1 };
		break;
	case 0x6B:
		return Instruction{ LD, Argument{a->L}, Argument{a->E}, 1 };
		break;
	case 0x6C:
		return Instruction{ LD, Argument{a->L}, Argument{a->H}, 1 };
		break;
	case 0x6D:
		return Instruction{ LD, Argument{a->L}, Argument{a->L}, 1 };
		break;
	case 0x6E:
		return Instruction{ LD, Argument{a->L}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x6F:
		return Instruction{ LD, Argument{a->L}, Argument{a->A}, 1 };
		break;
	case 0x70:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->B}, 1 };
		break;
	case 0x71:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->C}, 1 };
		break;
	case 0x72:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->D}, 1 };
		break;
	case 0x73:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->E}, 1 };
		break;
	case 0x74:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->H}, 1 };
		break;
	case 0x75:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->L}, 1 };
		break;
	case 0x36:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x0A:
		return Instruction{ LD, Argument{a->A}, Argument{a->BC, ADDR}, 1 };
		break;
	case 0x1A:
		return Instruction{ LD, Argument{a->A}, Argument{a->DE, ADDR}, 1 };
		break;
	case 0xFA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{a->A}, Argument{imm.to_ulong(), W_IMM, ADDR}, 3 };
		break;
	}
	case 0x3E:
		return Instruction{ LD, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x02:
		return Instruction{ LD, Argument{a->BC, ADDR}, Argument{a->A}, 1 };
		break;
	case 0x12:
		return Instruction{ LD, Argument{a->DE, ADDR}, Argument{a->A}, 1 };
		break;
	case 0x77:
		return Instruction{ LD, Argument{a->HL, ADDR}, Argument{a->A}, 1 };
		break;
	case 0xEA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{imm.to_ulong(), W_IMM, ADDR}, Argument{a->A}, 3 };
		break;
	}
	case 0xF2:
		return Instruction{ LD, Argument{a->A}, Argument{a->C, ADDR}, 1 };
		break;
	case 0xE2:
		return Instruction{ LD, Argument{a->C, ADDR}, Argument{a->A}, 1 };
		break;
	case 0x3A:
		return Instruction{ LDD, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x32:
		return Instruction{ LDD, Argument{a->HL, ADDR}, Argument{a->A}, 1 };
		break;
	case 0x2A:
		return Instruction{ LDI, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0x22:
		return Instruction{ LDI, Argument{a->HL, ADDR}, Argument{a->A}, 1 };
		break;
	case 0xE0:
		return Instruction{ LDH, Argument{a->ram[address + 1].to_ulong(), IMM, ADDR}, Argument{a->A}, 2 };
		break;
	case 0xF0:
		return Instruction{ LDH, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM, ADDR}, 2 };
		break;
	case 0x01: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{a->BC}, Argument{imm.to_ulong(), W_IMM}, 3};
		break;
	}
	case 0x11: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{a->DE}, Argument{imm.to_ulong(), W_IMM}, 3};
		break;
	}
	case 0x21: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{a->HL}, Argument{imm.to_ulong(), W_IMM}, 3};
		break;
	}
	case 0x31: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{a->SP}, Argument{imm.to_ulong(), W_IMM}, 3};
		break;
	}
	case 0xF9:
		return Instruction{ LD, Argument{a->SP}, Argument{a->HL}, 1 };
		break;
	case 0xF8:
		return Instruction{ LDHL, Argument{a->SP}, Argument{a->ram[address + 1].to_ulong(), IMM, ADDR }, 2 };
		break;
	case 0x08: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, Argument{imm.to_ulong(), W_IMM, ADDR}, Argument{a->SP}, 2 };
		break;
	}
	case 0xF5:
		return Instruction{ PUSH, Argument{a->AF}, Argument{NONE}, 1 };
		break;
	case 0xC5:
		return Instruction{ PUSH, Argument{a->BC}, Argument{NONE}, 1 };
		break;
	case 0xD5:
		return Instruction{ PUSH, Argument{a->DE}, Argument{NONE}, 1 };
		break;
	case 0xE5:
		return Instruction{ PUSH, Argument{a->HL}, Argument{NONE}, 1 };
		break;
	case 0xF1:
		return Instruction{ POP, Argument{a->AF}, Argument{NONE}, 1 };
		break;
	case 0xC1:
		return Instruction{ POP, Argument{a->BC}, Argument{NONE}, 1 };
		break;
	case 0xD1:
		return Instruction{ POP, Argument{a->DE}, Argument{NONE}, 1 };
		break;
	case 0xE1:
		return Instruction{ POP, Argument{a->HL}, Argument{NONE}, 1 };
		break;
	case 0x87:
		return Instruction{ ADD, Argument{a->A}, Argument{a->A}, 1 };
		break;
	case 0x80:
		return Instruction{ ADD, Argument{a->A}, Argument{a->B}, 1 };
		break;
	case 0x81:
		return Instruction{ ADD, Argument{a->A}, Argument{a->C}, 1 };
		break;
	case 0x82:
		return Instruction{ ADD, Argument{a->A}, Argument{a->D}, 1 };
		break;
	case 0x83:
		return Instruction{ ADD, Argument{a->A}, Argument{a->E}, 1 };
		break;
	case 0x84:
		return Instruction{ ADD, Argument{a->A}, Argument{a->H}, 1 };
		break;
	case 0x85:
		return Instruction{ ADD, Argument{a->A}, Argument{a->L}, 1 };
		break;
	case 0x86:
		return Instruction{ ADD, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0xC6:
		return Instruction{ ADD, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x8F:
		return Instruction{ ADC, Argument{a->A}, Argument{a->A}, 1 };
		break;
	case 0x88:
		return Instruction{ ADC, Argument{a->A}, Argument{a->B}, 1 };
		break;
	case 0x89:
		return Instruction{ ADC, Argument{a->A}, Argument{a->C}, 1 };
		break;
	case 0x8A:
		return Instruction{ ADC, Argument{a->A}, Argument{a->D}, 1 };
		break;
	case 0x8B:
		return Instruction{ ADC, Argument{a->A}, Argument{a->E}, 1 };
		break;
	case 0x8C:
		return Instruction{ ADC, Argument{a->A}, Argument{a->H}, 1 };
		break;
	case 0x8D:
		return Instruction{ ADC, Argument{a->A}, Argument{a->L}, 1 };
		break;
	case 0x8E:
		return Instruction{ ADC, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0xCE:
		return Instruction{ ADC, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x97:
		return Instruction{ SUB, Argument{a->A}, Argument{a->A}, 1 };
		break;
	case 0x90:
		return Instruction{ SUB, Argument{a->A}, Argument{a->B}, 1 };
		break;
	case 0x91:
		return Instruction{ SUB, Argument{a->A}, Argument{a->C}, 1 };
		break;
	case 0x92:
		return Instruction{ SUB, Argument{a->A}, Argument{a->D}, 1 };
		break;
	case 0x93:
		return Instruction{ SUB, Argument{a->A}, Argument{a->E}, 1 };
		break;
	case 0x94:
		return Instruction{ SUB, Argument{a->A}, Argument{a->H}, 1 };
		break;
	case 0x95:
		return Instruction{ SUB, Argument{a->A}, Argument{a->L}, 1 };
		break;
	case 0x96:
		return Instruction{ SUB, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0xD6:
		return Instruction{ SUB, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM}, 2 };
		break;
	case 0x9F:
		return Instruction{ SBC, Argument{a->A}, Argument{a->A}, 1 };
		break;
	case 0x98:
		return Instruction{ SBC, Argument{a->A}, Argument{a->B}, 1 };
		break;
	case 0x99:
		return Instruction{ SBC, Argument{a->A}, Argument{a->C}, 1 };
		break;
	case 0x9A:
		return Instruction{ SBC, Argument{a->A}, Argument{a->D}, 1 };
		break;
	case 0x9B:
		return Instruction{ SBC, Argument{a->A}, Argument{a->E}, 1 };
		break;
	case 0x9C:
		return Instruction{ SBC, Argument{a->A}, Argument{a->H}, 1 };
		break;
	case 0x9D:
		return Instruction{ SBC, Argument{a->A}, Argument{a->L}, 1 };
		break;
	case 0x9E:
		return Instruction{ SBC, Argument{a->A}, Argument{a->HL, ADDR}, 1 };
		break;
	case 0xA7:
		return Instruction{ AND, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0xA0:
		return Instruction{ AND, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0xA1:
		return Instruction{ AND, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0xA2:
		return Instruction{ AND, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0xA3:
		return Instruction{ AND, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0xA4:
		return Instruction{ AND, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0xA5:
		return Instruction{ AND, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0xA6:
		return Instruction{ AND, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	case 0xE6:
		return Instruction{ AND, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE}, 2 };
		break;
	case 0xB7:
		return Instruction{ OR, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0xB0:
		return Instruction{ OR, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0xB1:
		return Instruction{ OR, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0xB2:
		return Instruction{ OR, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0xB3:
		return Instruction{ OR, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0xB4:
		return Instruction{ OR, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0xB5:
		return Instruction{ OR, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0xB6:
		return Instruction{ OR, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	case 0xF6:
		return Instruction{ OR, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE}, 2 };
		break;
	case 0xAF:
		return Instruction{ XOR, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0xA8:
		return Instruction{ XOR, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0xA9:
		return Instruction{ XOR, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0xAA:
		return Instruction{ XOR, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0xAB:
		return Instruction{ XOR, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0xAC:
		return Instruction{ XOR, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0xAD:
		return Instruction{ XOR, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0xAE:
		return Instruction{ XOR, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	case 0xEE:
		return Instruction{ XOR, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE}, 2 };
		break;
	case 0xBF:
		return Instruction{ CP, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0xB8:
		return Instruction{ CP, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0xB9:
		return Instruction{ CP, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0xBA:
		return Instruction{ CP, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0xBB:
		return Instruction{ CP, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0xBC:
		return Instruction{ CP, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0xBD:
		return Instruction{ CP, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0xBE:
		return Instruction{ CP, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	case 0xFE:
		return Instruction{ CP, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE}, 2 };
		break;
	case 0x3C:
		return Instruction{ INC, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0x04:
		return Instruction{ INC, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0x0C:
		return Instruction{ INC, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0x14:
		return Instruction{ INC, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0x1C:
		return Instruction{ INC, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0x24:
		return Instruction{ INC, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0x2C:
		return Instruction{ INC, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0x34:
		return Instruction{ INC, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	case 0x3D:
		return Instruction{ DEC, Argument{a->A}, Argument{NONE}, 1 };
		break;
	case 0x05:
		return Instruction{ DEC, Argument{a->B}, Argument{NONE}, 1 };
		break;
	case 0x0D:
		return Instruction{ DEC, Argument{a->C}, Argument{NONE}, 1 };
		break;
	case 0x15:
		return Instruction{ DEC, Argument{a->D}, Argument{NONE}, 1 };
		break;
	case 0x1D:
		return Instruction{ DEC, Argument{a->E}, Argument{NONE}, 1 };
		break;
	case 0x25:
		return Instruction{ DEC, Argument{a->H}, Argument{NONE}, 1 };
		break;
	case 0x2D:
		return Instruction{ DEC, Argument{a->L}, Argument{NONE}, 1 };
		break;
	case 0x35:
		return Instruction{ DEC, Argument{a->HL, ADDR}, Argument{NONE}, 1 };
		break;
	}
}