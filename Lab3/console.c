#include "console.h"


void console_init() {
	//this is just so we know we've gotten here
    //asm("ldr r0,=0x1234");
	baseaddr1 = (unsigned int) framebuffer;
	haxis=0x3f1f3f00 | (WIDTH/4 - 4);
	vaxis=0x80b6000 | (HEIGHT - 1);
	polarity=0x067f3800;
	params = 0x1820 | 9;
}

void setpixel(int x, int y, unsigned short color) {
	framebuffer[y*WIDTH+x] = color;
}

void drawbox(int x, int y, int width, int height, unsigned short color) {
	for (int cx = x; cx < width + x; cx++) {
		for (int cy = y; cy < height + y; cy++) {
			if (cy == height + y - 1) {
				setpixel(cx, cy, color);
			} else if (cy == y) {
				setpixel(cx, cy, color);
			}
			if (cx == x) {
				setpixel(cx, cy, color);
			} else if (cx == width + x - 1) {
				setpixel(cx, cy, color);
			}
		}
		
	}
}