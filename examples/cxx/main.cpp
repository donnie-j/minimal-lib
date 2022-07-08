#include <stdio.h>
#include "led.h"

int
main(int argc, char *argv[])
{
   class led l;
   int i;

   for (i=0x5a; i; i=l.set(i)>>1) {}
}
