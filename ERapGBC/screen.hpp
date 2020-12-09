#pragma once
#include "architecture.hpp"
#include <SFML/Graphics.hpp>

#define TILE_W 8
#define TILE_H 8
#define PIXEL_BYTES 4

#define H_TILES 20
#define V_TILES 20

struct RGB
{
	sf::Uint8 red, green, blue;
};

RGB fromGBColor(byte high, byte low)
{
	/*
	*	High		Low
	*	xBBBBBGG	GGGRRRRR
	*/
	RGB result;
	result.red = (low & (byte)0x1F).to_ulong() << 3;
	result.green = ((low & (byte)0xE0) >> 5).to_ulong() + ((high & (byte)0x3) << 3).to_ulong() << 3;
	result.blue = ((high & (byte)0x7c) >> 2).to_ulong() << 3;

	return result;
}

void paintPixel(sf::Uint8* pixel, RGB color)
{
	pixel[0] = color.red; //R
	pixel[1] = color.green; //G
	pixel[2] = color.blue; //B
	pixel[3] = 0xFF; //A
}

void drawTile(Architecture* arch, sf::RenderWindow& const window, unsigned short x, unsigned short y, sf::Texture* tile)
{
	if (x > 31 || y > 31)
	{
		error("Trying to draw a non exising tile: " + to_string(y) + "," + to_string(x));
		return;
	}

	byte lcdcReg = arch->ram[LCDC];

	///////////////////////////////////////////////////////////////// BG rendering

	//The base of video banks will be 0x8000 in memory, so we use
	//an appropriate offset for internal indexing
	data tileOffset = y * 32 + x;

	if (lcdcReg[6])
		tileOffset += 0x1800; // 0x9800
	else
		tileOffset += 0x1C00; // 0x9C00
	
	data tileNumber = arch->videoBanks[0][tileOffset].to_ulong();

	byte tileAttributes = arch->videoBanks[1][tileOffset];
	unsigned short palNum = tileAttributes.to_ulong() & 7;
	unsigned short bank = tileAttributes[3];
	bool hFlip = tileAttributes[5];
	bool vFlip = tileAttributes[6];

	if (palNum > 7)
	{
		error("Non exising palette " + to_string(palNum) + " used in tile: " + to_string(y) + "," + to_string(x));
		return;
	}

	RGB paletteColors[4];

	for (int i = 0; i < 4; i++)
	{
		paletteColors[i] = fromGBColor(arch->colorPalettes[palNum][i][1], arch->colorPalettes[palNum][i][0]);
	}

	//The base of video banks will be 0x8000 in memory, so to have
	//the equivalent we use 0 offset
	//Every tile of 8*8 2bpp uses 16 bytes 
	data tileAddr;
	if (lcdcReg[4]) // 8000 mode
		tileAddr = /* 0x0000 + */ 16 * tileNumber;
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

	byte* tileMem = &arch->videoBanks[bank][tileAddr];

	unsigned short dots[64];
	unsigned short bit;
	int by;
	for (int i = 0; i < 64; i++)
	{
		if(vFlip)
			by = (7-(i/8)) * 2;
		else
			by = (i/8) * 2;

		if(hFlip)
			bit = i % 8;
		else
			bit = 7 - (i % 8);

		dots[i] = tileMem[by][bit] + 2*tileMem[by+1][bit];
	}

	sf::Uint8* pixels = new sf::Uint8[TILE_W * TILE_H * 4];

	unsigned short index;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;
			paintPixel(&pixels[index*PIXEL_BYTES], paletteColors[dots[index]]);
		}
	}

	sf::Image img;
	img.create(8, 8, pixels);

	tile->loadFromImage(img);

	delete[] pixels;
}

void drawScreen(Architecture* arch, sf::RenderWindow& const window, sf::Texture* tiles)
{
	arch->updateVideoBank();
	for (int y = 0; y < V_TILES; y++)
	{
		for (int x = 0; x < H_TILES; x++)
		{
			drawTile(arch, window, x, y, &tiles[y*V_TILES+x]);
		}
	}
}