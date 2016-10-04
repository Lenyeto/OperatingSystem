
#include "disk.h"
#include "kprintf.h"
#include "errno.h"
#pragma pack(push,1)

#define MAX_FILES 16
static File file_table[MAX_FILES];

struct Superblock{
    unsigned inode_count;       //total # of inodes
    unsigned block_count;       //total # of blocks
    unsigned r_block_count;     //# reserved blocks
    unsigned free_block_count;  //# free blocks
    unsigned free_inode_count;  //# free inodes
    unsigned first_data_block;  //block where fs starts
    unsigned logical_block_size;    //block size = 2**(10+logical_block_size)
    unsigned logical_fragment_size; //unused
    unsigned blocks_per_group;      //total # of blocks per group
    unsigned fragments_per_group;   //unused
    unsigned inodes_per_group;
    unsigned mounttime;         //when fs last mounted
    unsigned writetime;         //when fs last written to
    unsigned short mountcount;  
    unsigned short maxmountcount;
    unsigned short magic;       //0xef53
    unsigned short state; 
    unsigned short errors;
    unsigned short minorrev;
    unsigned lastcheck;
    unsigned checktime;
    unsigned creator;
    unsigned revision;
    unsigned short resuid;
    unsigned short resgid;
    unsigned first_inode;
    unsigned short inode_size;
    unsigned short block_group_number;  //used for copies of superblock
    unsigned feature_compat;
    unsigned feature_incompat;
    unsigned feature_ro_compat;
    unsigned char uuid[16];
    char volname[16];
    char lastmount[64];
    char reserved[824];
};

struct BlockGroupDescriptor{
    unsigned block_bitmap;
    unsigned inode_bitmap;
    unsigned inode_table;
    unsigned short free_blocks;
    unsigned short free_inodes;
    unsigned short used_dirs;
    unsigned short pad;
    char reserved[12];
};

struct Inode {
    unsigned short mode;
    unsigned short uid;
    unsigned size;
    unsigned atime;
    unsigned ctime;
    unsigned mtime;
    unsigned dtime;
    unsigned short gid;
    unsigned short links;
    unsigned blocks;
    unsigned flags;
    unsigned osd1;
    unsigned direct[12];
    unsigned indirect;
    unsigned doubleindirect;
    unsigned tripleindirect;
    unsigned generation;
    unsigned fileacl;
    unsigned diracl;
    unsigned osd2;
    char reserved[12];
};

struct DirEntry{
    unsigned inode;
    unsigned short rec_len;
    unsigned short name_len;
    char name[1];   //might be longer! Variable size
};

#pragma pack(pop)

struct Superblock superblock;
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


void file_init(){
    
    disk_read_sectors(2,2,&superblock);
    kprintf("Volume label: %.16s\n",superblock.volname);
    kprintf("Free blocks: %d\n",superblock.free_block_count);
    kprintf("Blocks per group: %d\n",superblock.blocks_per_group);
    int num_block_groups = superblock.block_count / BLOCKS_PER_BLOCK_GROUP;
    if( superblock.block_count % BLOCKS_PER_BLOCK_GROUP )
        ++num_block_groups;
    kprintf("Num block groups: %d\n",num_block_groups);
    int i;
    
    for(i=0;i<num_block_groups;++i){
        unsigned bg_start = i * superblock.blocks_per_group ;
        //make sure we're in the right place
        read_block(bg_start,block_buffer);
        struct Superblock* sb = (struct Superblock*) (block_buffer + (i==0?1024:0));
        kprintf("Block group %d at block %d: ",i,bg_start);
        kprintf("Vol label: %.16s Magic: %x ",sb->volname,sb->magic);
        read_block(bg_start+1,block_buffer);
        struct BlockGroupDescriptor* bgd = (struct BlockGroupDescriptor*)block_buffer;
        kprintf("Free blocks: %d\n",bgd[i].free_blocks);
    }
}

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
	
	
}

int file_close(int fd) {
	if (fd < 0 | fd >= MAX_FILES) {
		return -EINVAL;
	}
	if (file_table[fd].in_use == 0) {
		return -EINVAL;
	}
	file_table[fd].in_use = 0;
	return SUCCESS;
}