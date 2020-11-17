#include "architecture.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Architecture* arch = Architecture::instance();

	arch->exec(RL, Argument{arch->BC});

	return 0;
}