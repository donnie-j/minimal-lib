/**************************************
 SuperH (SH-2) C Compiler Linker Script
 **************************************/ 

OUTPUT_FORMAT("elf32-sh")
OUTPUT_ARCH(sh)

MEMORY
{
	rom    : o = 0x00000000, l = 0x2000
	ram    : o = 0x10000000, l = 0x20000
}

SECTIONS 				
{
.text :	{
	__svect = .;
	*(.vect)
	__evect = .;
	__stext = .;
	*(.text.crt)
	*(.text.startup)
	*(.text.exit)
	*(.text)
	*(.strings)
	. = ALIGN(4);
   	 __etext = . ; 
	}  > rom

.tors : {
	__sctors = . ;
	*(.ctors)
	__ectors = . ;
	__sdtors = . ;
	*(.dtors)
	__edtors = . ;
	. = ALIGN(4);
	}  > rom

.rodata : {
	__srodata = .;
	*(.rodata*)
	. = ALIGN(4);
	__erodata = .;
	*(.eh*)
	} >rom

__sidata = ADDR(.text) + SIZEOF(.text) + SIZEOF(.tors) + SIZEOF(.rodata);
.data : AT(__sidata) {
        __sdata = . ;
	*(.data)
	. = ALIGN(4);
	__edata = . ;
	}  > ram
__eidata = __sidata + SIZEOF(.data);

.bss (NOLOAD) : {
	__sbss = .;
	*(.bss)
	*(COMMON)
	. = ALIGN(4);
	__ebss = .;
	__sstack = .;
	__end = .;
	}  >ram

__estack = ORIGIN(ram) + LENGTH(ram);
}
