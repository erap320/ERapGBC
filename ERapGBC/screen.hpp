#pragma once
#include "architecture.hpp"
#include <SFML/Graphics.hpp>

#define TILE_W 8
#define TILE_H 8

#define LINE_W 256
#define BYTES_PER_PIXEL 4

#define H_TILES 32
#define V_TILES 32

#define SPRITES_NUM 40

#define TRANSPARENT RGBA{0,0,0,0}

#define WRAP true
#define DONT_WRAP false

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

void tile2line(byte* tileMem, sf::Uint8* pixelLine, unsigned short lineX, RGBA* paletteColors, unsigned short y, bool hFlip, bool vFlip, bool wrap, bool spriteMode = false)
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
		for (unsigned short x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;
			if(dots[index] != 0)
				paintPixel(pixelLine + ((lineX + x) * BYTES_PER_PIXEL), paletteColors[dots[index]]);
		}
	}
	else
	{
		for (unsigned short x = 0; x < 8; x++)
		{
			index = y * TILE_W + x;

			//If SCX is used, the parts of the tile that exceed the map length wrap to the start
			if (wrap)
				paintPixel(pixelLine + ((lineX + x) % LINE_W) * BYTES_PER_PIXEL, paletteColors[dots[index]]);
			else
				paintPixel(pixelLine + (lineX + x) * BYTES_PER_PIXEL, paletteColors[dots[index]]);
		}
	}
}

void drawLine(Architecture* arch, TileType type)
{
	arch->updateVideoBank();

	unsigned short line = arch->ram[LY].to_ulong();
	
	if (type == WIN && (!arch->lineSet[line].winEnabled || line < arch->lineSet[line].wy)) {
		for (unsigned short x = 0; x < LINE_W; x++) {
			paintPixel(&arch->WINlayer[(line * LINE_BYTES) + x * BYTES_PER_PIXEL], TRANSPARENT);
		}
		return;
	}

	byte lcdcReg = arch->lineSet[line].lcdc;

	data tileOffset;
	byte tileAttributes;
	unsigned short palNum, bank;
	bool hFlip, vFlip;
	RGBA paletteColors[4];
	byte* tileMem;

	//The line from which tiles should be taken to draw this physical line
	short drawLine = line;
	if (type == BG) {
		drawLine += arch->lineSet[line].scy;
		drawLine %= LINE_W;
	}
	else if (type == WIN) {
		drawLine -= arch->lineSet[line].wy;
		if (drawLine < 0)
			drawLine += LINE_W;
	}

	for (unsigned short hTile = 0; hTile < H_TILES; hTile++)
	{
		//The base of video banks will be 0x8000 in memory, so we use
		//an appropriate offset for internal indexing
		tileOffset = (drawLine / 8) * 32 + hTile;

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
			error("Non exising palette " + to_string(palNum) + " used in line " + to_string(line) + ", tile " + to_string(hTile));
			throw ArchitectureException();
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
		if (type == BG)
		{
			tile2line(tileMem, &arch->BGlayer[(line * LINE_BYTES)], (hTile * TILE_W) - arch->lineSet[line].scx, paletteColors, drawLine % 8, hFlip, vFlip, WRAP);

			//Overlay background
			//Transparency
			paletteColors[0].alpha = 0x00;
			tile2line(tileMem, &arch->Olayer[(line * LINE_BYTES)], (hTile * TILE_W) - arch->lineSet[line].scx, paletteColors, drawLine % 8, hFlip, vFlip, WRAP);
		}
		else if (type == WIN) {
			//Make the section before the offset transparent
			for (unsigned short x = 0; x < arch->lineSet[line].wx; x++) {
				paintPixel(&arch->WINlayer[(line * LINE_BYTES) + x * BYTES_PER_PIXEL], TRANSPARENT);
			}
			tile2line(tileMem, &arch->WINlayer[(line * LINE_BYTES)], (hTile * TILE_W) + arch->lineSet[line].wx, paletteColors, drawLine % 8, hFlip, vFlip, DONT_WRAP);
		}
	}
}

void drawSprites(Architecture* arch)
{
	arch->updateVideoBank();

	unsigned short line = arch->ram[LY].to_ulong();

	if (!arch->lineSet[line].spritesEnabled) {
		for (unsigned short x = 0; x < LINE_W; x++) {
			paintPixel(&arch->SPlayer[(line * LINE_BYTES) + x * BYTES_PER_PIXEL], TRANSPARENT);
			paintPixel(&arch->PSPlayer[(line * LINE_BYTES) + x * BYTES_PER_PIXEL], TRANSPARENT);
		}
		return;
	}

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

	bool doubleSize = arch->ram[LCDC][2];
	unsigned short subLine;

	byte* tileMem;

	//Fill with 0 to make unpainted pixels transparent
	for (unsigned int i = (line * LINE_BYTES); i < (line * LINE_BYTES) + LINE_BYTES; i++)
	{
		arch->SPlayer[i] = 0;
		arch->PSPlayer[i] = 0;
	}

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
				throw ArchitectureException();
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
					tile2line(tileMem, &arch->PSPlayer[(line * LINE_BYTES)], X, paletteColors, subLine, xFlip, yFlip, DONT_WRAP, true);
				else
					tile2line(tileMem, &arch->SPlayer[(line * LINE_BYTES)], X, paletteColors, subLine, xFlip, yFlip, DONT_WRAP, true);
			}
			else
			{
				tileAddr = 16 * arch->ram[address + 2].to_ulong();
				tileMem = &arch->videoBanks[bank][tileAddr];

				subLine = line - Y;

				if(priority)
					tile2line(tileMem, &arch->PSPlayer[(line * LINE_BYTES)], X, paletteColors, subLine, xFlip, yFlip, DONT_WRAP, true);
				else
					tile2line(tileMem, &arch->SPlayer[(line * LINE_BYTES)], X, paletteColors, subLine, xFlip, yFlip, DONT_WRAP, true);
			}
		}
	}
}