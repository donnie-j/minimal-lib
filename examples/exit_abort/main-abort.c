#include <stdlib.h>

__attribute__ ((destructor))
void
foo()
{
   *(volatile unsigned long*)0xabcd0000 = 0x55;
}

int
main (int argc, char *argv[])
{
   *(volatile unsigned long*)0xabcd0000 = 0xff;
   abort(); /* does not run destructors */
   while(1) {;}
}
