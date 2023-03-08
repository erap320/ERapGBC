#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <string>
#include <fstream>
#include <filesystem>
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
	string filename = "";
	string directory = "";

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

	string getSavefilePath();
	bool loadROM(string romFileName);
};

#endif