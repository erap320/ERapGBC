#include "resources.hpp"
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

#define LCD_W 160
#define LCD_H 144

bool debugger = false;

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
	arch->right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	arch->left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	arch->up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	arch->down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	arch->Abtn = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	arch->Bbtn = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
	arch->select = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	arch->start = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

	bool pressedAfter = arch->right || arch->left || arch->up || arch->down || arch->Abtn || arch->Bbtn || arch->select || arch->start;

	if (!pressedBefore && pressedAfter)
		arch->ram[IF][4] = true;
	
	return pressedAfter;
}

void displayAll(sf::RenderWindow &window, sf::Sprite* sprites)
{
	unsigned int index;
	for (int y = 0; y < V_TILES; y++)
	{
		for (int x = 0; x < H_TILES; x++)
		{
			index = y * V_TILES + x;
			window.draw(sprites[index]);
		}
	}
}

int main(int argc, char* argv[])
{
	sf::RenderWindow window(sf::VideoMode(LCD_W, LCD_H), "ERapGBC");
	window.setSize(sf::Vector2u(LCD_W*3, LCD_H*3));
	window.setVerticalSyncEnabled(true);
	sf::Image icon;
	icon.loadFromMemory(window_icon, window_icon_size);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//Create architecture and load rom
	Architecture* arch = Architecture::instance();

	//ROM name from arguments
	if (argc > 1)
		arch->loadROM(string(argv[1]));

	//Start the "CPU"
	thread processing(architecture_main, arch);
	processing.detach();
	
	sf::Texture BGtilesTex[H_TILES * V_TILES];
	sf::Sprite BGtiles[H_TILES * V_TILES];

	sf::Texture OtilesTex[H_TILES * V_TILES];
	sf::Sprite Otiles[H_TILES * V_TILES];

	sf::Texture WINtilesTex[H_TILES * V_TILES];
	sf::Sprite WINtiles[H_TILES * V_TILES];

	sf::Texture spritesTex[SPRITES_NUM];
	sf::Sprite sprites[SPRITES_NUM];
	bool spritesPriority[SPRITES_NUM];

	bool buttonsPressed = false;
	bool winEnabled = false;
	bool spritesEnabled = false;

	unsigned short wx, wy;
	unsigned short scx, scy;

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

		arch_mutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause))
			debugger = true;

		buttonsPressed = check_buttons(arch, buttonsPressed);
		arch_mutex.unlock();

		window.clear();

		if (arch->lcdcMode == 3)
		{
			winEnabled = arch->ram[LCDC][5] && arch->ram[WX].to_ulong() > 0x7 && arch->ram[WX].to_ulong() < 0xA7;
			spritesEnabled = arch->ram[LCDC][1];

			drawScreen(arch, BGtilesTex, BG, OtilesTex);

			if (winEnabled)
				drawScreen(arch, WINtilesTex, WIN);

			if (spritesEnabled)
				drawSprites(arch, spritesTex, sprites, spritesPriority);

			wx = arch->ram[WX].to_ulong() - 7;
			wy = arch->ram[WY].to_ulong();
			scx = arch->ram[SCX].to_ulong();
			scy = arch->ram[SCY].to_ulong();

			for (int y = 0; y < V_TILES; y++)
			{
				for (int x = 0; x < H_TILES; x++)
				{
					index = y * V_TILES + x;

					BGtiles[index].setTexture(BGtilesTex[index]);
					BGtiles[index].setPosition(x * 8 + scx, y * 8 + scy);

					Otiles[index].setTexture(OtilesTex[index]);
					Otiles[index].setPosition(x * 8 + scx, y * 8 + scy);

					if (winEnabled)
					{
						WINtiles[index].setTexture(WINtilesTex[index]);
						WINtiles[index].setPosition(x * 8 + wx, y * 8 + wy);
					}
				}
			}
		}

		displayAll(window, BGtiles);

		if (spritesEnabled)
		{
			for (int i = 0; i < SPRITES_NUM; i++)
			{
				if (spritesPriority[i])
					window.draw(sprites[i]);
			}
		}

		displayAll(window, Otiles);

		if (spritesEnabled)
		{
			for (int i = 0; i < SPRITES_NUM; i++)
			{
				if (!spritesPriority[i])
					window.draw(sprites[i]);
			}
		}

		window.display();
	}

	return 0;
}

