#include "architecture.hpp"
#include "screen.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <SFML/Graphics.hpp>
using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::thread;
using std::mutex;

#define ROM_FILENAME "picross.gbc"

#define LCD_W 160
#define LCD_H 144

#define CLK_TIME std::chrono::nanoseconds(0) //1.05 MHz, 954
#define DOUBLE_CLK_TIME std::chrono::nanoseconds(0) //2.10 Mhz, 477
#define LCD_HLINE_TIME std::chrono::microseconds(109)
#define LCD_MODE_TIME std::chrono::microseconds(55) //roughly half

bool debugger = true;

//To ensure synchronization between threads
//that try to access the architecture simultaneously
mutex arch_mutex;

void architecture_main(Architecture* arch)
{
	string buffer;

	if (!arch->cart.loaded)
		warning("Running with no ROM loaded");

	//Don't execute instructions for one time after some
	//debugger commands
	bool wait = false;

	while (true)
	{
		if (wait)
			wait = false;
		else
		{
			arch->step(debugger);

			//Instruction cycle
			if (arch->doubleSpeed)
				std::this_thread::sleep_for(DOUBLE_CLK_TIME);
			else
				std::this_thread::sleep_for(CLK_TIME); 
		}

		if (debugger)
		{
			cout << "\n> ";
			getline(cin, buffer);


			switch (buffer[0])
			{
			case 'b': {
				//Breakpoint
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					data breakpoint = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
					data next = arch->PC.to_ulong();
					debugger = false;
					while (next != breakpoint && !debugger)
					{
						next = arch->step(debugger);

						/*if (arch->doubleSpeed)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);*/
					}
					debugger = true;
				}
				break;
			}
			case 'c': {
				debugger = false;
				break;
			}
			case 'n': {
				//Make n steps
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					unsigned long steps = strtoul(buffer.c_str(), NULL, 0);
					cout << endl;
					debugger = false;
					for (unsigned int i = 0; i < steps && !debugger; i++)
					{
						arch->step(debugger);

						/*if (arch->doubleSpeed)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);*/

						if (i % 10000 == 0)
						{
							warning(to_string(steps - i) + " instructions left");
						}
					}
					cout << endl;
					debugger = true;
				}
				break;
			}
			case 'o': {
				//Step out
				data next = arch->PC.to_ulong();
				Command cmd = disasm(next).cmd;
				debugger = false;
				while (cmd != RET && cmd != RETNZ && cmd != RETZ && cmd != RETNC && cmd != RETC && cmd != RETI && !debugger)
				{
					next = arch->step(debugger);

					/*if (arch->doubleSpeed)
						std::this_thread::sleep_for(DOUBLE_CLK_TIME);
					else
						std::this_thread::sleep_for(CLK_TIME);*/

					cmd = disasm(next).cmd;
				}
				debugger = true;
				break;
			}
			case 'm': {
				//Print memory
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					unsigned long address = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
					arch->print_ram(address, 5);
					wait = true;
				}
				break;
			}
			case 'd': {
				//Disassemble instructions
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					unsigned long address = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
					arch->print_instructions(address, 5);
					wait = true;
				}
				break;
			}
			case 'r': {
				//Dump ram
				arch->dump_ram();
				wait = true;
				break;
			}
			case 'v': {
				//Dump video ram
				arch->dump_video();
				wait = true;
				break;
			}
			case 'p': {
				//Print palettes
				arch->print_palettes();
				wait = true;
				break;
			}
			case 's': {
				//Show current banks
				arch->print_banks();
				wait = true;
			}
			}
		}
	}
}

