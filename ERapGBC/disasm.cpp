#include "architecture.hpp"

Instruction disasm(data address, byte* const& ram)
{
	Instruction res = Instruction{ NOP };
	res.setLength(1);
	return res;
}