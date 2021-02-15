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
	
	sf::Texture BGtex[LINES_NUM];
	sf::Sprite Background[LINES_NUM];

	sf::Texture Otex[LINES_NUM];
	sf::Sprite Overlay[LINES_NUM];

	sf::Texture WINtex[LINES_NUM];
	sf::Sprite Window[LINES_NUM];

	sf::Texture SPtex[LINES_NUM];
	sf::Sprite Sprites[LINES_NUM];

	sf::Texture PSPtex[LINES_NUM];
	sf::Sprite PSprites[LINES_NUM];

	// Initialize sprite lines' positions, always the same
	// unlike the ones of background and window that change
	// at each line due to scx/y and wx/y
	for (unsigned short line = 0; line < LINES_NUM; line++)
	{
		Sprites[line].setPosition(0, line);
		PSprites[line].setPosition(0, line);
	}

	bool buttonsPressed = false;

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause))
			debugger = true;

		buttonsPressed = check_buttons(arch, buttonsPressed);
		arch_mutex.unlock();

		window.clear();

		if (arch->lcdcMode == 3)
		{
			for (unsigned short line = 0; line < LINES_NUM; line++)
			{
				drawLine(arch, line, &BGtex[line], BG, &Otex[line]);

				if (arch->lineSet[line].winEnabled)
				{
					drawLine(arch, line, &WINtex[line], WIN);
				}

				Background[line].setTexture(BGtex[line]);
				Background[line].setPosition(arch->lineSet[line].scx, line + arch->lineSet[line].scy);

				Overlay[line].setTexture(Otex[line]);
				Overlay[line].setPosition(arch->lineSet[line].scx, line + arch->lineSet[line].scy);

				if (arch->lineSet[line].winEnabled)
				{
					Window[line].setTexture(WINtex[line]);
					Window[line].setPosition(arch->lineSet[line].wx, line + arch->lineSet[line].wy);
				}

				if (arch->lineSet[line].spritesEnabled)
				{
					drawSprites(arch, line, &SPtex[line], &PSPtex[line]);
					Sprites[line].setTexture(SPtex[line]);
					PSprites[line].setTexture(PSPtex[line]);
				}
			}
		}

		for (unsigned short line = 0; line < LINES_NUM; line++)
			window.draw(Background[line]);

		for (unsigned short line = 0; line < LINES_NUM; line++)
			window.draw(PSprites[line]);

		for (unsigned short line = 0; line < LINES_NUM; line++)
			window.draw(Overlay[line]);

		for (unsigned short line = 0; line < LINES_NUM; line++)
			if (arch->lineSet[line].winEnabled)
				window.draw(Window[line]);

		for (unsigned short line = 0; line < LINES_NUM; line++)
			window.draw(Sprites[line]);

		window.display();
	}

	return 0;
}

