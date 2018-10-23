/*************************************************************************
    > File Name: kernel/debug/printk.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月16日 星期二 17时32分42秒
 ************************************************************************/

#include "debug.h"

#define PER_SYM 1<<0
#define FLUSH_LEFT 1<<1
#define FLUSH_RIGHT 1<<2
#define NUMBER_SIGN 1<<3
#define POINT 1<<4

#define HEX 16

const char hex[] = "0123456789abcdef";

int number_sign(char *buf, const char ch);
int spit_int(int num, char *fmt);
int sipt_hex(int num, char *buf);

int printk(const char *fmt, ...)
{
    char buf[SIZE];
    int retnum;
    va_list varg;

    va_start(varg, fmt);
    retnum = vprintk(buf, fmt, varg);

    va_end(varg);
    buf[retnum  + 1] = '\0';
    console_write(buf);

    return retnum;
}

int printk_color(real_color_t back, real_color_t fore, const char *fmt, ...)
{
    char buf[SIZE];
    int renum;
    va_list varg;

    va_start(varg, fmt);
    renum = vprintk(buf, fmt, varg);
    va_end(varg);
    buf[renum] = '\0';
    char *p;
    console_write_color(buf, back, fore);
    /*
    int tmp = renum;
    tmp = spit_int(tmp, p);
    p[tmp] = '\0';
    console_write(p);
*/ 
    return renum;
}

// printf %c %d %s %- %+ %m
int vprintk(char *buf, const char *fmt, va_list varg)
{
    char ch;	/* 保存 char 类型的参数 */ 
    int len;	/* 保存函数的返回值 */
    int flags = 0;
    int num;	/* 保存int参数 */
    int m;	/* m 用来保存 %m 中m的值 */
    int i;


    char *ptr_buf = NULL;   /* 保存 char * 参数 */ 
    char *str = NULL;

    /* 将 " " 进行一个一个翻译并保存在buf缓冲区中 */
    for(ptr_buf = buf; *fmt;)
    {
	if(*fmt != '%')
	{
	    *ptr_buf++ = *fmt++;
	    continue;
	}

	/* %号后面的特殊参数 %- %# %. */
	switch(*++fmt)
	{
	    case '-':
		flags |= FLUSH_LEFT;
		break;
	    case '#':
		flags |= NUMBER_SIGN;
		break;
	    case '.':
		flags |= POINT;
		break;
	}

	/* 判断%.m 并将m的值保存在变量m中 */
	if((flags & POINT) && isdigit(*++fmt))
	{
	    m = 0;
loop:
	    if(*fmt >= '0' && *fmt <= '9')
	    {
		m = *fmt - '0' + m * 10;
		fmt++;
		goto loop;
	    }
	}

	/* 判断%#, 并执行往缓冲区追加0x字符串 */
	if(flags & NUMBER_SIGN)
	{
	    len = number_sign(ptr_buf, *++fmt);
	    ptr_buf += len;
	}
    
	/* 执行 %d %s %c 功能 */ 
	switch(*fmt)
	{
	    case 'd':
		num = va_arg(varg, int);
		/* 将int转为char *, 并返回长度  */
		len = spit_int(num, ptr_buf);
		ptr_buf += len;
		break;
	    case 'c':
		*ptr_buf++ = va_arg(varg, char );
		break;
	    case 's':
		str = va_arg(varg, char *);
		len = strlen(str);
		num = (m ? (m > len ? len : m) : len);
		/*
		strncpy(ptr_buf, str, num);
		ptr_buf += num;
		*/
		for(i = 0; i < num; i++)
		    *ptr_buf++ = *str++;
		m = 0;
		break;
	    case 'x':
		num = va_arg(varg, int);
		len = spit_hex(num , ptr_buf);
		ptr_buf += len;
		break;
	}
	fmt++;
	flags = 0;
    }

    *ptr_buf = '\0';
    return ptr_buf - buf;
}

int spit_int(int num, char *fmt)
{
    char bitint[BITINT];
    int i, itmp;
    for(i = 0; num; i++)
    {
	bitint[i] = num % 10;
	num /= 10;
    }
    itmp = i;
    while(--i >= 0)
	*fmt++ = bitint[i] + '0';
    return itmp;
}

int spit_hex(int num, char *buf)
{
    int i, tmp;
    char bithex[BITINT];
    for(i = 0; num; i++)
    {
	bithex[i] = hex[num % HEX];
	num /= HEX;
    }
    tmp = i;
    while(--i >= 0)
	*buf++ = bithex[i];
    return tmp;
}

int number_sign(char *buf, const char ch)
{
    int num = 0;
    if(ch == 'x')
    {
	strncpy(buf, "0x", 2);
	num = 2;
    }
    else if(ch == 'o')
    {
	*buf++ = 'o';
	num = 1;
    }
    return num;
}

