#pragma once
#include <bitset>
#include <string>
#include <mutex>
using std::string;
using std::mutex;
using std::bitset;

//Mutex for console output
extern mutex out_mutex;

//Formatted output functions
void error(string str, bitset<16> PC = 0);
void warning(string str, bitset<16> PC = 0);
void debug(string str, bitset<16> PC = 0);

//Convert a number to a hex string, considered to be
//the size of a word, unless "byte" is true
string to_hex(unsigned long i, bool byte = false);