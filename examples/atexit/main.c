#include <stdlib.h>

void
foo55()
{
   *(volatile unsigned long*)0xabcd0000 = 0x55;
}

void
fooaa()
{
   *(volatile unsigned long*)0xabcd0000 = 0xaa;
}

int
main (int argc, char *argv[])
{
   *(volatile unsigned long*)0xabcd0000 = 0xff;

   *(volatile unsigned long*)0xabcd0000 = atexit(fooaa);
   *(volatile unsigned long*)0xabcd0000 = atexit(foo55);

   *(volatile unsigned long*)0xabcd0000 = 0xff;

   return 1;
}
