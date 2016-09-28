#pragma once


typedef struct va_list_ {
	void* current;
} va_list;


#define va_start(v, arg) v.current = ((char*)&arg+sizeof(arg))
#define va_end(v)
#define va_arg(v, t) *((t*)va_arg_(&v, sizeof(t)))

static void * va_arg_(va_list* v, int sz) {
	void * tmp = v->current;
	v->current+=sz;
	return tmp;
}