#include "console.h"

void kmain(){
    console_init();
   
	
	//setpixel(10, 10, COLOR16(255, 255, 255));
    
	//drawbox(10, 10, 100, 100, COLOR16(255, 255, 255));
	console_draw_character(150, 150, COLOR16(255, 255, 255), 'z');
	console_draw_character(250, 150, COLOR16(255, 255, 255), 'Z');
	console_draw_character(350, 150, COLOR16(255, 255, 255), 'a');
	console_draw_character(450, 150, COLOR16(255, 255, 255), 'A');

    while(1){
    }
}

