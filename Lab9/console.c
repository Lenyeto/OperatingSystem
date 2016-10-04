#include "font.h"
#include "util.h"


struct PL110MMIO {
    unsigned haxis;         //offset 0: horizontal axis                             
    unsigned vaxis;         //offset 4: vertical axis                               
    unsigned polarity;      //offset 8: clock+polarity                              
    unsigned lineend;       //offset 12: line end
    unsigned baseaddr1;     //offset 16: upper panel base address                           
    unsigned baseaddr2;     //offset 20: lower panel base address
    unsigned intmask;       //offset 24: interrupt mask
    unsigned params;        //offset 28: panel parameters   
};

#define WIDTH 800
#define HEIGHT 600

#define framebuffer ((volatile unsigned short*) (((0x07ffffff - WIDTH*HEIGHT*2))&~0xf)) //0x200000;
#define pl110 ( (volatile struct PL110MMIO*)0xc0000000 )

//input: r,g,b in range 0...255 
//output: 16 bit color with 565 weighting
#define COLOR16(r,g,b) ( ((b&0xf8)<<8) | ((g & 0xfc)<<3) | ((r&0xf8)>>3) )


static int current_row=0;
static int current_col=0;
static int current_color=COLOR16(128,128,128);


void console_init(){
  
    //To get screen width w: 0x3f1f3f00 | (w/4-4)
    //ex: 640 = 0x3f1f3f9c
    pl110->haxis = 0x3f1f3f00 | (WIDTH/ 4-4);
    
    //To get screen height v: 0x80b600 | (v-1) 
    //Ex: 480 = 0x80b61df
    pl110->vaxis = 0x80b6000 | (HEIGHT-1)   ;
    
    pl110->polarity = 0x067f3800;
    pl110->baseaddr1 = (unsigned) framebuffer;
    
    //value         bpp
    //0x1821        1
    //0x1823        2
    //0x1825        4
    //0x1827        8
    //0x1829        16
    //0x182b        24
    pl110->params = 0x1829;
    
}

void set_pixel(int x, int y, unsigned short color){
    framebuffer[ y*WIDTH+x ] = color;
}


void console_box( int x, int y, int w, int h ){
    int i;
    unsigned short color = COLOR16( 0,128,255 );
    
    for(i=0;i<w;++i){
        set_pixel(x+i,y,color);
        set_pixel(x+i,y+h,color);
    }
    for(i=0;i<h;++i){
        set_pixel(x,y+i,color);
        set_pixel(x+w,y+i,color);
    }
}

void console_set_pixel(int x, int y, unsigned short color){
    framebuffer[ y*WIDTH+x ] = color;
}

void console_draw_character(int x, int y, unsigned short color, char c){
    int i,j;
    for(i=0;i<CHAR_HEIGHT;++i){
        for(j=0;j<CHAR_WIDTH;++j){
            if( font_data[(int)c][i] & (MASK_VALUE >> j) ) {
                    framebuffer[ (y+i)*WIDTH + (x+j) ] = color;
            }
        }
    }
}


#define COLUMNS (WIDTH/CHAR_WIDTH)
#define ROWS (HEIGHT/CHAR_HEIGHT)

void console_putc(char ch){
    volatile unsigned* serialport = (unsigned*) 0x16000000;
    volatile unsigned* serialflags = (unsigned*) 0x16000018;
    while( *serialflags & 32 )
        ;
    *serialport = ch;
    
    if( ch == '\n' ){
        current_col=0;
        current_row++;
    }
    else if( ch == '\t' ){
        int tmp = current_col & 7;
        tmp = 8-tmp;
        current_col += tmp;
    }
    else if( ch == '\001' )
        current_color = COLOR16(255,255,255);
    else if( ch == '\002' )
        current_color = COLOR16(128,128,128);
    else{
        console_draw_character(current_col*CHAR_WIDTH,current_row*CHAR_HEIGHT,current_color,ch);
        current_col++;
    }

    
    if( current_col >= COLUMNS ){
        current_col -= COLUMNS;
        current_row++;
    }

    if( current_row >= ROWS ){
        kmemcpy( 
            (void*)framebuffer, 
            (void*)(framebuffer+CHAR_HEIGHT*WIDTH), 
            (ROWS-1)*CHAR_HEIGHT*WIDTH*2 );
        kmemset( (void*)(framebuffer+ (ROWS-1) * CHAR_HEIGHT * WIDTH), 0, CHAR_HEIGHT*WIDTH*2);
        current_row = ROWS-1;
    }
}

