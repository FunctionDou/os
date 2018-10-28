/*************************************************************************
    > File Name: include/idt.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月21日 星期日 16时27分18秒
 ************************************************************************/

#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_
#include "types.h"

// 中断描述符表
//首先我们定义IDT结构体,成员有:中断处理程序的底16位偏移、选择子、属性等、中断处理程序的高16位偏移
typedef struct idt_entry_t
{
	uint16_t base_lo;			//低16位偏移
	uint16_t sel;				//目标代码段描述符选择子
	uint8_t  always0;			//置0位
	uint8_t  flags;				//有4位的type、1位的S、2位的DPL、1位的P
	uint16_t base_hi;			//高16位偏移
}__attribute__((packed)) idt_entry_t;

//声明idtr寄存器的内容
typedef struct idt_ptr_t
{
	uint16_t limit;				//低16位限长
	uint32_t base;				//高32位基地址
}__attribute__((packed)) idt_ptr_t;

//cpu在处理中断的时候需要保护当前任务的信息，需要保存：
//1.保存中断号,以及错误号(如果有的话) 
//2.通用寄存器 
//3.中断前数据段选择子(硬件会自动为我们保存代码段选择子(CS寄存器))


//这个结构体的成员是处理中断时需要保存的现场信息
typedef struct pt_regs_t
{
	uint32_t ds;		//数据段选择子
	//通用寄存器由pusha指令一次压入
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;	//中断号
	uint32_t err_code;	//错误号
	//以下由硬件压入
	uint32_t cs;
	uint32_t eflags;
	uint32_t user_esp;
	uint32_t ss;
	uint32_t eip;
}pt_regs_t;

//初始化idt
void init_idt();

//irq处理函数
void irq_handler(pt_regs_t *regs);

// 声明加载 IDTR 的函数
extern void idt_flush(uint32_t);

//定义中断处理函数指针
typedef void (*interrupt_handler_t)(pt_regs_t *);

//注册中断处理服务函数
void register_interrupt_handler(uint8_t n, interrupt_handler_t h);

//调用中断处理函数
void isr_handler(pt_regs_t *regs);

void isr0();        // 0 #DE 除 0 异常 
void isr1();        // 1 #DB 调试异常 
void isr2();        // 2 NMI 
void isr3();        // 3 BP 断点异常 
void isr4();        // 4 #OF 溢出 
void isr5();        // 5 #BR 对数组的引用超出边界 
void isr6();        // 6 #UD 无效或未定义的操作码 
void isr7();        // 7 #NM 设备不可用(无数学协处理器) 
void isr8();        // 8 #DF 双重故障(有错误代码) 
void isr9();        // 9 协处理器跨段操作 
void isr10();       // 10 #TS 无效TSS(有错误代码) 
void isr11();       // 11 #NP 段不存在(有错误代码) 
void isr12();       // 12 #SS 栈错误(有错误代码) 
void isr13();       // 13 #GP 常规保护(有错误代码) 
void isr14();       // 14 #PF 页故障(有错误代码) 
void isr15();       // 15 CPU 保留 
void isr16();       // 16 #MF 浮点处理单元错误 
void isr17();       // 17 #AC 对齐检查 
void isr18();       // 18 #MC 机器检查 
void isr19();		// 19 #XM SIMD浮点异常

//20-31 Intel保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

//32-255 用户自定义异常
void isr255();

//定义IRQ
#define  IRQ0     32    // 电脑系统计时器
#define  IRQ1     33    // 键盘
#define  IRQ2     34    // 与 IRQ9 相接，MPU-401 MD 使用
#define  IRQ3     35    // 串口设备
#define  IRQ4     36    // 串口设备
#define  IRQ5     37    // 建议声卡使用
#define  IRQ6     38    // 软驱传输控制使用
#define  IRQ7     39    // 打印机传输控制使用
#define  IRQ8     40    // 即时时钟
#define  IRQ9     41    // 与 IRQ2 相接，可设定给其他硬件
#define  IRQ10    42    // 建议网卡使用
#define  IRQ11    43    // 建议 AGP 显卡使用
#define  IRQ12    44    // 接 PS/2 鼠标，也可设定给其他硬件
#define  IRQ13    45    // 协处理器使用
#define  IRQ14    46    // IDE0 传输控制使用
#define  IRQ15    47    // IDE1 传输控制使用

//声明相应的IRQ函数
void irq0();        // 电脑系统计时器
void irq1();        // 键盘
void irq2();        // 与 IRQ9 相接，MPU-401 MD 使用
void irq3();        // 串口设备
void irq4();        // 串口设备
void irq5();        // 建议声卡使用
void irq6();        // 软驱传输控制使用
void irq7();        // 打印机传输控制使用
void irq8();        // 即时时钟
void irq9();        // 与 IRQ2 相接，可设定给其他硬件
void irq10();       // 建议网卡使用
void irq11();       // 建议 AGP 显卡使用
void irq12();       // 接 PS/2 鼠标，也可设定给其他硬件
void irq13();       // 协处理器使用
void irq14();       // IDE0 传输控制使用
void irq15();       // IDE1 传输控制使用



#endif

