/*************************************************************************
    > File Name: drivers/timer.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月23日 星期二 14时42分41秒
 ************************************************************************/

#include "timer.h"
#include "io.h"
#include "idt.h"
#include "debug.h"
#include "sched.h"

// 中断处理函数
void timer_callback(pt_regs_t *regs)
{
	schedule();
	/*
	static uint32_t tick = 0;
	printk_color(rc_black, rc_blue, "timer %d\n", tick);
	tick++;
	*/
}

void init_timer(uint32_t frequency)
{
	register_interrupt_handler(IRQ0, timer_callback);
	
	// Intel 8253/8254 PIT芯片 I/O端口地址范围是40h~43h
	// 输入频率为 1193180，frequency 即每秒中断次数
	uint32_t divisor = 1193180 / frequency;
	
	// D7 D6 D5 D4 D3 D2 D1 D0
	// 0  0  1  1  0  1  1  0
	// 即就是 36 H
	// 设置 8253/8254 芯片工作在模式 3 下
	outb(0x43, 0x36);

	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	// 分别写入低字节和高字节
	outb(0x40, low);
	outb(0x40, high);
}


