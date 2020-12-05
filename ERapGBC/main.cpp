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

#define CLK_TIME std::chrono::nanoseconds(0) //1.05 MHz
#define DOUBLE_CLK_TIME std::chrono::nanoseconds(0) //2.10 Mhz
#define LCD_VLINE_TIME std::chrono::nanoseconds(0)

//#define CLK_TIME std::chrono::nanoseconds(954) //1.05 MHz
//#define DOUBLE_CLK_TIME std::chrono::nanoseconds(477) //2.10 Mhz
//#define LCD_VLINE_TIME std::chrono::microseconds(109)

#define DEBUGGER true

//To ensure synchronization between threads
//that try to access the architecture simultaneously
mutex arch_mutex;

void architecture_main(Architecture* arch)
{
	//SCREEN TEST
	/*arch_mutex.lock();

	arch->videoBanks[0][0] = 0x7C;
	arch->videoBanks[0][1] = 0x7C;
	arch->videoBanks[0][2] = 0xC6;
	arch->videoBanks[0][3] = 0xC6;
	arch->videoBanks[0][4] = 0xC6;
	arch->videoBanks[0][5] = 0xC6;
	arch->videoBanks[0][6] = 0xFE;
	arch->videoBanks[0][7] = 0xFE;
	arch->videoBanks[0][8] = 0xC6;
	arch->videoBanks[0][9] = 0xC6;
	arch->videoBanks[0][10] = 0xC6;
	arch->videoBanks[0][11] = 0xC6;
	arch->videoBanks[0][12] = 0xC6;
	arch->videoBanks[0][13] = 0xC6;
	arch->videoBanks[0][14] = 0x00;
	arch->videoBanks[0][15] = 0x00;

	arch->colorPalettes[0][3][0] = 0x7F;
	arch->colorPalettes[0][3][1] = 0xFF;
	arch->colorPalettes[1][3][0] = 0x03;
	arch->colorPalettes[1][3][1] = 0xE0;
	arch->colorPalettes[2][3][0] = 0x00;
	arch->colorPalettes[2][3][1] = 0x1F;
	arch->colorPalettes[3][3][0] = 0x7C;
	arch->colorPalettes[3][3][1] = 0x00;
	arch->colorPalettes[0][0][0] = 0x00;
	arch->colorPalettes[0][0][1] = 0x00;

	arch->videoBanks[1][0x1801] = 1;
	arch->videoBanks[1][0x1802] = 2;
	arch->videoBanks[1][0x1803] = 3;

	arch_mutex.unlock();*/

	string buffer;

	if (!arch->cart.loaded)
		warning("Running with no ROM loaded");

	//Don't execute instructions for one time after some
	//debugger commands
	bool wait = false;

	bool debugger = DEBUGGER;

	while (true)
	{
		if (wait)
			wait = false;
		else
		{
			arch_mutex.lock();
			arch->step(debugger);
			arch_mutex.unlock();


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
						arch_mutex.lock();
						next = arch->step(debugger);
						arch_mutex.unlock();

						if (arch->doubleSpeed)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);
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
						arch_mutex.lock();
						arch->step(debugger);
						arch_mutex.unlock();

						if (arch->doubleSpeed)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);
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
					arch_mutex.lock();
					next = arch->step(debugger);
					arch_mutex.unlock();

					if (arch->doubleSpeed)
						std::this_thread::sleep_for(DOUBLE_CLK_TIME);
					else
						std::this_thread::sleep_for(CLK_TIME);

					cmd = disasm(next).cmd;
				}
				debugger = true;
				break;
			}
			case 'm': {
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
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					unsigned long address = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
					arch->print_instructions(address, 5);
					wait = true;
				}
				break;
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

	bool right, left, up, down, z, x, backspace, enter;

	//Input
	while (true)
	{
		right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		z = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		x = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
		backspace = sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace);
		enter = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
		
		arch_mutex.lock();

		if (!arch->ram[P1][4]) //Read DPAD
		{
			arch->ram[P1][0] = !right;
			arch->ram[P1][1] = !left;
			arch->ram[P1][2] = !up;
			arch->ram[P1][3] = !down;
		}
		else if (!arch->ram[P1][5]) //Read A,B,Select;Start
		{
			arch->ram[P1][0] = !z;
			arch->ram[P1][1] = !x;
			arch->ram[P1][2] = !backspace;
			arch->ram[P1][3] = !enter;
		}
		pressedAfter = right || left || up || down || z || x || backspace || enter;

		if (!pressedBefore && pressedAfter)
			arch->ram[IF][4] = true;

		pressedBefore = pressedAfter;

		arch_mutex.unlock();

		//Instruction cycle
		if (arch->doubleSpeed)
			std::this_thread::sleep_for(DOUBLE_CLK_TIME);
		else
			std::this_thread::sleep_for(CLK_TIME);
	}
}

void LCDtiming(Architecture* arch)
{
	unsigned short Yline;
	while (true)
	{
		std::this_thread::sleep_for(LCD_VLINE_TIME);

		arch_mutex.lock();

		Yline = arch->ram[LY].to_ulong();
		
		Yline++;
		if (Yline >= 154)
			Yline = 0;

		arch->ram[LY] = Yline;

		//Vblank interrupt

		arch->ram[IF][0] = (Yline == 144);

		//Match of LYC
		arch->ram[STAT][2] = (arch->ram[LY] == arch->ram[LYC]);
		arch->ram[IF][1] = arch->ram[STAT][2] && arch->ram[STAT][6];

		arch_mutex.unlock();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(LCD_W, LCD_H), "ERapGBC");
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

		arch_mutex.lock();
		drawScreen(arch, window, tilesTex);
		arch_mutex.unlock();

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

