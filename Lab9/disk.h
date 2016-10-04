#pragma once

void disk_init();
void disk_read_sector(unsigned sector, void* datablock);
void disk_read_sectors(unsigned sector, unsigned count, void* datablock);
void disk_write_sector(unsigned sector, const void* datablock);


