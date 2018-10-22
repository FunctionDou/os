/*************************************************************************
    > File Name: include/lef.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月18日 星期四 16时39分41秒
 ************************************************************************/

#ifndef INCLUDE_ELF_H_
#define INCLUDE_ELF_H_

/* ELF : 可重定位目标格式 */ 
#include "types.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i) & 0xF)

/* name : 目标的大小 */ 
/* type : 字符串表中的字节偏移 */ 
/* size : 要么是数据, 要么是函数 */ 

/* ELF结构的段头 */
typedef struct elf_section_header_t
{
	uint32_t name;		/* 字符串表中的字节偏移 */ 
	uint32_t type;		/* 要么是数据, 要么是函数 */ 
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;		/* 目标的大小 */ 
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
}__attribute__((packed)) elf_section_header_t;

/* ELF的格式信息 
 * 符号表
 * */ 
typedef struct elf_symbol_t
{
	uint32_t name;	
	uint32_t value;
	uint32_t size;		
	uint32_t info;
	uint32_t other;
	uint32_t shndx;
}__attribute__((packed)) elf_symbol_t;

/* ELF的信息 */ 
typedef struct elf_t
{
	/* 指向elf_symbol_t符号表的指针, symtabsz : 其结构体的大小  */
	elf_symbol_t *symtab;
	uint32_t symtabsz;

	/* 指向字符串表的指针, strtabsz : 字符串表的大小 */ 
	const char *strtab;
	uint32_t strtabsz;
}elf_t;

elf_t elf_from_multiboot(multiboot_t *mb);	/* 从multiboot结构获取elf参数的地址等, 初始化该结构体 */ 

const char *elf_lookup_symbol(uint32_t addr, elf_t *elf);

#endif

