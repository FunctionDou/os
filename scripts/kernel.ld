ENTRY(start)    /* ENTRY : 设置入口. 这里入口是从 start 开始进入的*/
SECTIONS
{
    /* 段起始位置 */

    . = 0x100000;
    .text :     /* 存放以编译的机械代码, 代码段 */
    {
        *(.text)    /* *通配符, 将链接的text段的数据写入 .text段中 */
        . = ALIGN(4096);    /* 设置对齐大小为 4k */
    }

    .data :     /* 存放已初始化的全局变量和静态变量 */
    {
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :      /* 存放未初始化的全局和静态变量, 以及初始化未0的全局和静态变量 */
    {
        *(.bss)
        . = ALIGN(4096);
    }

    .stab :
    {
        *(.stab)
        . = ALIGN(4096);
    }
    
    .stabstr :
    {
        *(.stabstr)
        . = ALIGN(4096);
    }

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
