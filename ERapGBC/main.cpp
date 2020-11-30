#include "architecture.hpp"
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
	string buffer;
	bool dblClk = false;

	while (true)
	{
		if (DEBUGGER)
		{
			for (int i = 0; i < 10; i++) cout << endl;
		}

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
					while (arch->PC.to_ulong() + disasm(arch->PC.to_ulong()).length() != breakpoint)
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

		//Input
		arch_mutex.lock();
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

		window.clear();
		window.display();
	}

	return 0;
}

