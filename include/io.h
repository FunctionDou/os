/*************************************************************************
    > File Name: include/io.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 18时53分09秒
 ************************************************************************/

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_ 

#include "types.h"

//端口写一个字节
#define outb(port, value) \
	__asm__ volatile("outb %%al, %%dx" : : "a"(value), "d"(port))

//端口读一个字节
#define inb(port)\
({\
	uint8_t v;\
	__asm__ volatile ("inb %%dx, %%al" : "=a"(v) : "d"(port));\
	v;\
})

//端口读一个字
#define inw(port)\
({\
	uint16_t v;\
	__asm__ volatile ("inw %%dx, %%ax" : "=a"(v) : "d"(port));\
	v;\
})

#endif
