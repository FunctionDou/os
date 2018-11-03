/*************************************************************************
    > File Name: mm/vmm.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月29日 星期一 09时41分12秒
 ************************************************************************/

#include "idt.h"
#include "debug.h"
#include "string.h"
#include "vmm.h"

// 页目录项存储位置, 并且设置以4k进行对齐
pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

// 内核页框页表
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

void init_vmm()
{
    uint32_t kern_ptr_first_idx = PGD_INDEX(PAGE_OFFSET);

    uint32_t i, j;
    for(i =  kern_ptr_first_idx, j = 0; i < PTE_COUNT + kern_ptr_first_idx; i++, j++)
    {
	//减去0xC0000000的主要是 MMU翻译地址时,需要得到页框的物理地址
	pgd_kern[i] = ((uint32_t)pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
    }

    uint32_t *pte = (uint32_t) *pte_kern;
    for(i = 1; i < PTE_COUNT * PTE_SIZE; i++)
    {
	pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    // 获取pgd_kern的真实物理地址
    uint32_t pgd_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;
    // 注册页错误中断的处理函数(14 是页故障的中断号)
    register_interrupt_handler(14, &page_fault);
    switch_pgd(pgd_kern_phy_addr);
}

// 将地址传给cr3页目录寄存器
void switch_pgd(uint32_t pd)
{
    asm volatile("mov %0, %%cr3" :: "r"(pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags)
{
    // 将虚拟地址的11-21的页表地址和21-31位的页目录项地址分离出来保存
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    // pte指向虚拟地址在的页目录项指向的页框地址(以4K对齐)
    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

    // 如果页框没有分配地址. 那么执行页分配
    if(!pte)
    {
	// alloc_page() : 分配一个页地址
	pte = (pte_t *)alloc_page();
	// 设置该页框在内存中存在, 并设置读写
	pgd_now[pgd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;

	// 这里为了映射到虚拟地址上
	// pte是页框地址, 将pte转为了内核线性地址也就是物理地址直接映射到虚拟地址上
	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
	bzero(pte, PAGE_SIZE);
    }
    else 
    {
	// 物理地址直接映射到虚拟地址上
	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
    }

    // 设置页表的访问权限(这里是页表, 因为页框加上偏移就是一个页表的地址)
    pte[pte_idx] = (pa & PAGE_MASK) | flags;

    // 更新页表缓存
    asm volatile ("invlpg (%0)" : : "a"(va));
}

// 取消虚拟地址到物理地址的映射
void unmap(pgd_t *pgd_now, uint32_t va)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    // pte指向物理地址在的页目录项指向的页框地址(以4K对齐)
    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
    
    // 本身是空页框就直接返回
    if(!pte)
    {
	return;
    }

    // 设置页表的访问权限(这里是页表, 因为页框加上偏移就是页表)
    // 不为空就直接指向页表的地址并将该页表的地址重新设置为0, 代表没有分配页表
    // pte转为了内核线性地址
    pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
    pte[pte_idx] = 0;

    // 更新页表
    asm volatile ("invlpg (%0)" :: "a"(va));
}

// 获得虚拟地址所在页框的物理地址, 把无力地孩子存放在pa中
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
    if(!pte)
    {
	return 0;
    }

    // pte转为了内核线性地址
    pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
    if(pte[pte_idx] != 0 && pa)
    {
	*pa = pte[pte_idx] & PAGE_MASK;
	return 1;
    }

    return 0;
}

// 中断14的输出
void page_fault(pt_regs_t *regs)
{
    uint32_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r"(cr2));

    printk("PAGE fault at %#x, address %#x\n", regs->eip, cr2);
    printk("Error code %#x\n", regs->err_code);

    if(!(regs->err_code & 0x1))
    {
	printk_color(rc_black, rc_red, "Becase the page wasn't present\n");
    }

    if(regs->err_code & 0x2)
    {
	printk_color(rc_black, rc_red, "Write error\n");
    }
    else 
    {
	printk_color(rc_black, rc_red, "Read error\n");
    }

    if(regs->err_code & 0x4)
    {
	printk_color(rc_black, rc_red, "In user mode\n");
    }
    else 
    {
	printk_color(rc_black, rc_red, "In kernel mode\n");
    }

    if(regs->err_code & 0x8)
    {
	printk_color(rc_black, rc_red, "Reserved bits being overwrite\n");
    }

    if(regs->err_code & 0x10)
    {
	printk_color(rc_black, rc_red, "The fault occurred an instruction fetch\n");
    }

    while(1)
	;
}
