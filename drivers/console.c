/*************************************************************************
    > File Name: drivers/console.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 19时40分46秒
 ************************************************************************/

#include "console.h"
#include "io.h"

// 绑定VGA的入口地址
static uint16_t *video_memory = (uint16_t *) 0xB8000;

/* 光标的位置, 先设置为(0,0)坐标 */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

/* 设置黑色, 用来覆盖屏幕, 达到清除字符的作用 */
uint16_t blank = 0x20 | ((0 << 4 | (15 & 0x0F)) << 8);

/* 设定显示的长和宽 */ 
#define WIDTH 80
#define HIGH 25

/* 设置光标的高位和低位 */
#define VGA_CURSOR_HIGH 14
#define VGA_CURSOR_LOW 15

/* 光标设置的端口 */ 
#define VGA_PORT_0x3D4 0x3D4
#define VGA_PORT_0x3D5 0x3D5

#define BLACK 0x08  /* 0x08 : 退格键的扫描码 */ 
#define TAB 0x09    /* 0x09 : tab键的扫描码 */ 

/* 光标的位置, 可以实现移动光标 */ 
static void update_cursor()
{
    /* 光标的位置更新 */
    uint16_t pos = cursor_y * WIDTH + cursor_x;

    outb(VGA_PORT_0x3D4, VGA_CURSOR_HIGH);  /* 设置VGA的高字节 */
    outb(VGA_PORT_0x3D5, pos >> 8);	    /* pos的高8位 */ 
    outb(VGA_PORT_0x3D4, VGA_CURSOR_LOW);   /* 设置VGA的低字节 */
    outb(VGA_PORT_0x3D5, pos);		    /* pos的低8位 */
    
}

/* 清屏 */ 
/* 用黑来覆盖整个屏幕, 并且重设光标的位置 */
void console_clear()
{
    int i;
    for (i = 0; i < WIDTH * HIGH; i++)
	video_memory[i] = blank;

    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

/* 设定滑动 */
/* 将下一行的数据覆盖掉上一行的数据, 然后将最后一行的数据用黑色来覆盖, 最后在重新设置光标的位置, 实现滑动 */
void screen_scrolling()
{
    if(cursor_y >= HIGH)
    {
	int i;
	for(i = 0; i < (HIGH - 1) * WIDTH; i++)
	    video_memory[i] = video_memory[i + WIDTH];
	for(i = (HIGH - 1) * WIDTH; i < HIGH * WIDTH; i++)
	    video_memory[i] = blank;

	cursor_y = HIGH - 1;
    }

}

/* 输出有颜色的一个字符 */ 
void console_putc_color(const char c, real_color_t back, real_color_t fore)
{
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;
    uint16_t color = (back_color << 4 | (fore_color & 0x0F)) << 8;

    if(c == BLACK && cursor_x)
	cursor_x--;
    else if(c == TAB)
	cursor_x = (cursor_x + 8)& ~(8 - 1);
    else if(c == '\r')
	cursor_x = 0;
    else if(c == '\n')
    {
	cursor_x = 0;
	cursor_y++;
    }
    else if(c >= ' ')
    {
	video_memory[cursor_x + cursor_y * WIDTH] = c | color;
	cursor_x++;
    }

    if(cursor_x >= WIDTH)
    {
	cursor_x = 0;
	cursor_y++;
    }
    
    screen_scrolling();
    update_cursor();
}

/* 输出一个默认颜色的字符 */ 
void console_write(const char *cstr)
{
    while(*cstr)
	console_putc_color(*cstr++, rc_black, rc_white);
}

/* 输出一个有颜色的字符串 */
void console_write_color(const char *cstr, real_color_t back, real_color_t fore)
{
    while(*cstr)
	console_putc_color(*cstr++, back, fore);
}

/*
void console_write_hex(const uint32_t n, real_color_t back, real_color_t fore)
{
    char ch;
    char i = 24;
    while(i >= 0)
    {
	ch = (char)n >> i;
	console_putc_color(ch, back, fore);
	i -= 8;
    }
    console_write_color(&n, back, fore);
}
*/
