#include "utils.hpp"
#include "win_color.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using std::cout;
using std::endl;

mutex out_mutex;

void error(string str) 
{
	out_mutex.lock();
	Color('r');
	cout << "E> " << str << endl;
	Color('w');
	out_mutex.unlock();
}

void warning(string str)
{
	out_mutex.lock();
	Color('y');
	cout << "W> " << str << endl;
	Color('w');
	out_mutex.unlock();
}

void debug(string str)
{
	out_mutex.lock();
	cout << "D> " << str << endl;
	out_mutex.unlock();
}

string to_hex(unsigned long i, bool byte)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(byte ? 2 : 4)
		<< std::hex << i;
	return stream.str();
}