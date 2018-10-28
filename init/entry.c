/*************************************************************************
    > File Name: entry.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 13时37分55秒
 ************************************************************************/
#include "console.h"
#include "debug.h"
#include "idt.h"
#include "gdt.h"
#include "timer.h"
#include "pmm.h"

int kern_entry()
{
   init_debug();
   init_gdt();
   init_idt();

   console_clear();
   printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
    
   init_timer(200);

   printk("kernel memory start : %#x\n", kern_start);
   printk("kernel memory end : %#x\n", kern_end);
   printk("kernel memory used : %dKB\n", (kern_end - kern_start + 1023) / 1024);
   show_memory_map();

   init_mmp();

   uint32_t alloc = 0;
   printk_color(rc_black, rc_blue, "stack\n");
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);

   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);

/* 
    asm volatile ("sti");
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
*/ 
    return 0;
}
