#include "utils.hpp"
#include "win_color.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using std::cout;
using std::endl;

void error(string str) 
{
	Color('r');
	cout << "E> " << str << endl;
	Color('w');
}

void warning(string str)
{
	Color('y');
	cout << "W> " << str << endl;
	Color('w');
}

void debug(string str)
{
	cout << "D> " << str << endl;
}

string to_hex(unsigned long i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(4)
		<< std::hex << i;
	return stream.str();
}