/*************************************************************************
    > File Name: include/gdt.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月21日 星期日 12时57分09秒
 ************************************************************************/

#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_

#include  "types.h"

// 全局描述符类型
typedef struct gdt_entry_t
{
	uint16_t limit_low;		/* 0-15 : 段界限 */ 
	uint16_t base_low;		/* 0-15 : 段基地址 */ 
	uint8_t base_middle;	/* 16-23 : 段基地址 */ 
	uint8_t access;			/* 段存在位、描述符特权级、描述符类型、描述符子类别 */ 
	uint8_t granularity;	/* 16-19 : 段界限 */ 
	uint8_t base_high;		/* 24-31 : 段基地址 */
}__attribute__((packed)) gdt_entry_t;


// GDTR
typedef struct gdt_ptr_t
{
	uint16_t limit;		/* 16位段选择符 */ 
	uint32_t base;		/* 32位全局描述符基地址 */ 
}__attribute__((packed)) gdt_ptr_t;

void init_gdt();	/* 初始化全局描述符表 */ 

extern void gdt_flush(uint32_t );	/* GDT添加到GDTR函数 */

#endif
