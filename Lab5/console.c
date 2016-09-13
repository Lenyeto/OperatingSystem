#include "console.h"
#include "font.h"

void console_init() {
	//this is just so we know we've gotten here
    //asm("ldr r0,=0x1234");
	baseaddr1 = (unsigned int) framebuffer;
	haxis=0x3f1f3f00 | (WIDTH/4 - 4);
	vaxis=0x80b6000 | (HEIGHT - 1);
	polarity=0x067f3800;
	params = 0x1820 | 9;
	
	console_pos_x = 0;
	console_pos_y = 0;
	
	curColor = COLOR16(255, 255, 255);
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

void console_draw_character(int x, int y, unsigned int color, char c) {
	for (int i = 0; i < CHAR_WIDTH + 1; i++) {
		for (int j = 0; j < CHAR_HEIGHT; j++) {
			if (font_data[(int) c][j] >> i & 1) {
				setpixel(x - i + CHAR_WIDTH, y + j, color);
			} else {
				setpixel(x - i + CHAR_WIDTH, y + j, COLOR16(0, 0, 0));
			}
		}
	}
}

void console_putc(char x) {
	if (x == '\t') {
		while (1)
		{
			console_pos_x += CHAR_WIDTH;
			if (!(console_pos_x % 8)) {
				break;
			}
		}
	} else if (x == '\n') {
		console_pos_y += CHAR_HEIGHT;
		console_pos_x = 0;
	} else if (x == '\b') {
		if (console_pos_x == 0) {
			console_pos_y -= CHAR_HEIGHT;
			console_pos_x = 88 * CHAR_WIDTH;
		} else {
			console_pos_x -= CHAR_WIDTH;
			console_putc(' ');
			if (console_pos_x == 0) {
				console_pos_y -= CHAR_HEIGHT;
				console_pos_x = 88*CHAR_HEIGHT;
			} else {
				console_pos_x -= CHAR_WIDTH;
			}
		}
	} else if (x == '\f') {
		console_clear();
		console_pos_x = 0;
		console_pos_y = 0;
	} else {
		console_draw_character(console_pos_x, console_pos_y, curColor, x);
		console_pos_x += CHAR_WIDTH;
	}
	
	if (console_pos_x > 88 * CHAR_WIDTH) {
		console_pos_y += CHAR_HEIGHT;
		console_pos_x = 0;
	}
}

void console_clear() {
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		framebuffer[i] = 0;
	}
}
