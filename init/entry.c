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

int kern_entry()
{
    init_debug();
    init_gdt();
    init_idt();

    console_clear();
    printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
    
    init_timer(200);


    asm volatile ("sti");
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    return 0;
}
