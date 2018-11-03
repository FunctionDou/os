/*************************************************************************
    > File Name: libs/elf.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月18日 星期四 17时03分16秒
 ************************************************************************/

#include "elf.h"
#include "io.h"
#include "string.h"

/* 从multiboot结构体初始化elf结构的信息 */
elf_t elf_from_multiboot(multiboot_t *mb)
{
    int i;
    elf_t elf;
    elf_section_header_t *sh = (elf_section_header_t *)mb->addr;

    uint32_t shstrtab = sh[mb->shndx].addr;
    for(i = 0; i < mb->num; i++)
    {
	/*开启分页后修改 , 访问地址为内核地址, 在3G地址空间以上
	const char * name = (const char *)(sh[i].name + shstrtab);
	*/ 
	const char * name = (const char *)(sh[i].name + shstrtab) + PAGE_OFFSET;


	if(strcmp(name, ".strtab") == 0)
	{
	    /*开启分页后修改 , 访问地址为内核地址, 在3G地址空间以上
	    elf.strtab = (const char *)sh[i].addr;
	    */ 
	    elf.strtab = (const char *)sh[i].addr + PAGE_OFFSET;
	    elf.strtabsz = sh[i].size;
	}
	if(strcmp(name, ".symtab") == 0)
	{
	    /*开启分页后修改 , 访问地址为内核地址, 在3G地址空间以上
	    elf.symtab = (elf_symbol_t *)sh[i].addr;
	    */ 
	    elf.symtab = (elf_symbol_t *)(sh[i].addr + PAGE_OFFSET);
	    elf.symtabsz = sh[i].size;
	}
    }
    return elf;
}

const char * elf_lookup_symbol(uint32_t addr, elf_t *elf)
{
    int i = 0;
    int num;
    num = elf->symtabsz / sizeof(elf_symbol_t);
    for( ; i < num; i++)
    {
	if(ELF32_ST_TYPE(elf->symtab[i].info) != 0x2)
	    continue;
	if((addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size)))
	{
	    return (const char *)((uint32_t)elf->strtab + elf->symtab[i].name);
	}
    }

    return NULL;
}
