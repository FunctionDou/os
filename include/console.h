/*************************************************************************
    > File Name: include/console.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 19时18分54秒
 ************************************************************************/

#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_ 

#include "types.h"

/* K R G B I R G B */ 
/* 8位, 前四位设定前景色, 后四位设定背景色 8 */
/* K = 0 设置闪烁 k = 1不闪烁； I = 0 深色, I = 1 浅色 */
typedef enum real_color
{
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown  = 14,   // yellow
    rc_white = 15
} real_color_t;

void console_clear();   /* 清屏 */ 

void console_putc_color(const char c, real_color_t back, real_color_t fore); /* 向屏幕输出一个有颜色的字符 */ 

void console_write(const char *sctr); /* 输出一个默认颜色的字符串 */ 

void console_write_color(const char *cstr, real_color_t back, real_color_t fore); /* 向屏幕输出一个有颜色的字符串 */ 

void console_write_hex(const uint32_t n, real_color_t back, real_color_t fore);   /* 向屏幕输出一个有颜色的十六进制的整数 */ 

void console_wrire_dec(const uint32_t n, real_color_t back, real_color_t fore);   /* 向屏幕输出一个有颜色的十进制的整数 */

#endif 
