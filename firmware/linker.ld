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

    .init_array : ALIGN(4)
    {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
        
        . = ALIGN(4);
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
    } >FLASH

    .text : ALIGN(4)
    {
        KEEP(*(SORT_NONE(.init)))
        KEEP(*(SORT_NONE(.fini)))
        *(.text*)
    } >FLASH

    .rodata : ALIGN(4)
    {
        *(.rodata*)
    } >FLASH
    
    .ARM.extab : ALIGN(4)
    {
    	*(.ARM.extab*)
    } >FLASH
    
    .ARM.exidx : ALIGN(4)
    {
    	__exidx_start = .;
    	*(.ARM.exidx*)
    	__exidx_end = .;
    } >FLASH

    .data : ALIGN(4)
    {
        _sdata = .;
        _sidata = LOADADDR(.data);
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT>FLASH

    .bss : ALIGN(4)
    {
        _sbss = .;
        *(.bss* COMMON*)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    . = ALIGN(4);
    end = .;  /* heap start */
    . = ORIGIN(RAM) + LENGTH(RAM) - 4;
    _estack = .;  /* stack start */
    
    ASSERT(_ebss == end, "There is something beween _ebss and end")
}
