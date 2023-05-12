/* A minimal, generic C runtime for code loaded to ram fram a bootloader */

#include "linker_symbols.h"

extern int main(int argc, char *argv[]);

__attribute__ ((section (".text.crt"), naked))
void _start()
{
   unsigned long *ram;

   while (1) {
EXPORT_LABEL(abort);
      for (ram=_sbss               ; ram<_ebss  ; ram++       ) *ram = 0;

      for (ram=_sctors             ; ram<_ectors;        ram++) ((void (*)())*ram)();
      main(0, (void *)0);
EXPORT_LABEL(exit);
      for (                        ; ram<_edtors;        ram++) ((void (*)())*ram)();
   }
}
