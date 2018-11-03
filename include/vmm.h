/*************************************************************************
    > File Name: include/vmm.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月28日 星期日 23时29分27秒
 ************************************************************************/

#ifndef INCLUDE_VMM_H_
#define INCLUDE_VMM_H_

#include "types.h"
#include "idt.h"


// 内核的偏移地址
#define PAGE_OFFSET 0xC0000000 

/* 表示页表或页是否存在内存中
 * 1 : 表示存在内存中
 * 0 : 表示并未创建在内存中, 使用就会报错
 */
#define PAGE_PRESENT 0x1 


/* R/W 位 1 是读 / 写 标志。
 * 1 : 表示页面可以被读、写或执行。
 * 0 : 表示页面只读或可执行。
 * 当处理器运行在超级用户特权级(级别 0,1,2)时，则 R/W 位不起作用。
 * 页目录项中的 R/W 位对其所映射的所有页面起作用。
 */
#define PAGE_WRITE 0x02 


/* U/S  位 2 是用户 / 超级用户 (User/Supervisor) 标志。
 * 1 : 那么运行在任何特权级上的程序都可以访问该页面。
 * 0 : 那么页面只能被运行在超级用户特权级 (0,1 或 2) 上的程序访问。
 * 页目录项中的 U/S 位对其所映射的所有页面起作用。
 */
#define PAGE_USER 0x4

// 虚拟地址中分页的大小
#define PAGE_SIZE 0x1000

// 页掩码并且是4k对齐
#define PAGE_MASK 0xFFFFF000

#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)		// 获取页目录项的地址, 因为页目录在32位的最高10位中
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)		// 获取一个地址的页表项
#define OFFET_INDEX(x) ((x) & 0xFFF)			// 获取一个地址的页内偏移
typedef uint32_t pgd_t;							// 页目录数据类型
typedef uint32_t pte_t;							// 页表数据类型
// #define PGD_SIZE (PAGE_SIZE / sizeof(pte_t))	// 页目录数量
#define PGD_SIZE (PAGE_SIZE / sizeof(pte_t))	// 页目录数量
#define PTE_SIZE (PAGE_SIZE / sizeof(uint32_t))	// 页框成员的数量

#define PTE_COUNT 128		// 128	: 映射512M内存空间的页表数量

// 内核页目录区域
extern pgd_t pgd_kern[PGD_SIZE];

// 初始化虚拟内存管理
void init_vmm();

// 更换页目录项
void switch_pgd(uint32_t pd);

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);		// flags : 设置的权限, 函数将 pa 的物理地址映射到 va 的虚拟地址上
void unmap(pgd_t *pgd_now, uint32_t va);	// 取消虚拟地址到物理的映射

// 虚拟地址va如果映射到物理地址则返回 1. 并且pa不是空指针就把物理地址写入pa参数中, 如果pa为空指针就返回0
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);

// 页错误中断处理函数
void page_fault(pt_regs_t *regs);

#endif
