#pragma once

void file_init();



int file_open(char* fname, int flags);
int file_close(int fd);