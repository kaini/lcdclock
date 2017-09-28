MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 192K
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .isr_vector : ALIGN(4)
    {
        KEEP(*(.isr_vector))
    } >FLASH

    .text : ALIGN(4)
    {
        KEEP(*(SORT_NONE(.init)))
        KEEP(*(SORT_NONE(.fini)))
        *(.text .text.*)
    } >FLASH

    .rodata : ALIGN(4)
    {
        *(.rodata .rodata.*)
    } >FLASH

    .data : ALIGN(4)
    {
        _sdata = .;
        _sidata = LOADADDR(.data);
        *(.data .data.*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT>FLASH

    .bss : ALIGN(4)
    {
        _sbss = .;
        *(.bss .bss.*)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    . = ALIGN(4);
    end = .;  /* heap start */
    . = ORIGIN(RAM) + LENGTH(RAM) - 4;
    _estack = .;  /* stack start */
}
