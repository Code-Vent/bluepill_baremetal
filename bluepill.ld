
MEMORY{
  FLASH (RX) : ORIGIN = 0x08000000, LENGTH = 64K
  SRAM (RWX) : ORIGIN = 0x20000000, LENGTH = 20k
}

__max_heap_size = 0x400;
__max_stack_size = 0x200;

SECTIONS{
  .text :
  {
    KEEP(*(.isr_vector))
    *(.text)
    *(.text.*)
    *(.init)
    *(.init.*)
    *(.fini)
    *(.fini.*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _etext = .;
  }>FLASH

  .preinit_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(8);
  }>FLASH

  .init_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(8);
  }>FLASH

  .fini_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(8);
  }>FLASH

  _la_data = LOADADDR(.data);
  .data :
  {
    _sdata = .;
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _edata = .;
  }>SRAM AT> FLASH

  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  }
}