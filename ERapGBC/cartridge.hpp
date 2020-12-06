#pragma once
#include <string>
#include <fstream>
#include "types.hpp"
using std::string;
using std::ifstream;

//Maximum ROM: 8MB
#define ROM_BANK_SIZE 0x4000
#define CART_ROM_BANKS 512
//Maximum RAM: 32kB
#define RAM_BANK_SIZE 0x2000
#define CART_RAM_BANKS 4

//Data contained in a game cartridge
class Cartridge {
public:
	byte romBanks[CART_ROM_BANKS][ROM_BANK_SIZE];
	byte ramBanks[CART_RAM_BANKS][RAM_BANK_SIZE];

	bool loaded = false;

	bool loadROM(string romFileName)
	{
		//Load rom
		ifstream rom(romFileName, std::ios::in | std::ios::binary);
		if (!rom)
			return false;

		char buffer;
		for (unsigned int bank = 0; bank < CART_ROM_BANKS && !rom.eof(); bank++)
		{
			for (data addr = 0; addr < ROM_BANK_SIZE && !rom.eof(); addr++)
			{
				rom.read(&buffer, 1);
				romBanks[bank][addr] = (byte)buffer;
			}
		}
		rom.close();

		for (unsigned int bank = 0; bank < CART_RAM_BANKS; bank++)
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