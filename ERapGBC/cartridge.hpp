#pragma once
#include <string>
#include <fstream>
#include "types.hpp"
#include "exceptions.hpp"
using std::string;
using std::ifstream;

//Maximum ROM: 8MB
#define ROM_BANK_SIZE 0x4000
#define MAX_CART_ROM_BANKS 512
//Maximum RAM: 32kB
#define RAM_BANK_SIZE 0x2000
#define MAX_CART_RAM_BANKS 16

enum CartridgeType {
	C_ROM = 0x00,
	C_MBC1 = 0x01,
	C_MBC1_RAM = 0x02,
	C_MBC1_RAM_BATT = 0x03,
	C_MBC2 = 0x05,
	C_MBC2_BATT = 0x06,
	C_ROM_RAM = 0x08,
	C_ROM_RAM_BATT = 0x09,
	C_MMM01 = 0x0b,
	C_MMM01_RAM = 0x0c,
	C_MMM01_RAM_BATT = 0x0d,
	C_MBC3_TIMER_BATT = 0x0f,
	C_MBC3_TIMER_RAM_BATT = 0x10,
	C_MBC3 = 0x11,
	C_MBC3_RAM = 0x12,
	C_MBC3_RAM_BATT = 0x13,
	C_MBC5 = 0x19,
	C_MBC5_RAM = 0x1a,
	C_MBC5_RAM_BATT = 0x1b,
	C_MBC5_RUMBLE = 0x1c,
	C_MBC5_RUMBLE_RAM = 0x1d,
	C_MBC5_RUMBLE_RAM_BATT = 0x1e,
	C_MBC6 = 0x20,
	C_MBC7_SENSOR_RUMBLE_RAM_BATT = 0x22,
	C_CAMERA = 0xfc,
	C_TAMA5 = 0xdf,
	C_HUC3 = 0xfe,
	C_HUC1_RAM_BATT = 0xFF
};

enum MemoryController {
	UNKNOWN_CONTROLLER = 0,
	MBC1,
	MBC2,
	MBC3,
	MBC5 = 5,
	MBC6,
	MBC7,
	MMM01,
	NO_CONTROLLER
};

const string memory_controller_names[] = { "UNKNOWN", "MBC1", "MBC2", "MBC3", "ERR", "MBC5", "MBC6", "MBC7", "MMM01", "NONE" };

//Data contained in a game cartridge
class Cartridge {
public:
	byte romBanks[MAX_CART_ROM_BANKS][ROM_BANK_SIZE];
	byte ramBanks[MAX_CART_RAM_BANKS][RAM_BANK_SIZE];

	MemoryController controller = UNKNOWN_CONTROLLER;
	bool hasRom = false;
	bool hasRam = false;
	bool hasBattery = false;
	bool hasTimer = false;
	bool hasRumble = false;
	bool hasSensor = false;
	void loadCartridgeType(data type);

	unsigned short romBanksNum = MAX_CART_ROM_BANKS;
	void setRomBanksNum(data code);

	unsigned short ramBanksNum = MAX_CART_RAM_BANKS;
	void setRamBanksNum(data code);

	bool loaded = false;

	bool loadROM(string romFileName)
	{
		//Load rom
		ifstream rom(romFileName, std::ios::in | std::ios::binary);
		if (!rom)
			return false;

		char buffer;

		for (data addr = 0; addr < ROM_BANK_SIZE && !rom.eof(); addr++)
		{
			rom.read(&buffer, 1);
			romBanks[0][addr] = (byte)buffer;
		}

		try {
			loadCartridgeType(romBanks[0][0x0147].to_ulong() & 0xff);
			setRomBanksNum(romBanks[0][0x0148].to_ulong() & 0xff);
			setRamBanksNum(romBanks[0][0x0149].to_ulong() & 0xff);
		}
		catch (const ArchitectureException& e) {
			return false;
		}

		info("Memory controller set to " + memory_controller_names[controller]);

		for (unsigned int bank = 1; bank < romBanksNum && !rom.eof(); bank++)
		{
			for (data addr = 0; addr < ROM_BANK_SIZE && !rom.eof(); addr++)
			{
				rom.read(&buffer, 1);
				romBanks[bank][addr] = (byte)buffer;
			}
		}
		rom.close();

		for (unsigned int bank = 0; bank < ramBanksNum; bank++)
		{
			for (data addr = 0; addr < RAM_BANK_SIZE; addr++)
			{
				ramBanks[bank][addr] = 0xFF;
			}
		}

		ramBanks[0][0x1FFF] = 0x37;

		loaded = true;
		return true;
	}
};