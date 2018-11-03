#include "ASCII.h"
#include "idt.h"
#include "io.h"

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT   0x60

// IRQ1 : 键盘中断
// 端口60 : 命令端口
// 端口64 : 数据端口
void irq21_handler(pt_regs_t *regs)
{
    uint8_t status = inb(KEYBOARD_STATUS_PORT);
    if(status & 0x1)
    {
	char keycode = inb(KEYBOARD_DATA_PORT);
	if(keycode < 0)
	    return ;
	printk("%c", keyboard_map[keycode]);
    }

    // printk_color(rc_black, rc_red, "IRQ1\n");
}
