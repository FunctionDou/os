/*************************************************************************
    > File Name: mm/pmm.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月24日 星期三 21时37分14秒
 ************************************************************************/

#include "io.h"
#include "debug.h"
#include "multiboot.h"
#include "pmm.h"

// 分配栈页的数组, 数组的每个值保存了每个页的物理地址
static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
static uint32_t pmm_stack_top = 0;

uint32_t phy_page_count = 0;

// 输出每个段的地址映射
void show_memory_map()
{
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;

    printk("memory map\n");

    mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;
    // 输出每个段的信息
    for( ; (uint32_t)mmap < mmap_addr + mmap_len; mmap++)
    {
	printk("mmap size = %d\taddr %#x%x\tlengt %#x%x\tmap type = %d\n\n", (uint32_t)mmap->size, (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low, (uint32_t)mmap->length_high, (uint32_t)mmap->length_low, (uint32_t)mmap->type);
    }
}

// 初始化页到物理地址之间的映射
void init_mmp()
{
    mmap_entry_t * mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
    mmap_entry_t * mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;

    mmap_entry_t *mmap = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
    for( ; mmap < mmap_end_addr; mmap++)
    {
	// type == 1 : 表示可使用内存, 2 : 表示以使用内存, 3 : 保留区域
	if(mmap->type == 1 && mmap->base_addr_low == 0x100000)
	{
	    uint32_t page_addr = mmap->base_addr_low + (uint32_t)(kern_end - kern_start);
	    uint32_t length = mmap->base_addr_low + mmap->length_low;

	    // 页到物理地址的映射
	    while(page_addr < length && page_addr <= PMM_MAX_SIZE)
	    {
		free_page(page_addr);
		page_addr += PAGE_SIZE;
		phy_page_count++;
	    }
	}
    }

    printk("page count %d\n", phy_page_count);
}

// 申请栈页
uint32_t alloc_page()
{
    assert(pmm_stack_top != 0, "out of page\n");
    uint32_t page = pmm_stack[pmm_stack_top--];

    return page;
}

// 释放栈页
void free_page(uint32_t page)
{
    assert(pmm_stack_top != PAGE_MAX_SIZE, "stack\n");
    pmm_stack[++pmm_stack_top]= page;
}
