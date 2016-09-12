# include "console.h"
 
const char* bigstring = "When in the course of human. No.\f"
    "We the people of. Oh crud.\f"
    "|       |       |       |       |       |       |       |       |       |       |\n"
    "Fourscore and\tseven years ago, our\tforefathers brought forth upon this continent\n"
    "a new \tNATION conceived in libertarian\b\b\b\b\by and dedicated to\tthe\tNOTION\tthat\n\n"
    "all men\b\b\bpeople are created equal.\n\n\n"
    "Now\n\twe\n\t\tare\n\t\t\tengaged.....................in...............a.........large,"
    "egregious, massive, overbearing " 
    "grate\b\b\beat civil strife, \t\t\t\t\t\ttesting whether that "
    "nation or fooby"
    "any\b\b\b\b\b\b\b\bany "
    "nation can so endure.";
     
void kmain(){
    console_init();
    const char* p = bigstring;
    while(*p){
        console_putc(*p++);
    }
    while(1){
    }
}