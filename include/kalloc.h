/*************************************************************************
    > File Name: include/kalloc.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月30日 星期二 11时13分10秒
 ************************************************************************/

#ifndef INCLUDE_KALLOC_H_
#define INCLUDE_KALLOC_H_ 

#include "types.h"

typedef struct header
{
	struct header *prev;	// 前后内存块管理结构指针
	struct header *next;
	uint32_t allocated : 1;		// 该内存块是否已经被申请
	uint32_t length : 31;		// 当前内存块的长度
}header_t;

// 初始化分配器
void init_alloc();

// 内存分配
void *kmalloc(uint32_t );

// 内存释放
void kfree(void *);

// 测试内核堆的释放
void test_alloc();

#endif

