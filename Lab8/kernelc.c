
#include "kprintf.h"
#include "console.h"
#include "disk.h"

const char* decl = 
"IN CONGRESS, July 4, 1776.\n\n"
"The unanimous Declaration of the thirteen united States of America,\n\n"
"When in the Course of human events, it becomes necessary for one people to "
"dissolve the political bands which have connected them with another, "
"and to assume among the powers of the earth, the separate and equal station to "
"to which the Laws of Nature and of Nature's God entitle them, a decent "
"respect to the opinions of mankind requires that they should declare the "
"causes which impel them to the separation. \n\n"
"We hold";

void kmain(){
    console_init();
	disk_init();
	unsigned data [512];
	for (int i = 0; i < 512; i++) {
		data[i] = 0;
	}
	disk_read_sector(1, (void*)data);
    kprintf("%s",(char*)data);
	disk_write_sector(2, (void*)decl);
	kprintf("%s","\n\n\n");
	disk_read_sector(2, (void*)data);
	kprintf("%s",(char*)data);
    while(1){
    }
}
