#pragma once

void file_init();


struct File{
    int in_use;
    struct Inode inode;
    int offset;
    int flags; //0 = read, 1 = write, 2 = r/w
};

int file_open(char* fname, int flags);
int file_close(int fd);