#pragma once
#include "architecture.hpp"
#include <SFML/Graphics.hpp>

#define TILE_W 8
#define TILE_H 8
#define LINE_W 160
#define BYTES_PER_PIXEL 4

#define LINE_BYTES 640

#define H_TILES 20
#define V_TILES 20

#define LINES_NUM 160

#define SPRITES_NUM 40

struct RGBA
{
	sf::Uint8 red, green, blue, alpha;
};

RGBA fromGBColor(byte high, byte low)
{
	/*
	*	High		Low
	*	xBBBBBGG	GGGRRRRR
	*/
	RGBA result;
	result.red = (low & (byte)0x1F).to_ulong() << 3;
	result.green = ((low & (byte)0xE0) >> 5).to_ulong() + ((high & (byte)0x3) << 3).to_ulong() << 3;
	result.blue = ((high & (byte)0x7c) >> 2).to_ulong() << 3;
	result.alpha = 0xFF;

	return result;
}

void paintPixel(sf::Uint8* pixel, RGBA color)
{
	pixel[0] = color.red; //R
	pixel[1] = color.green; //G
	pixel[2] = color.blue; //B
	pixel[3] = color.alpha; //A
}

enum TileType {
	BG = 3, WIN = 6
};

//Deprecated, but could be useful to see how complete tiles were drawn
void tile2pixels(byte* tileMem, sf::Uint8* pixels, RGBA* paletteColors, bool hFlip, bool vFlip)
{
	unsigned short dots[64];
	unsigned short bit;
	int by;
	for (int i = 0; i < 64; i++)
	{
		if (vFlip)
			by = (7 - (i / 8)) * 2;
		else
			by = (i / 8) * 2;

		if (hFlip)
			bit = i % 8;
		else
			bit = 7 - (i % 8);

		dots[i] = tileMem[by][bit] + 2 * tileMem[by + 1][bit];
	}

	unsigned short index;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;
			paintPixel(&pixels[index * BYTES_PER_PIXEL], paletteColors[dots[index]]);
		}
	}
}

void tile2line(byte* tileMem, sf::Uint8* pixels, RGBA* paletteColors, unsigned short y, bool hFlip, bool vFlip, bool spriteMode = false)
{
	unsigned short dots[64];
	unsigned short bit;
	int by;
	for (int i = 0; i < 64; i++)
	{
		if (vFlip)
			by = (7 - (i / 8)) * 2;
		else
			by = (i / 8) * 2;

		if (hFlip)
			bit = i % 8;
		else
			bit = 7 - (i % 8);

		dots[i] = tileMem[by][bit] + 2 * tileMem[by + 1][bit];
	}

	unsigned short index;

	//Sprites can overlap in the same line
	//this means we shouldn't change pixels
	//when there is a transparent color
	if (spriteMode)
	{
		for (int x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;
			if(dots[index] != 0)
				paintPixel(pixels + x * BYTES_PER_PIXEL, paletteColors[dots[index]]);
		}
	}
	else
	{
		for (int x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;
			paintPixel(pixels + x * BYTES_PER_PIXEL, paletteColors[dots[index]]);
		}
	}
}

void drawLine(Architecture* arch, unsigned short line, sf::Texture* tiles, TileType type, sf::Texture* overlay = NULL)
{
	arch->updateVideoBank();

	if (line > LINES_NUM)
	{
		error("Trying to draw a non exising line: " + to_string(line));
		return;
	}

	byte lcdcReg = arch->lineSet[line].lcdc;

	data tileOffset;
	byte tileAttributes;
	unsigned short palNum, bank;
	bool hFlip, vFlip;
	RGBA paletteColors[4];
	byte* tileMem;

	sf::Uint8* pixels = new sf::Uint8[LINE_BYTES];
	sf::Uint8* Opixels = new sf::Uint8[LINE_BYTES];
	sf::Image img;

	for (unsigned int col = 0; col < H_TILES; col++)
	{
		//The base of video banks will be 0x8000 in memory, so we use
		//an appropriate offset for internal indexing
		tileOffset = (line / 8) * 32 + col;

		if (lcdcReg[(unsigned short)type])
			tileOffset += 0x1C00; // 0x9C00
		else
			tileOffset += 0x1800; // 0x9800

		data tileNumber = arch->videoBanks[0][tileOffset].to_ulong();

		tileAttributes = arch->videoBanks[1][tileOffset];
		palNum = tileAttributes.to_ulong() & 7;
		bank = tileAttributes[3];
		hFlip = tileAttributes[5];
		vFlip = tileAttributes[6];

		if (palNum > 7)
		{
			error("Non exising palette " + to_string(palNum) + " used in line " + to_string(line) + ", tile " + to_string(col));
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			paletteColors[i] = fromGBColor(arch->colorPalettes[palNum][i][1], arch->colorPalettes[palNum][i][0]);
		}

		//The base of video banks will be 0x8000 in memory, so to have
		//the equivalent we use an appropriate offset
		//Every tile of 8*8 2bpp uses 16 bytes 
		data tileAddr;
		if (lcdcReg[4]) // 8000 mode
			tileAddr = 16 * tileNumber;
		else // 8800 mode
		{
			if (tileNumber <= 127)
				tileAddr = 0x1000 + (16 * tileNumber);
			else
			{
				tileNumber -= 128;
				tileAddr = 0x0800 + (16 * tileNumber);
			}
		}

		tileMem = &arch->videoBanks[bank][tileAddr];

		//Regular background
		tile2line(tileMem, &pixels[col * TILE_W * BYTES_PER_PIXEL], paletteColors, line % 8, hFlip, vFlip);

		//Overlay background
		if (overlay != NULL)
		{
			//Transparency
			paletteColors[0].alpha = 0x00;
			tile2line(tileMem, &Opixels[col * TILE_W * BYTES_PER_PIXEL], paletteColors, line % 8, hFlip, vFlip);
		}
	}

	img.create(LINE_W, 1, pixels);
	tiles->loadFromImage(img);

	//Overlay background
	if (overlay != NULL)
	{
		img.create(LINE_W, 1, Opixels);
		overlay->loadFromImage(img);
	}

	delete[] pixels;
	delete[] Opixels;
}

