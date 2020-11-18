#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	arch->print_registers();
	arch->print_instruction(0, disasm(0));
	//std::cin.get();

	while (arch->PC.to_ulong() != 0x9D)
		arch->step(false);

	char read;
	while (true)
	{
		for (int i = 0; i < 50; i++) cout << endl;
		arch->step(true);

		read = std::cin.get();

		switch (read)
		{
		case 'c':
			for (int i = 0; i < 100; i++)
				arch->step(false);
		case 'o':
			while (disasm(arch->PC.to_ulong()).cmd != RET)
				arch->step(false);
		}
	}

	return 0;
}