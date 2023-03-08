#ifndef TYPES_HPP
#define TYPES_HPP

#include <bitset>
using std::bitset;

#define BYTE_SIZE 8
#define WORD_SIZE 16
typedef bitset<BYTE_SIZE> byte;
typedef bitset<WORD_SIZE> word;

//Because it's the default smallest numeric type in which a bitset exports numbers
typedef unsigned long data;

#endif