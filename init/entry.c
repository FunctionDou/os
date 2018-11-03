/*************************************************************************
    > File Name: entry.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月15日 星期一 13时37分55秒
 ************************************************************************/

#include "console.h"
#include "debug.h"
#include "idt.h"
#include "gdt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "kalloc.h"
#include "string.h"
#include "sched.h"

int flag = 0;

// 开启分页后修改整个入口函数
static void kern_init();

multiboot_t *glb_mboot_ptr;      // 指向multiboot结构体指针, 在mutliboot.h中定义

// 内核栈
char kern_stack[STACK_SIZE];
uint32_t kern_stack_top;

// section属性是让某些变量, 函数存放在我们自己想要存放的段中, 而不是编译器默认设置的.data和.bss段
// 这里我们让页目录项和kern_entry都指定存放在 .init.data 段中
// 这是我们定义的临时页目录项和页表, 地址必须对齐, 而且在内存在0-640kb都是空闲的
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

__attribute__((section(".init.text"))) void kern_entry()
{
   uint32_t cr0;
   pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;  // 开启页目录项, 并设置其PAGE_PRESENT和页面可读写权限
   pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;    // 0xC0000000对应的页目录项在1M地址空间的物理映射, 并设置其PAGE_PRESENT和页面可读写权限

   int i;
   // 从物理地址0到4M的地址空间设置PAGE_PRESENT和读写权限位
   // 内核虚拟地址的0-4M和物理地址的0-4M进行映射
   for(i = 0; i < 1024; i++)
   {
      pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
   }
   // 这里映射内核地址从0xC0000000的4M空间与物理地址的0-4M的空间进行映射
   for(i = 0; i < 1024; i++)
   {
      pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
   }

   asm volatile ("mov %0, %%cr3" :: "r"(pgd_tmp));  // 设置临时页目录项, cr3寄存器保存的是页目录项地址, 将pgd_tmp存放的地址存放到cr3中
   
   // cr0 : 与开启分页相关
   // 第一位 : 0 表示在实模式下, 1 表示在保护模式下
   // 最后一位 : 0 表示为开启分页, 1 表示开启分页
   asm volatile ("mov %%cr0, %0" : "=r"(cr0));
   cr0 |= 0x80000000;       // 设置最高位为1, 启动分页
   asm volatile ("mov %0, %%cr0" :: "r"(cr0));

   // 设置内核栈
   kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
   asm volatile ("mov %0, %%esp\n\t"
         "xor %%ebp, %%esp" :: "r"(kern_stack_top));

   // 更新全局multiboot指针, mboot_ptr_tmp和glb_mboot_ptr来自multiboot.h中, mboot_ptr_tmp未建立分页机制时的临时指针
   glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

   // 初始化内核函数
   kern_init();
}

int thread(void *arg)
{
   while(1)
   {
      if(flag == 1)
      {
         printk("B");
         flag = 2;
         int  n = 100000;
         while(n--);
      }
   }

   return 0;
}

int thread1(void *arg)
{
   while(1)
   {
      if(flag == 2)
      {
         printk_color(rc_black, rc_green, "C");
         flag = 3;
         int n = 100000;
         while(n--)
            ;
      }
   }
}

int thread2(void *arg)
{
   while(1)
   {
      if(flag == 3)
      {
         printk_color(rc_black, rc_red, "D");
         flag = 0;
         int n = 100000;
         while(n--);
      }
   }
}

void kern_init()
{
   init_debug();
   init_gdt();
   init_idt();

   console_clear();
   printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
    
   init_timer(200);

   printk("kernel memory start : %#x\n", kern_start);
   printk("kernel memory end : %#x\n", kern_end);
   printk("kernel memory used : %dKB\n", (kern_end - kern_start + 1023) / 1024);

   show_memory_map();
   init_mmp();
   init_vmm();
   // printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);
   uint32_t alloc = 0;
   printk_color(rc_black, rc_blue, "stack\n");
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);

   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);
   alloc = alloc_page();
   printk_color(rc_black, rc_blue, "stack %#x\n", alloc);

   int *a = (int *)kmalloc(sizeof(int) * 3);
   a[0] = 1;
   a[1] = 2;
   kfree(a);
   int *b = (int *)kmalloc(sizeof(int) * 3);
   b[0] = 1;
   b[1] = 2;
   kfree(b);


   init_alloc();
   // test_alloc();
   init_sched();
   char s[100];
   sprintk(s, "sprintf clear %d\n", 1);
   printk("%s", s);

   kernel_thread(thread, NULL);
   kernel_thread(thread1, NULL);
   kernel_thread(thread2, NULL);
   
   asm volatile ("sti");
   while(1)
   {
      if(flag == 0)
      {
         printk_color(rc_black, rc_red, "A");
         flag = 1;
         int n = 100000;
         while(n--);
     }
   }


/* 
    asm volatile ("sti");
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
*/ 
}
