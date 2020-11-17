#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	arch->ram[0x100] = 0x7a;
	arch->ram[0x101] = 0xF0;
	arch->ram[0x102] = 0x00;
	arch->ram[0x103] = 0x7a;

	while (true)
	{
		arch->step(true);

		std::cin.get();
	}

	return 0;
}