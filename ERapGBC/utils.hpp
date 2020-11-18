#pragma once
#include <string>
using std::string;

//Formatted output functions
void error(string str);
void warning(string str);
void debug(string str);

//Convert a number to a hex string
string to_hex(unsigned long i);