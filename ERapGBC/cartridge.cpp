#include "cartridge.hpp"
#include "utils.hpp"

void Cartridge::loadCartridgeType(data type) {
	//Load cartidge hardware capabilities
	switch (type) {
	case C_ROM: {
		controller = NO_CONTROLLER;
		hasRom = true;
		break;
	}
	case C_MBC1: {
		controller = MBC1;
		break;
	}
	case C_MBC1_RAM: {
		controller = MBC1;
		hasRam = true;
		break;
	}
	case C_MBC1_RAM_BATT: {
		controller = MBC1;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC2: {
		controller = MBC2;
		break;
	}
	case C_MBC2_BATT: {
		controller = MBC2;
		hasBattery = true;
		break;
	}
	case C_ROM_RAM: {
		controller = NO_CONTROLLER;
		hasRom = true;
		hasRam = true;
		break;
	}
	case C_ROM_RAM_BATT: {
		controller = NO_CONTROLLER;
		hasRom = true;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MMM01: {
		controller = MMM01;
		break;
	}
	case C_MMM01_RAM: {
		controller = MMM01;
		hasRam = true;
		break;
	}
	case C_MMM01_RAM_BATT: {
		controller = MMM01;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC3_TIMER_BATT: {
		controller = MBC3;
		hasTimer = true;
		hasBattery = true;
		break;
	}
	case C_MBC3_TIMER_RAM_BATT: {
		controller = MBC3;
		hasTimer = true;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC3: {
		controller = MBC3;
		break;
	}
	case C_MBC3_RAM: {
		controller = MBC3;
		hasRam = true;
		break;
	}
	case C_MBC3_RAM_BATT: {
		controller = MBC3;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC5: {
		controller = MBC5;
		break;
	}
	case C_MBC5_RAM: {
		controller = MBC5;
		hasRam = 5;
		break;
	}
	case C_MBC5_RAM_BATT: {
		controller = MBC5;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC5_RUMBLE: {
		controller = MBC5;
		hasRumble = true;
		break;
	}
	case C_MBC5_RUMBLE_RAM: {
		controller = MBC5;
		hasRumble = true;
		hasRam = true;
		break;
	}
	case C_MBC5_RUMBLE_RAM_BATT: {
		controller = MBC5;
		hasRumble = true;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_MBC6: {
		controller = MBC6;
		break;
	}
	case C_MBC7_SENSOR_RUMBLE_RAM_BATT: {
		controller = MBC7;
		hasSensor = true;
		hasRumble = true;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_HUC1_RAM_BATT: {
		controller = UNKNOWN_CONTROLLER;
		hasRam = true;
		hasBattery = true;
		break;
	}
	case C_CAMERA:
	case C_TAMA5:
	case C_HUC3: {
		controller = UNKNOWN_CONTROLLER;
		break;
	}
	default:
		error("Unknown cartridge type: " + to_hex(type, true));
		throw ArchitectureException();
		break;
	}
}

void Cartridge::setRomBanksNum(data code) {
	if (code >= 0 && code <= 8) {
		romBanksNum = 2 << code;
	}
	else {
		error("Unsupported ROM banks number code: " + to_hex(code, true));
		throw ArchitectureException();
	}
}

void Cartridge::setRamBanksNum(data code) {
	switch (code) {
	case 0: {
		ramBanksNum = 0;
		break;
	}
	case 2: {
		ramBanksNum = 1;
		break;
	}
	case 3: {
		ramBanksNum = 4;
		break;
	}
	case 4: {
		ramBanksNum = 16;
		break;
	}
	case 5: {
		ramBanksNum = 16;
		break;
	}
	default: {
		error("Unsupported RAM banks number code: " + to_hex(code, true));
		throw ArchitectureException();
	}
	}
}

string Cartridge::getSavefilePath(){
	return directory + "/" + filename + ".sav";
}

bool Cartridge::loadROM(string romFileName)
{
	//Load rom
	if (!std::filesystem::exists(romFileName)) {
		error("ROM " + romFileName + " not found");
		return false;
	}

	ifstream rom(romFileName, std::ios::in | std::ios::binary);
	if (!rom) {
		error("Error while opening ROM file " + romFileName);
		return false;
	}

	filename = std::filesystem::path(romFileName).stem().string();
	directory = std::filesystem::path(romFileName).parent_path().string();

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

	//Check if there is a save file for the rom
	string saveFilePath = getSavefilePath();
	if (std::filesystem::exists(saveFilePath))
	{
		ifstream save(saveFilePath, std::ios::in | std::ios::binary);
		if (!rom) {
			error("Error while save file " + saveFilePath);
			return false;
		}

		for (unsigned int bank = 0; bank < ramBanksNum; bank++)
		{
			for (data addr = 0; addr < RAM_BANK_SIZE; addr++)
			{
				save.read(&buffer, 1);
				ramBanks[bank][addr] = buffer;
			}
		}

		save.close();
	}
	else 
	{
		for (unsigned int bank = 0; bank < ramBanksNum; bank++)
		{
			for (data addr = 0; addr < RAM_BANK_SIZE; addr++)
			{
				ramBanks[bank][addr] = 0xFF;
			}
		}
	}

	ramBanks[0][0x1FFF] = 0x37;

	loaded = true;
	return true;
}