/* Export the linker symbols for a bare metal J-Core system */

#ifndef __LINKER_SYMBOLS__
#define __LINKER_SYMBOLS__

extern unsigned long _svect[];
extern unsigned long _evect[];
extern unsigned long _stext[];
extern unsigned long _sctors[];
extern unsigned long _ectors[];
extern unsigned long _sdtors[];
extern unsigned long _edtors[];
extern unsigned long _erodata[];
extern unsigned long _srodata[];
extern unsigned long _etext[];
extern unsigned long _sidata[];
extern unsigned long _eidata[];
extern unsigned long _sdata[];
extern unsigned long _ebss[];
extern unsigned long _end[];
extern unsigned long _sbss[];
extern unsigned long _sstack[];
extern unsigned long _edata[];
extern unsigned long _estack[];

#endif /* __LINKER_SYMBOLS__ */
