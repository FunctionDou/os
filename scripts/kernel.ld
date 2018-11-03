ENTRY(start)    /* ENTRY : 设置入口. 这里入口是从 start 开始进入的*/
SECTIONS
{
    /* 段起始位置 */
    /* 修改 : 因为涉及到实模式到保护模式的3G-4G空间的地址问题, 所以需要设置每个段开启分页机机制后的虚拟地址, 增加了 AT(ADDR( ) - 0xC0000000. 也增加了 init.text和init.data段) */

    PROVIDE(kern_start = 0xC0100000);
    . = 0x100000;
    .init.text :
    {
        *(.init.text)
        . = ALIGN(4096);
    }
    .init.data :
    {
        *(.init.data)
        . = ALIGN(4096);
    }

    . += 0xC0000000;
    .text : AT(ADDR(.text) - 0xC0000000)    /* 存放以编译的机械代码, 代码段 */
    {
        *(.text)    /* *通配符, 将链接的text段的数据写入 .text段中 */
        . = ALIGN(4096);    /* 设置对齐大小为 4k */
    }

    .data : AT(ADDR(.data) - 0xC0000000)     /* 存放已初始化的全局变量和静态变量 */
    {
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - 0xC0000000)     /* 存放未初始化的全局和静态变量, 以及初始化未0的全局和静态变量 */
    {
        *(.bss)
        . = ALIGN(4096);
    }

    .stab : AT(ADDR(.stab) - 0xC0000000)
    {
        *(.stab)
        . = ALIGN(4096);
    }
    
    .stabstr : AT(ADDR(.stabstr) - 0xC0000000)
    {
        *(.stabstr)
        . = ALIGN(4096);
    }

    PROVIDE(kern_end = .);

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
