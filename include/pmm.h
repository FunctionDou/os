/*************************************************************************
    > File Name: include/pmm.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月24日 星期三 21时50分50秒
 ************************************************************************/

#ifndef INCLUDE_PMM_H_
#define INCLUDE_PMM_H_

#include "types.h"
#include "multiboot.h"

#define STACK_SIZE 8192			/* 线程栈大小 */ 
#define PMM_MAX_SIZE 0x20000000		/* 512M的大小 */ 
#define PAGE_SIZE 0x1000			/* 页大小为4k */ 
#define PAGE_MAX_SIZE (PMM_MAX_SIZE / PAGE_SIZE)	/* 512M可分配的页数量 */ 

extern uint32_t kern_stack_top;

// 内核文件在内存中的起始和结束位置
extern uint8_t kern_start[];
extern uint8_t kern_end[];

// 页的个数
extern uint32_t phy_page_count;

// 输出BIOS中物理内存的布局
void show_memory_map();

// 初始化内存
void init_mmp();

// 释放页
void free_page();

// 申请一个页
uint32_t alloc_page();

#endif

