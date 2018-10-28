/*************************************************************************
    > File Name: debug.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月17日 星期三 10时28分29秒
 ************************************************************************/

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_

#include "types.h"
#include "console.h"
#include "string.h"
#include "vargs.h"
#include "elf.h"

#define assert(x, info) \
	do { \
		if(!(x))	\
		panic(info);	\
	}while(0)

#define static_acssert(x)	\
switch(x)\
{\
	case 0:\
	case (x): \
		   ;\
}

void init_debug();

void print_cur_status();

void panic(const char *msg);

int printk(const char *fmt, ...);

int printk_color(real_color_t back, real_color_t fore, const char *fmt, ...);

int vprintk(char *buf, const char *fmt, va_list varg);

#endif
