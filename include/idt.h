/*************************************************************************
    > File Name: include/idt.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月21日 星期日 16时27分18秒
 ************************************************************************/

#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_

#include "types.h"

void init_idt();

typedef struct idt_entry_t
{
	uint16_t base_lo;	/* 中断处理函数的低16位地址 */ 
	uint16_t base_hi;	/* 中断处理函数的高16位地址 */ 
	uint16_t sel;		/* 目标代码选择子 */ 
	uint8_t always0;	/* 置0段 */ 
	uint8_t flags;		/* 4位的type, 2位的DPL, 1位的P */ 
}__attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_t
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed)) idt_ptr_t;

