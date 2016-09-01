#define COLOR16(r, g, b) ((r>>3) | ((b>>3) << 11) | ((g >> 2) << 5))

void console_init();
void setpixel(int x, int y, unsigned short color);