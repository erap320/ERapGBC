#pragma once
#include "architecture.hpp"
#include <SFML/Graphics.hpp>

#define TILE_W 8
#define TILE_H 8
#define PIXEL_BYTES 4

#define H_TILES 20
#define V_TILES 20

#define SPRITES_NUM 40

struct RGB
{
	sf::Uint8 red, green, blue, alpha;
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
	result.alpha = 0xFF;

	return result;
}

void paintPixel(sf::Uint8* pixel, RGB color)
{
	pixel[0] = color.red; //R
	pixel[1] = color.green; //G
	pixel[2] = color.blue; //B
	pixel[3] = color.alpha; //A
}

enum TileType {
	BG = 3, WIN = 6
};

void tile2pixels(Architecture* arch, byte* tileMem, sf::Uint8* pixels, RGB* paletteColors, bool hFlip, bool vFlip)
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
			paintPixel(&pixels[index * PIXEL_BYTES], paletteColors[dots[index]]);
		}
	}
}

void drawTile(Architecture* arch, unsigned short x, unsigned short y, sf::Texture* tile, TileType type)
{
	if (x > 31 || y > 31)
	{
		error("Trying to draw a non exising tile: " + to_string(y) + "," + to_string(x));
		return;
	}

	byte lcdcReg = arch->ram[LCDC];

	//The base of video banks will be 0x8000 in memory, so we use
	//an appropriate offset for internal indexing
	data tileOffset = y * 32 + x;

	if (lcdcReg[(unsigned short)type])
		tileOffset += 0x1C00; // 0x9C00
	else
		tileOffset += 0x1800; // 0x9800
	
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

	byte* tileMem = &arch->videoBanks[bank][tileAddr];

	sf::Uint8* pixels = new sf::Uint8[TILE_W * TILE_H * 4];

	tile2pixels(arch, tileMem, pixels, paletteColors, hFlip, vFlip);

	sf::Image img;
	img.create(8, 8, pixels);

	tile->loadFromImage(img);

	delete[] pixels;
}

void drawSprites(Architecture* arch, sf::Texture *spritesTex, sf::Sprite* sprites)
{
	data base = 0xFE00;
	data address;

	int Y;
	int X;
	data tileAddr;
	byte attributes;
	bool xFlip;
	bool yFlip;

	unsigned short palNum;
	RGB paletteColors[4];

	unsigned int tilePixels = TILE_W * TILE_H * 4;
	bool doubleSize = arch->ram[LCDC][2];
	//bool doubleSize = false;

	byte* tileMem;
	sf::Uint8* pixels;

	if (doubleSize)
		pixels = new sf::Uint8[tilePixels * 2];
	else
		pixels = new sf::Uint8[tilePixels];

	sf::Image img;
	for (unsigned int i = 0; i < SPRITES_NUM; i++)
	{
		address = base + (4 * i);
		
		Y = arch->ram[address].to_ulong() - 16;
		X = arch->ram[address+1].to_ulong() - 8;
		attributes = arch->ram[address + 3];
		xFlip = attributes[5];
		yFlip = attributes[6];

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
		if (doubleSize)
		{
			tileAddr = 16 * arch->ram[address + 2].to_ulong();
			tileMem = &arch->videoBanks[attributes[3]][tileAddr];
			tile2pixels(arch, tileMem, pixels, paletteColors, xFlip, yFlip);

			tileAddr += 16;
			tileMem = &arch->videoBanks[attributes[3]][tileAddr];
			tile2pixels(arch, tileMem, &pixels[tilePixels], paletteColors, xFlip, yFlip);

			img.create(8, 16, pixels);
		}
		else
		{
			tileAddr = 16 * arch->ram[address + 2].to_ulong();

			tileMem = &arch->videoBanks[attributes[3]][tileAddr];
			tile2pixels(arch, tileMem, pixels, paletteColors, xFlip, yFlip);
			img.create(8, 8, pixels);
		}

		spritesTex[i].loadFromImage(img);
		sprites[i].setTexture(spritesTex[i]);
		sprites[i].setPosition(X, Y);
	}

	delete[] pixels;
}

void drawScreen(Architecture* arch, sf::Texture* tiles, TileType type)
{
	arch->updateVideoBank();
	for (int y = 0; y < V_TILES; y++)
	{
		for (int x = 0; x < H_TILES; x++)
		{
			drawTile(arch, x, y, &tiles[y*V_TILES+x], type);
		}
	}
}