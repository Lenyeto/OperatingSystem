
#include "kprintf.h"
#include "console.h"
#include "disk.h"
#include "file.h"
#include "util.h"


char b[512];
void kmain(){
    console_init();
    disk_init();
    file_init();
	
	//Creates and reads in sectors to the list of data.
	char Data[1024];
	disk_read_sectors(2, 2, Data);
	
	
	//Creates the super block from the data gotten from the sectors.
	struct Superblock * superBlock;
	superBlock = (struct Superblock *) Data;
	
	
	//Prints the volume name of the super block;
	kprintf("Volume label: %s \n", superBlock->volname);
	
	//Creates a buffer for storing the data from the node. Then reads in the block.
	char buffer[4096];
	read_block(4, buffer);
	
	//Creates an Inode, then fills it with the data from the buffer.
	struct Inode * I;
	I = (struct Inode *) buffer;
	
	//Creates the root node from the Inode created earlier.
	struct Inode root_inode;
	kmemcpy(&root_inode, &(I[1]), sizeof(struct Inode));
	
	//Reads in the directory entry into a newly created DirEntry struct instance.
	read_block(root_inode.direct[0], buffer);
	struct DirEntry * dir;
	dir = (struct DirEntry *) buffer;
	
	//Loops to find all the files in the root directory.
	unsigned x = 0;
	while (x < root_inode.size) {
		kprintf("Inode: %d\t%s\n", dir->inode, dir->name);
		x += dir->rec_len;
		dir = (struct DirEntry *)(buffer + x);
		if (dir->rec_len == 0)
			break;
	}
	
	//Prints that it is done reading from the root directory.
	kprintf("End of root directory.");
	
    while(1){
		
    }
}


