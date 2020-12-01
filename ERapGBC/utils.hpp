#pragma once
#include <string>
#include <mutex>
using std::string;
using std::mutex;

//Mutex for console output
extern mutex out_mutex;

//Formatted output functions
void error(string str);
void warning(string str);
void debug(string str);

//Convert a number to a hex string, considered to be
//the size of a word, unless "byte" is true
string to_hex(unsigned long i, bool byte = false);