void drawSprites(Architecture* arch, unsigned short line, sf::Texture *spritesTex, sf::Texture* PspritesTex)
{
	arch->updateVideoBank();

	data base = 0xFE00;
	data address;

	int Y;
	int endY;
	int X;
	data tileAddr;
	byte attributes;
	unsigned short bank;
	bool xFlip;
	bool yFlip;
	bool priority;

	unsigned short palNum;
	RGBA paletteColors[4];

	bool doubleSize = arch->lineSet[line].lcdc[2];
	unsigned short subLine;

	byte* tileMem;
	sf::Uint8* pixels = new sf::Uint8[LINE_BYTES];
	sf::Uint8* Ppixels = new sf::Uint8[LINE_BYTES];

	//Fill with 0 to make unpainted pixels transparent
	for (unsigned int i = 0; i < LINE_BYTES; i++)
	{
		pixels[i] = 0;
		Ppixels[i] = 0;
	}

	sf::Image img;

	//Only 10 sprites per line are drawn, the rest is ignored
	unsigned short count = 0;

	for (unsigned int i = 0; i < SPRITES_NUM && count < 10; i++)
	{
		address = base + (4 * i);
		
		Y = arch->ram[address].to_ulong() - 16;
		endY = Y + (doubleSize ? 15 : 7);

		//Check if the sprite is in this line
		if (line >= Y && line <= endY)
		{
			count++;

			X = arch->ram[address + 1].to_ulong() - 8;
			attributes = arch->ram[address + 3];
			bank = attributes[3];
			xFlip = attributes[5];
			yFlip = attributes[6];
			priority = attributes[7];

			palNum = attributes.to_ulong() & 7;

			if (palNum > 7)
			{
				error("Non exising palette " + to_string(palNum) + " used in sprite: " + to_string(i));
				return;
			}

			for (int i = 0; i < 4; i++)
			{
				paletteColors[i] = fromGBColor(arch->objPalettes[palNum][i][1], arch->objPalettes[palNum][i][0]);
			}
			//Transparency
			paletteColors[0].alpha = 0x00;

			//The base of video banks will be 0x8000 in memory, so to have
			//the equivalent we use an appropriate offset
			if (doubleSize && line > Y + 7)
			{
				tileAddr = 16 * arch->ram[address + 2].to_ulong() + 16;
				tileMem = &arch->videoBanks[bank][tileAddr];

				subLine = line - (Y + 8);

				if (priority)
					tile2line(tileMem, &Ppixels[X * BYTES_PER_PIXEL], paletteColors, subLine, xFlip, yFlip, true);
				else
					tile2line(tileMem, &pixels[X * BYTES_PER_PIXEL], paletteColors, subLine, xFlip, yFlip, true);
			}
			else
			{
				tileAddr = 16 * arch->ram[address + 2].to_ulong();
				tileMem = &arch->videoBanks[bank][tileAddr];

				subLine = line - Y;

				if(priority)
					tile2line(tileMem, &Ppixels[X * BYTES_PER_PIXEL], paletteColors, subLine, xFlip, yFlip, true);
				else
					tile2line(tileMem, &pixels[X * BYTES_PER_PIXEL], paletteColors, subLine, xFlip, yFlip, true);
			}
		}
	}

	img.create(LINE_W, 1, pixels);
	spritesTex->loadFromImage(img);

	img.create(LINE_W, 1, Ppixels);
	PspritesTex->loadFromImage(img);

	delete[] pixels;
	delete[] Ppixels;
}