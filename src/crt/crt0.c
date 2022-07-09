/* A minimal, generic bare metal C runtime */

#include "linker_symbols.h"

extern int main(int argc, char *argv[]);

__attribute__ ((section (".text.crt"), naked))
void _start()
{
   unsigned long *rom;
   unsigned long *ram;

   while (1) {
EXPORT_LABEL(abort);
      for (ram=_sdata, rom=_sidata ; ram<_edata ; ram++, rom++) *ram = *rom;
      for (                        ; ram<_ebss  ; ram++       ) *ram = 0;

      for (rom=_sctors             ; rom<_ectors;        rom++) ((void (*)())*rom)();
      main(0, (void *)0);
EXPORT_LABEL(exit);
      for (                        ; rom<_edtors;        rom++) ((void (*)())*rom)();
   }
}

void * vec[] __attribute__ ((section (".vect"))) = { _start, _estack, _start, _estack };
