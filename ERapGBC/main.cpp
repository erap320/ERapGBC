#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::cin;
using std::getline;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	string buffer;
	while (true)
	{
		for (int i = 0; i < 10; i++) cout << endl;
		arch->step(true);

		cout << "\n> ";
		getline(cin, buffer);


		switch (buffer[0])
		{
		case 'b': {
			//Breakpoint
			buffer.erase(0, 2);
			if (!buffer.empty())
			{
				unsigned long breakpoint = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
				while (arch->PC.to_ulong() + disasm(arch->PC.to_ulong()).length() != breakpoint)
					arch->step(false);
			}
			break;
		}
		case 'n': {
			//Make n steps
			buffer.erase(0, 2);
			if (!buffer.empty())
			{
				unsigned long steps = strtoul(buffer.c_str(), NULL, 0);
				for (unsigned int i = 0; i < steps; i++)
					arch->step(false);
			}
			break;
		}
		case 'o': {
			//Step out
			while (disasm(arch->PC.to_ulong()).cmd != RET)
				arch->step(false);
		}
		}
	}

	return 0;
}