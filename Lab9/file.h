#pragma once

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

struct File{
    int in_use;
    struct Inode inode;
    int offset;
    int flags; //0 = read, 1 = write, 2 = r/w
};


void file_init();



int file_open(char* fname, int flags);
int file_close(int fd);

void read_block( unsigned blocknum, void* p);