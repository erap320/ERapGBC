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

#define DEBUGGER true

//To ensure synchronization between threads
//that try to access the architecture simultaneously
mutex arch_mutex;

void architecture_main(Architecture* arch)
{
	string buffer;

	while (true)
	{
		if (DEBUGGER)
		{
			for (int i = 0; i < 10; i++) cout << endl;
		}

		arch->step(DEBUGGER);

		//Instruction cycle
		std::this_thread::sleep_for(std::chrono::nanoseconds(477)); 

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
						arch->step(false);
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
						arch->step(false);
				}
				break;
			}
			case 'o': {
				//Step out
				while (disasm(arch->PC.to_ulong()).cmd != RET)
					arch->step(false);
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

		window.clear();
		window.display();
	}

	return 0;
}

