/*************************************************************************
    > File Name: idt/idt.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月22日 星期一 17时05分27秒
 ************************************************************************/

#include "idt.h"
#include "io.h"
#include "string.h"
#include "debug.h"

#define INTERRUPTMAX 256

// 初始化外部中断
static void init_IRQ();

// 设置中断描述符
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);



// 中断向量表
static idt_entry_t idt_entries[INTERRUPTMAX];

// 中断处理函数
static interrupt_handler_t interrupt_handlers[INTERRUPTMAX];

// 中断向量表指针
static idt_ptr_t idt_ptr;




// 初始化回调函数
void register_interrupt_handler(uint8_t n, interrupt_handler_t h)
{
    // 将中断向量表中对应的中断号设置成指向我们写好的回调函数的函数地址
    interrupt_handlers[n] = h;
}

// 中断处理函数
void isr_handler(pt_regs_t *regs)
{
    if(interrupt_handlers[regs->int_no])
	interrupt_handlers[regs->int_no](regs);
    else
	printk_color(rc_black, rc_red, "Unhandled interrupt : %d\n", regs->int_no);
}

// 外部中断处理函数
void irq_handler(pt_regs_t *regs)
{
    // 发送中断结束信号给 PICs
    // 按照我们的设置，从 32 号中断起为用户自定义中断
    // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
    // 故大于等于 40 的中断号是由从片处理的

    // 初始化中断向量
    if(regs->int_no >= 40)
    {
	outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    if(interrupt_handlers[regs->int_no])
    {
	interrupt_handlers[regs->int_no](regs);
    }
}

// 设置中断描述符
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;

    /* 
     * 先留下 0x60 这个魔数，以后实现用户态时候
     * 这个与运算可以设置中断门的特权级别为 3
     */
    idt_entries[num].flags = flags;
}

// 初始化外部中断
void init_IRQ()
{
    // 重新映射 IRQ 表
    // 两片级联的 Intel 8259A 芯片
    // 主片端口 0x20 0x21
    // 从片端口 0xA0 0xA1
        
    // 初始化主片、从片
    // 0001 0001
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // 设置主片 IRQ 从 0x20(32) 号中断开始
    outb(0x21, 0x20);

    // 设置从片 IRQ 从 0x28(40) 号中断开始
    outb(0xA1, 0x28);
    
    // 设置主片 IR2 引脚连接从片
    outb(0x21, 0x04);

    // 告诉从片输出引脚和主片 IR2 号相连
    outb(0xA1, 0x02);
    
    // 设置主片和从片按照 8086 的方式工作
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // 设置主从片允许中断
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    outb(0x21, 0xFD);
}

// 中断向量初始化
void init_idt()
{
    init_IRQ();

    bzero((uint8_t *)&interrupt_handlers, sizeof(interrupt_handler_t) * INTERRUPTMAX);
    idt_ptr.limit = sizeof(idt_entry_t) * INTERRUPTMAX - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    bzero((uint8_t *)&idt_entries, sizeof(idt_entry_t) * INTERRUPTMAX);

     // 0-32:  用于 CPU 的中断处理
    idt_set_gate( 0, (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    idt_set_gate(255, (uint32_t)isr255, 0x80, 0x8E);

    register_interrupt_handler(IRQ1, &irq21_handler);

    idt_flush((uint32_t) & idt_ptr);
}

