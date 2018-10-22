/*************************************************************************
    > File Name: entry.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 13时37分55秒
 ************************************************************************/
#include "console.h"
#include "debug.h"
#include "vargs.h"
#include "elf.h"

int kern_entry()
{
    init_debug();
    init_gdt();
    console_clear();
    printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
/*
    int i;
    int num = 4;
    char c = 'd';
    char s[] = "asdaf";
    char *t = "1";
    printk_color(rc_black, rc_red, "%.3s %s %#x %d %c\n", s, s, num, num, c, s);
    printk("%.3s %s %#x %d %c %s\n", s, "fangwenyuejie", num, num, c, s);

    printk("%d\n", isdigit('0'));
*/
    return 0;
}
