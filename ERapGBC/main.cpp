#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	while (true)
	{
		arch->step(true);

		std::cin.get();
	}

	return 0;
}