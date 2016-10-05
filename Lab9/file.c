
#include "disk.h"
#include "kprintf.h"
#include "errno.h"
#include "file.h"
#pragma pack(push,1)

#define MAX_FILES 16

//static struct File[] file_table = File[MAX_FILES];


#pragma pack(pop)

char block_buffer[4096];

//number of sectors per block. We assume always 4KB blocks.
#define SECTORS_PER_BLOCK 8
#define BYTES_PER_BLOCK 4096

#define BLOCKS_PER_BLOCK_GROUP 32768

#define BLOCK_NUM_TO_SECTOR_NUM(x) (x*SECTORS_PER_BLOCK)
void read_block( unsigned blocknum, void* p){
    disk_read_sectors(BLOCK_NUM_TO_SECTOR_NUM(blocknum), 
        SECTORS_PER_BLOCK, p);
}

struct Superblock superblock;

void file_init(){
    
    disk_read_sectors(2,2,&superblock);
    //kprintf("Volume label: %.16s\n",superblock.volname);
    //kprintf("Free blocks: %d\n",superblock.free_block_count);
    //kprintf("Blocks per group: %d\n",superblock.blocks_per_group);
    int num_block_groups = superblock.block_count / BLOCKS_PER_BLOCK_GROUP;
    if( superblock.block_count % BLOCKS_PER_BLOCK_GROUP )
        ++num_block_groups;
    //kprintf("Num block groups: %d\n",num_block_groups);
    int i;
    
    for(i=0;i<num_block_groups;++i){
        unsigned bg_start = i * superblock.blocks_per_group ;
        //make sure we're in the right place
        read_block(bg_start,block_buffer);
        //struct Superblock* sb = (struct Superblock*) (block_buffer + (i==0?1024:0));
        //kprintf("Block group %d at block %d: ",i,bg_start);
        //kprintf("Vol label: %.16s Magic: %x ",sb->volname,sb->magic);
        read_block(bg_start+1,block_buffer);
        //struct BlockGroupDescriptor* bgd = (struct BlockGroupDescriptor*)block_buffer;
        //kprintf("Free blocks: %d\n",bgd[i].free_blocks);
    }
}
/*
int file_open(char* fname, int flags) {
	int i;
	for (i = 0; i <= MAX_FILES; i++) {
		if (i == MAX_FILES + 1) {
			return -EMFILE;
		}
		if (file_table[i].in_use == 0) {
			break;
		}
	}
	files_table[i].in_use = 1;
	files_table[i].flags = flags;
	//files_table[i].inode = read_block();
	//files_table[i].offset = 0;
	return -SUCCESS;
}

int file_close(int fd) {
	if ((fd < 0) | (fd >= MAX_FILES)) {
		return -EINVAL;
	}
	if (file_table[fd].in_use == 0) {
		return -EINVAL;
	}
	file_table[fd].in_use = 0;
	return -SUCCESS;
}
*/