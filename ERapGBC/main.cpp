#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	arch->BC = 0xe0f0;

	arch->exec(INC, Argument{ arch->C });
	
	arch->print_registers();

	arch->exec(ADD, Argument{ arch->A }, Argument{ arch->C });

	arch->print_registers();

	arch->exec(SUB, Argument{ 0xf1, IMM });

	arch->print_registers();

	arch->exec(PUSH, Argument{ arch->BC });

	arch->print_stack(5);

	return 0;
}