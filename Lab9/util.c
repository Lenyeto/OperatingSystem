#include "util.h"

void kmemcpy(void* dest, void* src, int num){
    char* dp = (char*) dest;
    char* sp = (char*) src;
    while(num > 0 ){
        *dp = *sp;
        dp++;
        sp++;
        num--;
    }
}

void kmemset(void* dest, unsigned char val, int num){
    char* p = (char*) dest;
    while(num > 0 ){
        *p = val;
        p++;
        num--;
    }
}


void memcpy(void* dest, void* src, int num){
    kmemcpy(dest,src,num);
}

void kdivmod(unsigned n, unsigned d, unsigned*qp, unsigned*rp) {
	int i;
	int dividend = 0;
	int Q = 0;
	for (i = 0; i < 32; i++) {
		dividend <<= 1;
		if (n & 0x80000000) {
			dividend |= 1;
		}
		n <<= 1; Q <<= 1;
		if (dividend > d) {
			Q |= 1;
			dividend -= d;
		}
	}
	*qp = Q;
	*rp = dividend;
}

unsigned kdiv(unsigned n, unsigned d) {
	unsigned q, r;
	kdivmod(n, d, &q, &r);
	return q;
}

unsigned kmod(unsigned n, unsigned d) {
	unsigned q, r;
	kdivmod(n, d, &q, &r);
	return r;
}