#include "architecture.hpp"
#include "screen.hpp"
#include <thread>

#define HBLANK_CLKS 457
#define MODE2_CLKS 80
#define MODE3_CLKS 230
#define MODE3_END 310 //MODE2 + MODE3
#define MODE0_CLKS 146
#define VBLANK_LINES 144
#define MAX_V_LINES 153

#define FRAME_TIME std::chrono::milliseconds(10)

void Architecture::lcdc()
{
	if (screenOn)
	{
		unsigned short Yline = ram[LY].to_ulong();
		if (time >= HBLANK_CLKS)
		{
			Yline++;

			if (Yline == VBLANK_LINES) //Mode 1
			{
				/*
				* Sleep here until 16.75ms are elapsed
				* from the last vblank, to match the
				* original timing of the GBC
				*/
				if (!turbo)
				{
					std::this_thread::sleep_until(lastVBlank + FRAME_TIME);
					lastVBlank = std::chrono::high_resolution_clock::now() + FRAME_TIME;
				}

				ram[STAT][1] = 0;
				ram[STAT][0] = 1;
				lcdcMode = 1;

				ram[IF][0] = true;

				//Also as LCDC interrupt
				if (ram[STAT][4])
					ram[IF][1] = true;
			}
			else if (Yline > MAX_V_LINES)
				Yline = 0;

			ram[LY] = Yline;

			//Match of LYC
			ram[STAT][2] = (ram[LY] == ram[LYC]);

			if (ram[STAT][2] && ram[STAT][6])//Compare interrupt enabled
				ram[IF][1] = true;

			time = time % HBLANK_CLKS;
		}

		if (Yline < VBLANK_LINES)
		{
			if (time < MODE2_CLKS) //Mode 2
			{
				if (lcdcMode != 2)
				{
					ram[STAT][1] = 1;
					ram[STAT][0] = 0;
					lcdcMode = 2;

					//Mode 2 interrupt
					if (ram[STAT][5])
						ram[IF][1] = true;
				}
			}
			else if (time < MODE3_END) //Mode 3
			{
				if (lcdcMode != 3)
				{
					ram[STAT][1] = 1;
					ram[STAT][0] = 1;
					lcdcMode = 3;

					lineSet[Yline].scx = ram[SCX].to_ulong();
					lineSet[Yline].scy = ram[SCY].to_ulong();
					lineSet[Yline].wx = ram[WX].to_ulong() - 7;
					lineSet[Yline].wy = ram[WY].to_ulong();
					lineSet[Yline].winEnabled = ram[LCDC][5] && ram[WX].to_ulong() < 0xA7;
					lineSet[Yline].spritesEnabled = ram[LCDC][1];
					lineSet[Yline].lcdc = ram[LCDC];

					//Draw line on textures
					drawLine(this, BG);

					drawLine(this, WIN);

					drawSprites(this);
				}
			}
			else if (time < HBLANK_CLKS) //Mode 0
			{
				if (lcdcMode != 0)
				{
					ram[STAT][1] = 0;
					ram[STAT][0] = 0;
					lcdcMode = 0;

					//Hblank interrupt
					if (ram[STAT][3])
						ram[IF][1] = true;
				}
			}
		}
	}
	else
	{
		//Ready to restart from 0
		ram[LY] = 0;
		ram[STAT][1] = 0;
		ram[STAT][0] = 0;
		lcdcMode = 0;
		time = 0;
	}
}