/*************************************************************************
    > File Name: include/multiboot.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月18日 星期四 14时21分19秒
 ************************************************************************/

#ifndef INCLUDE_MULTIBOOT_H_
#define INCLUDE_MULTIBOOT_H_

#include "types.h"

typedef struct multiboot_t
{
	/* 记录multiboot的版本信息 */ 
	uint32_t flags;

	/* flags[0] 可通过 mem_lower和mem_upper可获取当前可用内存
	 * mem_lower : 存放低端地址
	 * men_upper : 存放高端地址
	 * 单位均为 k
	 */
	uint32_t mem_lower;
	uint32_t mem_upper;

	uint32_t boot_device;	/* flags[1]保存BIOS启动时从哪个OS镜像引导 */ 
	uint32_t cmdline;		/* 内核命令行 */ 
	uint32_t mods_count;	/* boot 模块列表 */
	uint32_t mods_addr;

	/* ELF格式的内核段头表*/
	uint32_t num;			/*  */ 
	uint32_t size;			/* ELF数组的大小 */ 
	uint32_t addr;			/* ELF结构首地址 */ 
	uint32_t shndx;			/*  */

	/* 内存缓冲区的地址和长度 */ 
	uint32_t mmap_length;	/* 缓冲区的长度 */
	uint32_t mmap_addr;		/* 缓冲区的地址 */ 

	uint32_t drives_length;	/* 第一个驱动器的长度 */ 
	uint32_t drives_addr;	/* 第一个驱动器的地址 */ 
	uint32_t config_table;	/* ROM配置表 */
	uint32_t boot_loader_name;
	uint32_t apm_table;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
}__attribute__((packed)) multiboot_t;

/* 
 * 
 */

typedef struct mmap_entry_t
{
	uint32_t size;				/* 相关结构的大小 */
	uint32_t base_addr_low;		/* 启动地址的低32位 */
	uint32_t base_addr_high;	/* 启动地址的高32位 */ 
	uint32_t length_low;		/* 内存区域的低32位 */ 
	uint32_t length_high;		/* 内存区域的高32位 */ 
	uint32_t type;
}__attribute__((packed)) mmap_entry_t;	/* __attribute__ : gcc下使用的特性, 用来告知编译器的 */

extern multiboot_t *glb_mboot_ptr;

#endif
