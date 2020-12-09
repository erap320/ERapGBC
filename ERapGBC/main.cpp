#include "architecture.hpp"
#include "screen.hpp"
#include <iostream>
#include <thread>
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

bool debugger = true;

//To ensure synchronization between threads
//that try to access the architecture simultaneously
mutex arch_mutex;

void architecture_main(Architecture* arch)
{
	string buffer;
	string lastCommand = "n";

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
		}

		if (debugger)
		{
			cout << "\n> ";
			getline(cin, buffer);

			if (buffer.empty())
				buffer = lastCommand;
			else
				lastCommand = buffer;

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

bool check_buttons(Architecture* arch, bool pressedBefore)
{
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool A = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	bool B = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
	bool select = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	bool start = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

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
	bool pressedAfter = right || left || up || down || A || B || select || start;

	if (!pressedBefore && pressedAfter)
		arch->ram[IF][4] = true;

	return pressedAfter;
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
	
	sf::Texture tilesTex[H_TILES * V_TILES];
	sf::Sprite tiles[H_TILES * V_TILES];

	bool buttonsPressed = false;

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

		buttonsPressed = check_buttons(arch, buttonsPressed);

		window.clear();

		if (arch->ram[LY].to_ulong() < 144)
		{
			drawScreen(arch, window, tilesTex);
		}

		for (int y = 0; y < V_TILES; y++)
		{
			for (int x = 0; x < H_TILES; x++)
			{
				index = y * V_TILES + x;
				tiles[index].setTexture(tilesTex[index]);
				tiles[index].setPosition(x * 8, y * 8);
				window.draw(tiles[index]);
			}
		}

		window.display();
	}

	return 0;
}