void buttons(Architecture* arch)
{
	//For button press interrupts
	bool pressedBefore = false;
	bool pressedAfter = false;

	bool right, left, up, down, A, B, select, start;

	//Input
	while (true)
	{
		right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		A = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		B = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
		select = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		start = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		if (!arch->ram[P1][4]) //Read DPAD
		{
			arch->ram[P1][0] = !right;
			arch->ram[P1][1] = !left;
			arch->ram[P1][2] = !up;
			arch->ram[P1][3] = !down;
		}
		else if (!arch->ram[P1][5]) //Read A,B,Select;Start
		{
			arch->ram[P1][0] = !A;
			arch->ram[P1][1] = !B;
			arch->ram[P1][2] = !select;
			arch->ram[P1][3] = !start;
		}
		pressedAfter = right || left || up || down || A || B || select || start;

		if (!pressedBefore && pressedAfter)
			arch->ram[IF][4] = true;

		pressedBefore = pressedAfter;

		//Instruction cycle
		/*if (arch->doubleSpeed)
			std::this_thread::sleep_for(DOUBLE_CLK_TIME);
		else
			std::this_thread::sleep_for(CLK_TIME);*/
	}
}

void LCDtiming(Architecture* arch)
{
	unsigned short Yline;
	bool hblank = false;
	arch->ram[STAT][1] = 1;
	arch->ram[STAT][0] = 0;

	while (true)
	{
		std::this_thread::sleep_for(LCD_MODE_TIME);

		//Two mode times make a horizontal line
		if (arch->screenOn)
		{
			if (hblank && !debugger)
			{
				Yline = arch->ram[LY].to_ulong();

				Yline++;
				if (Yline >= 154)
					Yline = 0;

				arch->ram[LY] = Yline;

				arch->ram[STAT][1] = 0;
				if (Yline >= 144)
				{
					arch->ram[STAT][0] = 1;

					//Vblank interrupt
					if (Yline == 144)
					{
						arch->ram[IF][0] = true;

						//Also as LCDC interrupt
						if(arch->ram[STAT][4])
							arch->ram[IF][1] = true;
					}
				}
				else
				{
					//Hblank interrupt
					if (arch->ram[STAT][3])
						arch->ram[IF][1] = true;

					arch->ram[STAT][0] = 0;
				}

				//Match of LYC
				if (arch->ram[LY] == arch->ram[LYC])
				{
					arch->ram[STAT][2] = true;

					if (arch->ram[STAT][6]) //Compare interrupt enabled
						arch->ram[IF][1] = true;
				}
				else
				{
					arch->ram[STAT][2] = false;
				}
			}
			else if (!debugger)
			{
				arch->ram[STAT][1] = 1;
				arch->ram[STAT][0] = 0;

				//Mode 2 interrupt
				if(arch->ram[STAT][5])
					arch->ram[IF][1] = true;

				//Ignore state 3, in the emulator we can
				//always write to ram
			}
		}
		else
		{
			arch->ram[LY] = 0;
			arch->ram[STAT][1] = 0;
			arch->ram[STAT][0] = 0;
		}

		hblank = !hblank;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(LCD_W, LCD_H), "ERapGBC");
	window.setSize(sf::Vector2u(LCD_W*3, LCD_H*3));
	window.setVerticalSyncEnabled(true);

	//Create architecture and load rom
	Architecture* arch = Architecture::instance();
	arch->loadROM(ROM_FILENAME);

	//Start the "CPU"
	thread processing(architecture_main, arch);
	processing.detach();

	//Start checking button presses
	thread buttons_check(buttons, arch);
	buttons_check.detach();

	//Start checking button presses
	thread lcd(LCDtiming, arch);
	lcd.detach();
	
	sf::Texture tilesTex[32 * 32];
	sf::Sprite tiles[32 * 32];

	//Window loop
	unsigned int index;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		drawScreen(arch, window, tilesTex);

		for (int y = 0; y < 32; y++)
		{
			for (int x = 0; x < 32; x++)
			{
				index = y * 32 + x;
				tiles[index].setTexture(tilesTex[index]);
				tiles[index].setPosition(x * 8, y * 8);
				window.draw(tiles[index]);
			}
		}

		window.display();
	}

	return 0;
}

