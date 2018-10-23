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

void timer_callback(pt_regs_t *regs)
{
	static uint32_t tick = 0;
	printk_color(rc_black, rc_blue, "timer %d\n", tick);
	tick++;
}

void init_timer(uint32_t frequency)
{
	register_interrupt_handler(IRQ0, timer_callback);
	uint32_t divisor = 1193180 / frequency;
	
	outb(0x43, 0x36);

	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, low);
	outb(0x40, high);
}


