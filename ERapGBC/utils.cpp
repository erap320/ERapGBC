#include "utils.hpp"
#include "win_color.h"
#include <iostream>
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