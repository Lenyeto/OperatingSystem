#include "disk.h"

#define MMCP_START ((volatile unsigned*) 0x1c000000 ) 
#define POWER ( MMCP_START)
#define CLOCK (MMCP_START+1)
#define ARG (MMCP_START+2)
#define CMD (MMCP_START+3)
#define RESPONSE_CMD (MMCP_START+4)
#define RESPONSE (MMCP_START+5)
#define DATA_TIMER (MMCP_START+9)
#define DATA_LENGTH (MMCP_START+10)
#define DATA_CONTROL (MMCP_START+11)
#define DATA_COUNTER (MMCP_START+12)
#define STATUS (MMCP_START+13)
#define CLEAR (MMCP_START+14)
#define INTERRUPT0_MASK (MMCP_START+15)
#define INTERRUPT1_MASK (MMCP_START+16)
#define SELECT (MMCP_START+17)
#define FIFO_COUNT (MMCP_START+18)
#define DATA_FIFO (MMCP_START+32)

void disk_init() {
	*POWER = 3;
	*CLOCK = 8;
	*CMD = (1 << 10);
	*CMD = 8 | (1 << 10);
	do {
		*CMD = 55 | (1 << 10) | (1 << 6);
		*ARG = 0xffffffff;
		*CMD = 41 | (1 << 10) | (1 << 6);
	} while (isBusy());
	*CMD = 2 | (1 << 10) | (1 << 6) | (1 << 7);
	*CMD = 3 | (1 << 10) | (1 << 6);
	unsigned relative_address = *RESPONSE;
	*ARG = relative_address;
	*CMD = 7 | (1 << 10) | (1 << 6);
}

int isBusy() {
	return *STATUS&(1<<24);
}

void disk_read_sector(unsigned sector, void* datablock) {
	*DATA_LENGTH = 512;
	*DATA_TIMER = 100;
	*DATA_CONTROL = 1 | (1<<1) | (9<<4);
	*ARG = 512*sector;
	*CLEAR = 0x3ff;
	*CMD = 17 | (1<<10) | (1<<6);
	unsigned* p = (unsigned *)datablock;
	int k;
	for (k = 0; k < 128; ++k) {
		while(!(*STATUS&(1<<21))) {
			;
		}
		*CLEAR = 0x3ff;
		unsigned v = *DATA_FIFO;
		*p++ = v;
	}
}

void disk_write_sector(unsigned sector, void* datablock) {
	*DATA_LENGTH = 512;
	*DATA_TIMER = 100;
	*DATA_CONTROL = (1<<0) | (9<<4);
	*ARG = 512*sector;
	*CLEAR = 0x3ff;
	*CMD = 24 | (1 << 10) | (1 << 6);
	unsigned* p = (unsigned*)datablock;//The data to write
	int k;
	for (k = 0; k < 128; ++k) {
		while ((*STATUS&(1<<20))) {
			;
		}
		*CLEAR = 0x3ff;
		*DATA_FIFO = *p;
		p++;
	}
}

///READING STATUS & (1<<19) CHANGE TO 21

