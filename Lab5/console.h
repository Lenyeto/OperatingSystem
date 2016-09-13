#define WIDTH 800
#define HEIGHT 600

#define COLOR16(r, g, b) ((r>>3) | ((b>>3) << 11) | ((g >> 2) << 5))
#define framebuffer ((volatile unsigned short*) (((0x07ffffff - WIDTH*HEIGHT*2))&~0xf))

#define pl110 ( (volatile unsigned*)0xc0000000 )
#define haxis       (*(pl110+0))    //offset 0: horizontal axis                             
#define vaxis       (*(pl110+1))    //offset 4: vertical axis                               
#define polarity    (*(pl110+2))    //offset 8: clock+polarity 
#define lineend     (*(pl110+3))    //offset 12: line end
#define baseaddr1   (*(pl110+4))    //offset 16: upper panel base address                           
#define baseaddr2   (*(pl110+5))    //offset 20: lower panel base address
#define intmask     (*(pl110+6))    //offset 24: interrupt mask
#define params      (*(pl110+7))    //offset 28: panel parameters 


int console_pos_x;
int console_pos_y;

unsigned int curColor;

void console_init();
void setpixel(int x, int y, unsigned short color);
void drawbox(int x, int y, int width, int height, unsigned short color);
void console_draw_character(int x, int y, unsigned int color, char c);
void console_putc(char c);
void console_clear();