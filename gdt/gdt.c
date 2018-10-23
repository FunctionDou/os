/*************************************************************************
    > File Name: gdt/gdt.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月21日 星期日 13时21分55秒
 ************************************************************************/


#include "gdt.h"
#include "string.h"

/* 全局描述符长度 : 0段, 指令段, 数据段. 用户代码段, 用户数据段*/ 
#define GDT_LEHGTH 5

/* 全局描述符比表定义 */ 
gdt_entry_t gdt_entries[GDT_LEHGTH];

// 定一个GDTR
gdt_ptr_t gdt_ptr;

// 传入参数 : 数组下标, 基地址, 长度, 访问标志, 其他访问标志
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

extern uint32_t stack;	/* 声明内核栈地址 */ 

// 传入参数 : 数组下标, 基地址, 长度, 访问标志, 其他访问标志
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

// 初始化全局描述符表
void init_gdt()
{
    //设置gdtr寄存器的值,因为是从0开始,所以需要减1, 数组的大小为GDT_LEGTH
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LEHGTH - 1;
    gdt_ptr.base = (uint32_t)gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);		    // 按照 Intel 文档要求，第一个描述符必须全0
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);	    // 指令段
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);	    // 数据段
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);	    // 用户模式代码段
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);	    // 用户模式数据段

    // 加载全局描述符表地址到 GPTR 寄存器
    gdt_flush((uint32_t) & gdt_ptr);
}
