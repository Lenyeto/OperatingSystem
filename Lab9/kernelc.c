
#include "kprintf.h"
#include "console.h"
#include "disk.h"
#include "file.h"


char b[512];
void kmain(){
    console_init();
    console_box( 20,40,100,200 );
    disk_init();
    file_init();
    while(1){
		
    }
}


