#include "architecture.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "utils.hpp"
using std::ofstream;
using std::cout;
using std::endl;

Architecture* Architecture::singleton = NULL;

void Architecture::dump_ram()
{
	ofstream dump("dump.bin", std::ios::binary);

	if (!dump)
	{
		error("Could not create dump.bin file");
		return;
	}

	uint8_t buffer;
	for (int i = 0; i < RAM_SIZE; i++)
	{
		buffer = ram[i].to_ulong() & 0xff;
		dump.write((char*)(&buffer), sizeof(uint8_t));
	}

	dump.close();
}

void Architecture::print_registers()
{
	cout << "\n################# REGISTERS #################\n";
	cout << "PC:\t" << PC.to_string() << endl;
	cout << "SP:\t" << SP.to_string() << endl;
	cout << "A:\t" << A.to_string() << "\tF:\t" << F.to_string() << endl;
	cout << "B:\t" << B.to_string() << "\tC:\t" << C.to_string() << endl;
	cout << "D:\t" << D.to_string() << "\tE:\t" << E.to_string() << endl;
	cout << "H:\t" << H.to_string() << "\tL:\t" << L.to_string() << endl;
	cout << "Flags: Z:" << Zflag() << " N:" << Nflag() << " H:" << Hflag() << " C:" << Cflag() << endl;
	cout << "---------------------------------------------\n";
}

void Architecture::print_stack(unsigned int rows)
{
	cout << "\n################### STACK ###################\n";
	data addr = SP.to_ulong();

	printf("%#.4x | ", addr);
	cout << ram[addr].to_string() << "  <- SP" << endl;
	for (unsigned int i = 1; i < rows && addr+i <= 0xffff; i++)
	{
		printf("%#.4x | ", addr+i);
		cout << ram[addr + i].to_string() << endl;
	}
	cout << "---------------------------------------------\n";
}