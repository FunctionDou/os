/*************************************************************************
    > File Name: entry.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 13时37分55秒
 ************************************************************************/
#include "console.h"
#include "debug.h"
#include "vargs.h"

void pp(char *s, ...)
{
    va_list varg;
    va_start(varg, s);
    char c = va_arg(varg, char );
    console_putc_color(c, rc_black, rc_white);
}

int kern_entry()
{
    console_clear();
    int i;
    for(i = 0; i < 25; i++)
        console_write_color("hello world ", rc_black, rc_red);
    console_write_color("hello\n", rc_black, rc_red);

    int num = 4;
    char c = 'd';
    char s[] = "asdaf";
    char *t = "1";
    printk("%.3s %s %#x %d %c %s\n", s, s, num, num, c, s);
    /*
    printk("%d\n", isdigit('1'));
    printk("%d\n", atoi("2"));
    printk("%d\n", isdigit('3'));
    printk("%d\n",atoi("3"));
    */

    return 0;
}
