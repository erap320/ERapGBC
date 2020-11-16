#include <windows.h>

void Color(int c)
{	
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, c);
}

void Color(char c)
{
	int code;
	switch (c)
	{
	case 'r':
		code = 0x0C;
		break;
	case 'y':
		code = 0x0E;
		break;
	case 'g':
		code = 0x0A;
		break;
	case 'w':
	default:
		code = 0x7;
		break;
	}
	Color(code);
}