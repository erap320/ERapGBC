#pragma once
#include "architecture.hpp"
#include <SFML/Graphics.hpp>

void fromGBColor(sf::Uint8* pixel, byte high, byte low)
{
	/*
	*	High		Low
	*	xBBBBBGG	GGGRRRRR
	*/

	sf::Uint8 red = (low & (byte)0x1F).to_ulong() << 3;
	sf::Uint8 green = ((low & (byte)0xE0) >> 5).to_ulong() + ((high & (byte)0x3) << 3).to_ulong() << 3;
	sf::Uint8 blue = ((high & (byte)0x7c) >> 2).to_ulong() << 3;

	pixel[0] = red; //R
	pixel[1] = green; //G
	pixel[2] = blue; //B
	pixel[3] = 0xFF; //A
}

void drawTile(Architecture* arch, sf::RenderWindow& const window, byte tileNumber, byte tilePalette)
{
	sf::Uint8* pixels = new sf::Uint8[8 * 8 * 4];

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			fromGBColor(&pixels[(y * 8 + x) * 4], 3, 224);
		}
	}

	sf::Image image;
	image.create(8, 8, pixels);

	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite tile;
	tile.setTexture(texture);
	
	window.draw(tile);
}