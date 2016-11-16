#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row,col, 240)] = color;
}

void fillScreen3(volatile u16 color) {
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 38400 | DMA_SOURCE_FIXED | DMA_ON;
}

void drawImage3(int x, int y, int width, int height, const u16 *image) {
	for (int row = 0; row < height; row++) {
		DMA[DMA_CHANNEL_3].src = &image[row*width + 1];
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[(row + x) * 240 + y];
		DMA[DMA_CHANNEL_3].cnt = width |DMA_SOURCE_INCREMENT |
                            	DMA_DESTINATION_INCREMENT |
                            	DMA_ON;
	}
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

