/* Linker script to configure memory regions. */
/* customized for Alphatronics Oslo */
MEMORY
{ 
  SRAM1 (rwx)  : ORIGIN = 0x200001AC, LENGTH = 256K - 0x1AC
/*  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K */

    META_B1 (rx)        : ORIGIN = 0x8010000, LENGTH = 2K
    FLASH_B1 (rx)   	: ORIGIN = 0x8010400, LENGTH = 383K
    BOOTCODE_B1 (rx)	: ORIGIN = 0x8070800, LENGTH = 54K
    BOOTSN_B1 (rx) 		: ORIGIN = 0x807E800, LENGTH = 2K
    BOOTCALL_B1 (rx) 	: ORIGIN = 0x807F000, LENGTH = 2K
    BOOTINFO_B1 (r)		: ORIGIN = 0x807F800, LENGTH = 2K

    FLASH_B2 (rx)		: ORIGIN = 0x8090400, LENGTH = 383K
    BOOTCODE_B2 (rx)	: ORIGIN = 0x80F0800, LENGTH = 54K
    BOOTSN_B2 (rx)		: ORIGIN = 0x80FE800, LENGTH = 2K
    BOOTCALL_B2 (rx) 	: ORIGIN = 0x80FF000, LENGTH = 2K
    BOOTINFO_B2 (r)		: ORIGIN = 0x80FF800, LENGTH = 2K
}

/* convert regions defined below to regions defined above */
REGION_ALIAS("FLASH", FLASH_B1)
REGION_ALIAS("BOOTCODE", BOOTCODE_B1)
REGION_ALIAS("BOOTSN", BOOTSN_B1)
REGION_ALIAS("BOOTCALL", BOOTCALL_B1)
REGION_ALIAS("BOOTINFO", BOOTINFO_B1)

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 * 
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   _estack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)
        KEEP(*(.eh_frame*))
    } > FLASH

    .gnu_build_id :
    {
        PROVIDE(g_note_build_id = .);
        *(.note.gnu.build-id)
    } > FLASH

    .header :
    {
        . = ALIGN(8);
        _sheader = .;
        KEEP(*(.header*))
        _eheader = .;
    } > META_B1

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;
    _sidata = .;

    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;
        _edata = .;

    } > SRAM1

    /*!! Aphatronics boot info for bank 1*/
    .bootinfo :
    {
        . = ALIGN(8);
        _sbootinfo = .;
		/* 2 byte version of the header */
        KEEP (*(.bootinfo.ALFBOOT_INFO_VERSION))
		/* info structure with 4 byte serial number and 2 byte PCB ID */	
        KEEP (*(.bootinfo.ALFBOOT_INFO))
        *(.bootinfo*);
        . = ALIGN(8);
        _ebootinfo = .;
    } >BOOTINFO


    /*!! Aphatronics boot call (table)*/
    .bootcall :
    {
        . = ALIGN(8);
        _sbootcall = .;
        KEEP (*(.bootcall*))
        . = ALIGN(8);
        _ebootcall = .;
    } >BOOTCALL

    /*!! Aphatronics boot code*/
    .bootcode :
    {
        . = ALIGN(8);
        _sbootcdoe = .;
//        KEEP (*(.bootclass*))
//        KEEP (*(.bootcode*))
        . = ALIGN(8);
        _ebootcode = .;
    } >BOOTCODE
    
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
    } > SRAM1

    .heap (COPY):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        __HeapLimit = .;
    } > SRAM1

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > SRAM1

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM1) + LENGTH(SRAM1);
    _estack = __StackTop;
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
 /*   ASSERT(_eheader = _sheader, "no region header used")*/

}
