#include "architecture.hpp"

Instruction disasm(data address)
{
	Architecture* a = Architecture::instance();

	unsigned short opcode = a->ram[address].to_ulong();
	switch (opcode)
	{
	default:
		return Instruction{ ERR, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x06:
		return Instruction{ LD, 2, Argument{a->B}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x0E:
		return Instruction{ LD, 2, Argument{a->C}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x16:
		return Instruction{ LD, 2, Argument{a->D}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x1E:
		return Instruction{ LD, 2, Argument{a->E}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x26:
		return Instruction{ LD, 2, Argument{a->H}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x2E:
		return Instruction{ LD, 2, Argument{a->L}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x7F:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->A} };
		break;
	case 0x78:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->B} };
		break;
	case 0x79:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->C} };
		break;
	case 0x7A:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->D} };
		break;
	case 0x7B:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->E} };
		break;
	case 0x7C:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->H} };
		break;
	case 0x7D:
		return Instruction{ LD, 1, Argument{a->A}, Argument{a->L} };
		break;
	case 0x7E:
		return Instruction{ LD, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0x40:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->B} };
		break;
	case 0x41:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->C} };
		break;
	case 0x42:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->D} };
		break;
	case 0x43:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->E} };
		break;
	case 0x44:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->H} };
		break;
	case 0x45:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->L} };
		break;
	case 0x46:
		return Instruction{ LD, 2, Argument{a->B}, Argument{a->HL, ADDR} };
		break;
	case 0x47:
		return Instruction{ LD, 1, Argument{a->B}, Argument{a->A} };
		break;
	case 0x48:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->B} };
		break;
	case 0x49:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->C} };
		break;
	case 0x4A:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->D} };
		break;
	case 0x4B:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->E} };
		break;
	case 0x4C:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->H} };
		break;
	case 0x4D:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->L} };
		break;
	case 0x4E:
		return Instruction{ LD, 2, Argument{a->C}, Argument{a->HL, ADDR} };
		break;
	case 0x4F:
		return Instruction{ LD, 1, Argument{a->C}, Argument{a->A} };
		break;
	case 0x50:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->B} };
		break;
	case 0x51:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->C} };
		break;
	case 0x52:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->D} };
		break;
	case 0x53:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->E} };
		break;
	case 0x54:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->H} };
		break;
	case 0x56:
		return Instruction{ LD, 2, Argument{a->D}, Argument{a->HL, ADDR} };
		break;
	case 0x57:
		return Instruction{ LD, 1, Argument{a->D}, Argument{a->A} };
		break;
	case 0x58:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->B} };
		break;
	case 0x59:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->C} };
		break;
	case 0x5A:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->C} };
		break;
	case 0x5B:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->E} };
		break;
	case 0x5C:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->H} };
		break;
	case 0x5D:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->L} };
		break;
	case 0x5E:
		return Instruction{ LD, 2, Argument{a->E}, Argument{a->HL, ADDR} };
		break;
	case 0x5F:
		return Instruction{ LD, 1, Argument{a->E}, Argument{a->A} };
		break;
	case 0x60:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->B} };
		break;
	case 0x61:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->C} };
		break;
	case 0x62:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->D} };
		break;
	case 0x63:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->E} };
		break;
	case 0x64:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->H} };
		break;
	case 0x65:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->L} };
		break;
	case 0x66:
		return Instruction{ LD, 2, Argument{a->H}, Argument{a->HL, ADDR} };
		break;
	case 0x67:
		return Instruction{ LD, 1, Argument{a->H}, Argument{a->A} };
		break;
	case 0x68:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->B} };
		break;
	case 0x69:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->C} };
		break;
	case 0x6A:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->D} };
		break;
	case 0x6B:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->E} };
		break;
	case 0x6C:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->H} };
		break;
	case 0x6D:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->L} };
		break;
	case 0x6E:
		return Instruction{ LD, 2, Argument{a->L}, Argument{a->HL, ADDR} };
		break;
	case 0x6F:
		return Instruction{ LD, 1, Argument{a->L}, Argument{a->A} };
		break;
	case 0x70:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->B} };
		break;
	case 0x71:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->C} };
		break;
	case 0x72:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->D} };
		break;
	case 0x73:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->E} };
		break;
	case 0x74:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->H} };
		break;
	case 0x75:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->L} };
		break;
	case 0x36:
		return Instruction{ LD, 3, Argument{a->HL, ADDR}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x0A:
		return Instruction{ LD, 2, Argument{a->A}, Argument{a->BC, ADDR} };
		break;
	case 0x1A:
		return Instruction{ LD, 2, Argument{a->A}, Argument{a->DE, ADDR} };
		break;
	case 0xFA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 4, Argument{a->A}, Argument{imm.to_ulong(), W_IMM, ADDR} };
		break;
	}
	case 0x3E:
		return Instruction{ LD, 2, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x02:
		return Instruction{ LD, 2, Argument{a->BC, ADDR}, Argument{a->A} };
		break;
	case 0x12:
		return Instruction{ LD, 2, Argument{a->DE, ADDR}, Argument{a->A} };
		break;
	case 0x77:
		return Instruction{ LD, 2, Argument{a->HL, ADDR}, Argument{a->A} };
		break;
	case 0xEA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 4, Argument{imm.to_ulong(), W_IMM, ADDR}, Argument{a->A} };
		break;
	}
	case 0xF2:
		return Instruction{ LD, 2, Argument{a->A}, Argument{a->C, ADDR} };
		break;
	case 0xE2:
		return Instruction{ LD, 2, Argument{a->C, ADDR}, Argument{a->A} };
		break;
	case 0x3A:
		return Instruction{ LDD, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0x32:
		return Instruction{ LDD, 2, Argument{a->HL, ADDR}, Argument{a->A} };
		break;
	case 0x2A:
		return Instruction{ LDI, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0x22:
		return Instruction{ LDI, 2, Argument{a->HL, ADDR}, Argument{a->A} };
		break;
	case 0xE0:
		return Instruction{ LDH, 3, Argument{a->ram[address + 1].to_ulong(), IMM, ADDR}, Argument{a->A} };
		break;
	case 0xF0:
		return Instruction{ LDH, 3, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM, ADDR} };
		break;
	case 0x01: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 3, Argument{a->BC}, Argument{imm.to_ulong(), W_IMM} };
		break;
	}
	case 0x11: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 3, Argument{a->DE}, Argument{imm.to_ulong(), W_IMM} };
		break;
	}
	case 0x21: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 3, Argument{a->HL}, Argument{imm.to_ulong(), W_IMM} };
		break;
	}
	case 0x31: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 3, Argument{a->SP}, Argument{imm.to_ulong(), W_IMM} };
		break;
	}
	case 0xF9:
		return Instruction{ LD, 2, Argument{a->SP}, Argument{a->HL} };
		break;
	case 0xF8:
		return Instruction{ LDHL, 3, Argument{a->SP}, Argument{a->ram[address + 1].to_ulong(), IMM } };
		break;
	case 0x08: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ LD, 5, Argument{imm.to_ulong(), W_IMM, ADDR}, Argument{a->SP} };
		break;
	}
	case 0xF5:
		return Instruction{ PUSH, 4, Argument{a->AF}, Argument{NONE} };
		break;
	case 0xC5:
		return Instruction{ PUSH, 4, Argument{a->BC}, Argument{NONE} };
		break;
	case 0xD5:
		return Instruction{ PUSH, 4, Argument{a->DE}, Argument{NONE} };
		break;
	case 0xE5:
		return Instruction{ PUSH, 4, Argument{a->HL}, Argument{NONE} };
		break;
	case 0xF1:
		return Instruction{ POP, 3, Argument{a->AF}, Argument{NONE} };
		break;
	case 0xC1:
		return Instruction{ POP, 3, Argument{a->BC}, Argument{NONE} };
		break;
	case 0xD1:
		return Instruction{ POP, 3, Argument{a->DE}, Argument{NONE} };
		break;
	case 0xE1:
		return Instruction{ POP, 3, Argument{a->HL}, Argument{NONE} };
		break;
	case 0x87:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->A} };
		break;
	case 0x80:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->B} };
		break;
	case 0x81:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->C} };
		break;
	case 0x82:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->D} };
		break;
	case 0x83:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->E} };
		break;
	case 0x84:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->H} };
		break;
	case 0x85:
		return Instruction{ ADD, 1, Argument{a->A}, Argument{a->L} };
		break;
	case 0x86:
		return Instruction{ ADD, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0xC6:
		return Instruction{ ADD, 2, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x8F:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->A} };
		break;
	case 0x88:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->B} };
		break;
	case 0x89:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->C} };
		break;
	case 0x8A:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->D} };
		break;
	case 0x8B:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->E} };
		break;
	case 0x8C:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->H} };
		break;
	case 0x8D:
		return Instruction{ ADC, 1, Argument{a->A}, Argument{a->L} };
		break;
	case 0x8E:
		return Instruction{ ADC, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0xCE:
		return Instruction{ ADC, 2, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x97:
		return Instruction{ SUB, 1, Argument{a->A} };
		break;
	case 0x90:
		return Instruction{ SUB, 1, Argument{a->B} };
		break;
	case 0x91:
		return Instruction{ SUB, 1, Argument{a->C} };
		break;
	case 0x92:
		return Instruction{ SUB, 1, Argument{a->D} };
		break;
	case 0x93:
		return Instruction{ SUB, 1, Argument{a->E} };
		break;
	case 0x94:
		return Instruction{ SUB, 1, Argument{a->H} };
		break;
	case 0x95:
		return Instruction{ SUB, 1, Argument{a->L} };
		break;
	case 0x96:
		return Instruction{ SUB, 2, Argument{a->HL, ADDR} };
		break;
	case 0xD6:
		return Instruction{ SUB, 2, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x9F:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->A} };
		break;
	case 0x98:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->B} };
		break;
	case 0x99:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->C} };
		break;
	case 0x9A:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->D} };
		break;
	case 0x9B:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->E} };
		break;
	case 0x9C:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->H} };
		break;
	case 0x9D:
		return Instruction{ SBC, 1, Argument{a->A}, Argument{a->L} };
		break;
	case 0x9E:
		return Instruction{ SBC, 2, Argument{a->A}, Argument{a->HL, ADDR} };
		break;
	case 0xA7:
		return Instruction{ AND, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0xA0:
		return Instruction{ AND, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0xA1:
		return Instruction{ AND, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0xA2:
		return Instruction{ AND, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0xA3:
		return Instruction{ AND, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0xA4:
		return Instruction{ AND, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0xA5:
		return Instruction{ AND, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0xA6:
		return Instruction{ AND, 2, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0xE6:
		return Instruction{ AND, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0xB7:
		return Instruction{ OR, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0xB0:
		return Instruction{ OR, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0xB1:
		return Instruction{ OR, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0xB2:
		return Instruction{ OR, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0xB3:
		return Instruction{ OR, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0xB4:
		return Instruction{ OR, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0xB5:
		return Instruction{ OR, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0xB6:
		return Instruction{ OR, 2, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0xF6:
		return Instruction{ OR, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0xAF:
		return Instruction{ XOR, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0xA8:
		return Instruction{ XOR, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0xA9:
		return Instruction{ XOR, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0xAA:
		return Instruction{ XOR, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0xAB:
		return Instruction{ XOR, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0xAC:
		return Instruction{ XOR, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0xAD:
		return Instruction{ XOR, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0xAE:
		return Instruction{ XOR, 2, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0xEE:
		return Instruction{ XOR, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0xBF:
		return Instruction{ CP, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0xB8:
		return Instruction{ CP, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0xB9:
		return Instruction{ CP, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0xBA:
		return Instruction{ CP, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0xBB:
		return Instruction{ CP, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0xBC:
		return Instruction{ CP, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0xBD:
		return Instruction{ CP, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0xBE:
		return Instruction{ CP, 2, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0xFE:
		return Instruction{ CP, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0x3C:
		return Instruction{ INC, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0x04:
		return Instruction{ INC, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0x0C:
		return Instruction{ INC, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0x14:
		return Instruction{ INC, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0x1C:
		return Instruction{ INC, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0x24:
		return Instruction{ INC, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0x2C:
		return Instruction{ INC, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0x34:
		return Instruction{ INC, 3, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0x3D:
		return Instruction{ DEC, 1, Argument{a->A}, Argument{NONE} };
		break;
	case 0x05:
		return Instruction{ DEC, 1, Argument{a->B}, Argument{NONE} };
		break;
	case 0x0D:
		return Instruction{ DEC, 1, Argument{a->C}, Argument{NONE} };
		break;
	case 0x15:
		return Instruction{ DEC, 1, Argument{a->D}, Argument{NONE} };
		break;
	case 0x1D:
		return Instruction{ DEC, 1, Argument{a->E}, Argument{NONE} };
		break;
	case 0x25:
		return Instruction{ DEC, 1, Argument{a->H}, Argument{NONE} };
		break;
	case 0x2D:
		return Instruction{ DEC, 1, Argument{a->L}, Argument{NONE} };
		break;
	case 0x35:
		return Instruction{ DEC, 3, Argument{a->HL, ADDR}, Argument{NONE} };
		break;
	case 0x09:
		return Instruction{ ADD, 2, Argument{a->HL}, Argument{a->BC} };
		break;
	case 0x19:
		return Instruction{ ADD, 2, Argument{a->HL}, Argument{a->DE} };
		break;
	case 0x29:
		return Instruction{ ADD, 2, Argument{a->HL}, Argument{a->HL} };
		break;
	case 0x39:
		return Instruction{ ADD, 2, Argument{a->HL}, Argument{a->SP} };
		break;
	case 0xE8:
		return Instruction{ ADD, 4, Argument{a->SP}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	case 0x03:
		return Instruction{ INC, 2, Argument{a->BC}, Argument{NONE} };
		break;
	case 0x13:
		return Instruction{ INC, 2, Argument{a->DE}, Argument{NONE} };
		break;
	case 0x23:
		return Instruction{ INC, 2, Argument{a->HL}, Argument{NONE} };
		break;
	case 0x33:
		return Instruction{ INC, 2, Argument{a->SP}, Argument{NONE} };
		break;
	case 0x0B:
		return Instruction{ DEC, 2, Argument{a->BC}, Argument{NONE} };
		break;
	case 0x1B:
		return Instruction{ DEC, 2, Argument{a->DE}, Argument{NONE} };
		break;
	case 0x2B:
		return Instruction{ DEC, 2, Argument{a->HL}, Argument{NONE} };
		break;
	case 0x3B:
		return Instruction{ DEC, 2, Argument{a->SP}, Argument{NONE} };
		break;
	case 0xCB:{
		opcode = a->ram[address + 1].to_ulong();
		switch(opcode)
		{
		default:
			return Instruction{ ERR, 1, Argument{NONE}, Argument{NONE} };
			break;
		case 0x37:
			return Instruction{ SWAP, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x30:
			return Instruction{ SWAP, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x31:
			return Instruction{ SWAP, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x32:
			return Instruction{ SWAP, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x33:
			return Instruction{ SWAP, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x34:
			return Instruction{ SWAP, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x35:
			return Instruction{ SWAP, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x36:
			return Instruction{ SWAP, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x07:
			return Instruction{ RLC, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x00:
			return Instruction{ RLC, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x01:
			return Instruction{ RLC, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x02:
			return Instruction{ RLC, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x03:
			return Instruction{ RLC, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x04:
			return Instruction{ RLC, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x05:
			return Instruction{ RLC, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x06:
			return Instruction{ RLC, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x17:
			return Instruction{ RL, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x10:
			return Instruction{ RL, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x11:
			return Instruction{ RL, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x12:
			return Instruction{ RL, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x13:
			return Instruction{ RL, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x14:
			return Instruction{ RL, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x15:
			return Instruction{ RL, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x16:
			return Instruction{ RL, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x0F:
			return Instruction{ RRC, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x08:
			return Instruction{ RRC, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x09:
			return Instruction{ RRC, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x0A:
			return Instruction{ RRC, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x0B:
			return Instruction{ RRC, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x0C:
			return Instruction{ RRC, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x0D:
			return Instruction{ RRC, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x0E:
			return Instruction{ RRC, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x1F:
			return Instruction{ RR, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x18:
			return Instruction{ RR, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x19:
			return Instruction{ RR, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x1A:
			return Instruction{ RR, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x1B:
			return Instruction{ RR, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x1C:
			return Instruction{ RR, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x1D:
			return Instruction{ RR, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x1E:
			return Instruction{ RR, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x27:
			return Instruction{ SLA, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x20:
			return Instruction{ SLA, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x21:
			return Instruction{ SLA, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x22:
			return Instruction{ SLA, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x23:
			return Instruction{ SLA, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x24:
			return Instruction{ SLA, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x25:
			return Instruction{ SLA, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x26:
			return Instruction{ SLA, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x2F:
			return Instruction{ SRA, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x28:
			return Instruction{ SRA, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x29:
			return Instruction{ SRA, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x2A:
			return Instruction{ SRA, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x2B:
			return Instruction{ SRA, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x2C:
			return Instruction{ SRA, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x2D:
			return Instruction{ SRA, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x2E:
			return Instruction{ SRA, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x3F:
			return Instruction{ SRL, 2, Argument{a->A}, Argument{NONE} };
			break;
		case 0x38:
			return Instruction{ SRL, 2, Argument{a->B}, Argument{NONE} };
			break;
		case 0x39:
			return Instruction{ SRL, 2, Argument{a->C}, Argument{NONE} };
			break;
		case 0x3A:
			return Instruction{ SRL, 2, Argument{a->D}, Argument{NONE} };
			break;
		case 0x3B:
			return Instruction{ SRL, 2, Argument{a->E}, Argument{NONE} };
			break;
		case 0x3C:
			return Instruction{ SRL, 2, Argument{a->H}, Argument{NONE} };
			break;
		case 0x3D:
			return Instruction{ SRL, 2, Argument{a->L}, Argument{NONE} };
			break;
		case 0x3E:
			return Instruction{ SRL, 4, Argument{a->HL, ADDR}, Argument{NONE} };
			break;
		case 0x47:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->A} };
			break;
		case 0x40:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->B} };
			break;
		case 0x41:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->C} };
			break;
		case 0x42:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->D} };
			break;
		case 0x43:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->E} };
			break;
		case 0x44:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->H} };
			break;
		case 0x45:
			return Instruction{ BIT, 2, Argument{0, IMM}, Argument{a->L} };
			break;
		case 0x46:
			return Instruction{ BIT, 4, Argument{0, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x4F:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->A} };
			break;
		case 0x48:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->B} };
			break;
		case 0x49:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->C} };
			break;
		case 0x4A:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->D} };
			break;
		case 0x4B:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->E} };
			break;
		case 0x4C:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->H} };
			break;
		case 0x4D:
			return Instruction{ BIT, 2, Argument{1, IMM}, Argument{a->L} };
			break;
		case 0x4E:
			return Instruction{ BIT, 4, Argument{1, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x57:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->A} };
			break;
		case 0x50:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->B} };
			break;
		case 0x51:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->C} };
			break;
		case 0x52:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->D} };
			break;
		case 0x53:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->E} };
			break;
		case 0x54:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->H} };
			break;
		case 0x55:
			return Instruction{ BIT, 2, Argument{2, IMM}, Argument{a->L} };
			break;
		case 0x56:
			return Instruction{ BIT, 4, Argument{2, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x5F:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->A} };
			break;
		case 0x58:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->B} };
			break;
		case 0x59:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->C} };
			break;
		case 0x5A:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->D} };
			break;
		case 0x5B:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->E} };
			break;
		case 0x5C:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->H} };
			break;
		case 0x5D:
			return Instruction{ BIT, 2, Argument{3, IMM}, Argument{a->L} };
			break;
		case 0x5E:
			return Instruction{ BIT, 4, Argument{3, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x67:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->A} };
			break;
		case 0x60:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->B} };
			break;
		case 0x61:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->C} };
			break;
		case 0x62:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->D} };
			break;
		case 0x63:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->E} };
			break;
		case 0x64:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->H} };
			break;
		case 0x65:
			return Instruction{ BIT, 2, Argument{4, IMM}, Argument{a->L} };
			break;
		case 0x66:
			return Instruction{ BIT, 4, Argument{4, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x6F:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->A} };
			break;
		case 0x68:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->B} };
			break;
		case 0x69:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->C} };
			break;
		case 0x6A:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->D} };
			break;
		case 0x6B:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->E} };
			break;
		case 0x6C:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->H} };
			break;
		case 0x6D:
			return Instruction{ BIT, 2, Argument{5, IMM}, Argument{a->L} };
			break;
		case 0x6E:
			return Instruction{ BIT, 4, Argument{5, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x77:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->A} };
			break;
		case 0x70:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->B} };
			break;
		case 0x71:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->C} };
			break;
		case 0x72:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->D} };
			break;
		case 0x73:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->E} };
			break;
		case 0x74:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->H} };
			break;
		case 0x75:
			return Instruction{ BIT, 2, Argument{6, IMM}, Argument{a->L} };
			break;
		case 0x76:
			return Instruction{ BIT, 4, Argument{6, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x7F:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->A} };
			break;
		case 0x78:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->B} };
			break;
		case 0x79:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->C} };
			break;
		case 0x7A:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->D} };
			break;
		case 0x7B:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->E} };
			break;
		case 0x7C:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->H} };
			break;
		case 0x7D:
			return Instruction{ BIT, 2, Argument{7, IMM}, Argument{a->L} };
			break;
		case 0x7E:
			return Instruction{ BIT, 4, Argument{7, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x87:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->A} };
			break;
		case 0x80:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->B} };
			break;
		case 0x81:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->C} };
			break;
		case 0x82:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->D} };
			break;
		case 0x83:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->E} };
			break;
		case 0x84:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->H} };
			break;
		case 0x85:
			return Instruction{ RES, 2, Argument{0, IMM}, Argument{a->L} };
			break;
		case 0x86:
			return Instruction{ RES, 4, Argument{0, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x8F:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->A} };
			break;
		case 0x88:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->B} };
			break;
		case 0x89:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->C} };
			break;
		case 0x8A:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->D} };
			break;
		case 0x8B:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->E} };
			break;
		case 0x8C:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->H} };
			break;
		case 0x8D:
			return Instruction{ RES, 2, Argument{1, IMM}, Argument{a->L} };
			break;
		case 0x8E:
			return Instruction{ RES, 4, Argument{1, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x97:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->A} };
			break;
		case 0x90:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->B} };
			break;
		case 0x91:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->C} };
			break;
		case 0x92:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->D} };
			break;
		case 0x93:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->E} };
			break;
		case 0x94:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->H} };
			break;
		case 0x95:
			return Instruction{ RES, 2, Argument{2, IMM}, Argument{a->L} };
			break;
		case 0x96:
			return Instruction{ RES, 4, Argument{2, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0x9F:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->A} };
			break;
		case 0x98:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->B} };
			break;
		case 0x99:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->C} };
			break;
		case 0x9A:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->D} };
			break;
		case 0x9B:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->E} };
			break;
		case 0x9C:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->H} };
			break;
		case 0x9D:
			return Instruction{ RES, 2, Argument{3, IMM}, Argument{a->L} };
			break;
		case 0x9E:
			return Instruction{ RES, 4, Argument{3, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xA7:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->A} };
			break;
		case 0xA0:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->B} };
			break;
		case 0xA1:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->C} };
			break;
		case 0xA2:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->D} };
			break;
		case 0xA3:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->E} };
			break;
		case 0xA4:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->H} };
			break;
		case 0xA5:
			return Instruction{ RES, 2, Argument{4, IMM}, Argument{a->L} };
			break;
		case 0xA6:
			return Instruction{ RES, 4, Argument{4, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xAF:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->A} };
			break;
		case 0xA8:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->B} };
			break;
		case 0xA9:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->C} };
			break;
		case 0xAA:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->D} };
			break;
		case 0xAB:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->E} };
			break;
		case 0xAC:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->H} };
			break;
		case 0xAD:
			return Instruction{ RES, 2, Argument{5, IMM}, Argument{a->L} };
			break;
		case 0xAE:
			return Instruction{ RES, 4, Argument{5, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xB7:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->A} };
			break;
		case 0xB0:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->B} };
			break;
		case 0xB1:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->C} };
			break;
		case 0xB2:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->D} };
			break;
		case 0xB3:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->E} };
			break;
		case 0xB4:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->H} };
			break;
		case 0xB5:
			return Instruction{ RES, 2, Argument{6, IMM}, Argument{a->L} };
			break;
		case 0xB6:
			return Instruction{ RES, 4, Argument{6, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xBF:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->A} };
			break;
		case 0xB8:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->B} };
			break;
		case 0xB9:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->C} };
			break;
		case 0xBA:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->D} };
			break;
		case 0xBB:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->E} };
			break;
		case 0xBC:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->H} };
			break;
		case 0xBD:
			return Instruction{ RES, 2, Argument{7, IMM}, Argument{a->L} };
			break;
		case 0xBE:
			return Instruction{ RES, 4, Argument{7, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xC7:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->A} };
			break;
		case 0xC0:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->B} };
			break;
		case 0xC1:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->C} };
			break;
		case 0xC2:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->D} };
			break;
		case 0xC3:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->E} };
			break;
		case 0xC4:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->H} };
			break;
		case 0xC5:
			return Instruction{ SET, 2, Argument{0, IMM}, Argument{a->L} };
			break;
		case 0xC6:
			return Instruction{ SET, 4, Argument{0, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xCF:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->A} };
			break;
		case 0xC8:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->B} };
			break;
		case 0xC9:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->C} };
			break;
		case 0xCA:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->D} };
			break;
		case 0xCB:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->E} };
			break;
		case 0xCC:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->H} };
			break;
		case 0xCD:
			return Instruction{ SET, 2, Argument{1, IMM}, Argument{a->L} };
			break;
		case 0xCE:
			return Instruction{ SET, 4, Argument{1, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xD7:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->A} };
			break;
		case 0xD0:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->B} };
			break;
		case 0xD1:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->C} };
			break;
		case 0xD2:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->D} };
			break;
		case 0xD3:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->E} };
			break;
		case 0xD4:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->H} };
			break;
		case 0xD5:
			return Instruction{ SET, 2, Argument{2, IMM}, Argument{a->L} };
			break;
		case 0xD6:
			return Instruction{ SET, 4, Argument{2, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xDF:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->A} };
			break;
		case 0xD8:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->B} };
			break;
		case 0xD9:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->C} };
			break;
		case 0xDA:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->D} };
			break;
		case 0xDB:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->E} };
			break;
		case 0xDC:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->H} };
			break;
		case 0xDD:
			return Instruction{ SET, 2, Argument{3, IMM}, Argument{a->L} };
			break;
		case 0xDE:
			return Instruction{ SET, 4, Argument{3, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xE7:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->A} };
			break;
		case 0xE0:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->B} };
			break;
		case 0xE1:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->C} };
			break;
		case 0xE2:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->D} };
			break;
		case 0xE3:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->E} };
			break;
		case 0xE4:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->H} };
			break;
		case 0xE5:
			return Instruction{ SET, 2, Argument{4, IMM}, Argument{a->L} };
			break;
		case 0xE6:
			return Instruction{ SET, 4, Argument{4, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xEF:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->A} };
			break;
		case 0xE8:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->B} };
			break;
		case 0xE9:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->C} };
			break;
		case 0xEA:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->D} };
			break;
		case 0xEB:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->E} };
			break;
		case 0xEC:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->H} };
			break;
		case 0xED:
			return Instruction{ SET, 2, Argument{5, IMM}, Argument{a->L} };
			break;
		case 0xEE:
			return Instruction{ SET, 4, Argument{5, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xF7:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->A} };
			break;
		case 0xF0:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->B} };
			break;
		case 0xF1:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->C} };
			break;
		case 0xF2:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->D} };
			break;
		case 0xF3:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->E} };
			break;
		case 0xF4:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->H} };
			break;
		case 0xF5:
			return Instruction{ SET, 2, Argument{6, IMM}, Argument{a->L} };
			break;
		case 0xF6:
			return Instruction{ SET, 4, Argument{6, IMM}, Argument{a->HL, ADDR} };
			break;
		case 0xFF:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->A} };
			break;
		case 0xF8:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->B} };
			break;
		case 0xF9:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->C} };
			break;
		case 0xFA:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->D} };
			break;
		case 0xFB:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->E} };
			break;
		case 0xFC:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->H} };
			break;
		case 0xFD:
			return Instruction{ SET, 2, Argument{7, IMM}, Argument{a->L} };
			break;
		case 0xFE:
			return Instruction{ SET, 4, Argument{7, IMM}, Argument{a->HL, ADDR} };
			break;
		}
		break;
	}
	case 0x27:
		return Instruction{ DAA, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x2F:
		return Instruction{ CPL, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x3F:
		return Instruction{ CCF, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x37:
		return Instruction{ SCF, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x00:
		return Instruction{ NOP, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x76:
		return Instruction{ HALT, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x10: {
		if (a->ram[address + 1].to_ulong() == 0x00)
			return Instruction{ STOP, 1, Argument{NONE}, Argument{NONE} };
		else //Dunno, it's not the right way but seems to work in some games
			return Instruction{ STOP, 1, Argument{NONE}, Argument{NONE} };
		break;
	}
	case 0xF3:
		return Instruction{ DI, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0xFB:
		return Instruction{ EI, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x07:
		return Instruction{ RLCA, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x17:
		return Instruction{ RLA, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x0F:
		return Instruction{ RRCA, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0x1F:
		return Instruction{ RRA, 1, Argument{NONE}, Argument{NONE} };
		break;
	case 0xC3: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ JP, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xC2: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ JPNZ, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xCA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ JPZ, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xD2: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ JPNC, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xDA: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ JPC, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xE9:
		return Instruction{ JP, 1, Argument{a->HL}, Argument{NONE} };
		break;
	case 0x18:
		return Instruction{ JR, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0x20:
		return Instruction{ JRNZ, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0x28:
		return Instruction{ JRZ, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0x30:
		return Instruction{ JRNC, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0x38:
		return Instruction{ JRC, 2, Argument{a->ram[address + 1].to_ulong(), IMM}, Argument{NONE} };
		break;
	case 0xCD: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ CALL, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xC4: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ CALLNZ, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xCC: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ CALLZ, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xD4: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ CALLNC, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xDC: {
		word imm = (word)(a->ram[address + 2].to_string() + a->ram[address + 1].to_string());
		return Instruction{ CALLC, 3, Argument{imm.to_ulong(), W_IMM}, Argument{NONE} };
		break;
	}
	case 0xC7:
		return Instruction{ RST, 8, Argument{0x00, IMM}, Argument{NONE} };
		break;
	case 0xCF:
		return Instruction{ RST, 8, Argument{0x08, IMM}, Argument{NONE} };
		break;
	case 0xD7:
		return Instruction{ RST, 8, Argument{0x10, IMM}, Argument{NONE} };
		break;
	case 0xDF:
		return Instruction{ RST, 8, Argument{0x18, IMM}, Argument{NONE} };
		break;
	case 0xE7:
		return Instruction{ RST, 8, Argument{0x20, IMM}, Argument{NONE} };
		break;
	case 0xEF:
		return Instruction{ RST, 8, Argument{0x28, IMM}, Argument{NONE} };
		break;
	case 0xF7:
		return Instruction{ RST, 8, Argument{0x30, IMM}, Argument{NONE} };
		break;
	case 0xFF:
		return Instruction{ RST, 8, Argument{0x38, IMM}, Argument{NONE} };
		break;
	case 0xC9:
		return Instruction{ RET, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xC0:
		return Instruction{ RETNZ, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xC8:
		return Instruction{ RETZ, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xD0:
		return Instruction{ RETNC, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xD8:
		return Instruction{ RETC, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xD9:
		return Instruction{ RETI, 2, Argument{NONE}, Argument{NONE} };
		break;
	case 0xDE:
		return Instruction{ SBC, 2, Argument{a->A}, Argument{a->ram[address + 1].to_ulong(), IMM} };
		break;
	}
}

unsigned short Instruction::length()
{
	//The byte used to define the command
	unsigned short len = 1;

	//Additional byte for commands defined with 2 bytes
	switch (cmd)
	{
	case SWAP:
	case STOP:
	case RLC:
	case RL:
	case RRC:
	case RR:
	case SLA:
	case SRA:
	case SRL:
	case BIT:
	case SET:
	case RES:
		len++;
		break;
	}

	//Don't consider immediate values for the RST command
	//since different opcodes are simulated using an immediate
	//value
	if (cmd == BIT || cmd == SET || cmd == RES || cmd == RST)
		return len;

	//Additional bytes for immediate values
	if (arg1.type == IMM)
		len++;
	if (arg2.type == IMM)
		len++;
	if (arg1.type == W_IMM)
		len += 2;
	if (arg2.type == W_IMM)
		len += 2;

	return len;
}