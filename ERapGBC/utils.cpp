#include "utils.hpp"
#include "win_color.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using std::cout;
using std::endl;

mutex out_mutex;

void error(string str, bitset<16> PC)
{
	out_mutex.lock();
	Color('r');
	if(PC == 0)
		cout << "E> " << str << endl;
	else
		cout << "E|" << to_hex(PC.to_ulong()) << "> " << str << endl;
	Color('w');
	out_mutex.unlock();
}

void warning(string str, bitset<16> PC)
{
	out_mutex.lock();
	Color('y');
	if (PC == 0)
		cout << "W> " << str << endl;
	else
		cout << "W|" << to_hex(PC.to_ulong()) << "> " << str << endl;
	Color('w');
	out_mutex.unlock();
}

void debug(string str, bitset<16> PC)
{
	out_mutex.lock();
	Color('g');
	if (PC == 0)
		cout << "D> " << str << endl;
	else
		cout << "D|" << to_hex(PC.to_ulong()) << "> " << str << endl;
	Color('w');
	out_mutex.unlock();
}

void info(string str, bitset<16> PC)
{
	out_mutex.lock();

	if (PC == 0)
		cout << "I> " << str << endl;
	else
		cout << "I|" << to_hex(PC.to_ulong()) << "> " << str << endl;

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