#include "resources.hpp"
#include "architecture.hpp"
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

//Physical pixels
#define LCD_W 160
#define LCD_H 144

//Tilmaps size
#define TEX_W 256
#define TEX_H 256

#define TRANSPARENT_BLACK sf::Color(0, 0, 0, 0)

#define LAYERS_NUM 5

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
			case 'h': {
				//Print help message for the debugger
				cout << " b (address): break execution on the address\n"
					<< " c: continue with normal execution\n"
					<< " n (steps): make a number of steps\n"
					<< " o: continue execution until a return instruction is encountered\n"
					<< " m: print memory\n"
					<< " d (address): disassemble instructions at address\n"
					<< " r: dump ram to a file\n"
					<< " v: dump video ram to a file\n"
					<< " p: print palettes\n"
					<< " h: print this help message\n";
				wait = true;
				break;
			}
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
	
	sf::Sprite Background;
	sf::Sprite Overlay;
	sf::Sprite Window;
	sf::Sprite Sprites;
	sf::Sprite PSprites;

	sf::Texture BGtex;
	sf::Texture Otex;
	sf::Texture WINtex;
	sf::Texture SPtex;
	sf::Texture PSPtex;

	//Layers are organized in these arrays in the same order in which
	//they have to be drawn during each cycle
	sf::Sprite* layerSprites[LAYERS_NUM] = { &Background, &PSprites, &Overlay, &Window, &Sprites };
	sf::Texture* layerTextures[LAYERS_NUM] = { &BGtex, &PSPtex, &Otex, &WINtex, &SPtex };
	PixelLayer* layerPixels[LAYERS_NUM] = { &arch->BGlayer, &arch->PSPlayer, &arch->Olayer, &arch->WINlayer, &arch->SPlayer };


	// Initialize textures as transparent black
	// and assign each texture to its sprite, so that
	// it is updated automatically
	sf::Image img;
	img.create(TEX_W, TEX_H, TRANSPARENT_BLACK);

	for (sf::Texture* tex : layerTextures)
		tex->loadFromImage(img);

	for (int i = 0; i < LAYERS_NUM; i++)
		layerSprites[i]->setTexture(*layerTextures[i]);

	// Initialize sprite lines' positions, always the same
	// unlike the ones of background and window that change
	// at each line due to scx/y and wx/y
	for (sf::Sprite* sp : layerSprites) {
		sp->setPosition(0, 0);
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

		//Keyboard interactions
		//Stop execution to debug
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause))
			debugger = true;

		//Turbo mode!
		arch->turbo = sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

		buttonsPressed = check_buttons(arch, buttonsPressed);

		window.clear();

		for (int i = 0; i < LAYERS_NUM; i++)
			layerTextures[i]->update(*layerPixels[i]);

		for (sf::Sprite* sp : layerSprites)
			window.draw(*sp);

		window.display();
	}

	return 0;
}

