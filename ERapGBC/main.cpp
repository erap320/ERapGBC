#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	arch->exec(LD, Argument{ arch->BC }, Argument{ 0xe2f0, W_IMM });

	arch->print_registers();

	arch->exec(SWAP, Argument{ arch->B });

	arch->print_registers();

	return 0;
}