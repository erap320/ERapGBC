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

#define ROM_FILENAME "PokemonPicross.gbc"

#define LCD_W 160
#define LCD_H 144

#define CLK_TIME std::chrono::nanoseconds(954) //1.05 MHz
#define DOUBLE_CLK_TIME std::chrono::nanoseconds(477) //2.10 Mhz

#define DEBUGGER false

//To ensure synchronization between threads
//that try to access the architecture simultaneously
mutex arch_mutex;

void architecture_main(Architecture* arch)
{
	//SCREEN TEST
	arch_mutex.lock();

	arch->videoBanks[0][0] = 0x7C;
	arch->videoBanks[0][1] = 0x7C;
	arch->videoBanks[0][2] = 0x00;
	arch->videoBanks[0][3] = 0xC6;
	arch->videoBanks[0][4] = 0xC6;
	arch->videoBanks[0][5] = 0x00;
	arch->videoBanks[0][6] = 0x00;
	arch->videoBanks[0][7] = 0xFE;
	arch->videoBanks[0][8] = 0xC6;
	arch->videoBanks[0][9] = 0xC6;
	arch->videoBanks[0][10] = 0x00;
	arch->videoBanks[0][11] = 0xC6;
	arch->videoBanks[0][12] = 0xC6;
	arch->videoBanks[0][13] = 0x00;
	arch->videoBanks[0][14] = 0x00;
	arch->videoBanks[0][15] = 0x00;

	arch->colorPalettes[0][3][0] = 0x7C;
	arch->colorPalettes[0][3][1] = 0x00;
	arch->colorPalettes[0][1][0] = 0x03;
	arch->colorPalettes[0][1][1] = 0xE0;
	arch->colorPalettes[0][2][0] = 0x00;
	arch->colorPalettes[0][2][1] = 0x1F;
	arch->colorPalettes[0][0][0] = 0x00;
	arch->colorPalettes[0][0][1] = 0x00;

	arch_mutex.unlock();

	string buffer;
	bool dblClk = false;

	while (true)
	{
		if (DEBUGGER)
			for (int i = 0; i < 10; i++) cout << endl;

		arch_mutex.lock();
		arch->step(DEBUGGER);
		dblClk = arch->ram[KEY1][7];
		arch_mutex.unlock();


		//Instruction cycle
		if (dblClk)
			std::this_thread::sleep_for(DOUBLE_CLK_TIME);
		else
			std::this_thread::sleep_for(CLK_TIME); 

		if (DEBUGGER)
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
					unsigned long breakpoint = strtoul(buffer.c_str(), NULL, 16) & 0xffff;
					while (arch->PC.to_ulong() != breakpoint)
					{
						arch_mutex.lock();
						arch->step(false);
						dblClk = arch->ram[KEY1][7];
						arch_mutex.unlock();

						if (dblClk)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);
					}
				}
				break;
			}
			case 'c': {
				//Continue
				if (!buffer.empty())
				{
					while (true)
					{
						arch_mutex.lock();
						arch->step(false);
						dblClk = arch->ram[KEY1][7];
						arch_mutex.unlock();

						if (dblClk)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);
					}
				}
				break;
			}
			case 'n': {
				//Make n steps
				buffer.erase(0, 2);
				if (!buffer.empty())
				{
					unsigned long steps = strtoul(buffer.c_str(), NULL, 0);
					for (unsigned int i = 0; i < steps; i++)
					{
						arch_mutex.lock();
						arch->step(false);
						dblClk = arch->ram[KEY1][7];
						arch_mutex.unlock();

						if (dblClk)
							std::this_thread::sleep_for(DOUBLE_CLK_TIME);
						else
							std::this_thread::sleep_for(CLK_TIME);
					}
				}
				break;
			}
			case 'o': {
				//Step out
				while (disasm(arch->PC.to_ulong()).cmd != RET)
				{
					arch_mutex.lock();
					arch->step(false);
					dblClk = arch->ram[KEY1][7];
					arch_mutex.unlock();

					if (dblClk)
						std::this_thread::sleep_for(DOUBLE_CLK_TIME);
					else
						std::this_thread::sleep_for(CLK_TIME);
				}
			}
			}
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(LCD_W, LCD_H), "ERapGBC");
	window.setVerticalSyncEnabled(true);

	Architecture* arch = Architecture::instance();
	arch->loadROM(ROM_FILENAME);

	thread processing(architecture_main, arch);
	processing.detach();

	//Window loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		arch_mutex.lock();

		window.clear();
		drawScreen(arch, window);
		window.display();

		//Input
		if (!arch->ram[P1][4]) //Read DPAD
		{
			arch->ram[P1][0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
			arch->ram[P1][1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
			arch->ram[P1][2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
			arch->ram[P1][3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		}
		else if (!arch->ram[P1][5]) //Read A,B,Select;Start
		{
			arch->ram[P1][0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
			arch->ram[P1][1] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
			arch->ram[P1][2] = sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace);
			arch->ram[P1][3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
		}
		arch_mutex.unlock();
	}

	return 0;
}

