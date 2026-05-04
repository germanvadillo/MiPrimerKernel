ENTRY(_start)

SECTIONS 
{
    . = 1M; //empieza a colocar bloques a partir de 1MB (0x1000000)

    .text BLOCK(4K) : ALIGN(4K) //primero la seccion multiboot obligatoria para que GRUB la encuentre rapido
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}