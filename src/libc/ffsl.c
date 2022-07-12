#include <string.h>

int
ffsl(long int n)
{
   int i = sizeof(n)*8;
   unsigned long m = 1L << (i-1);

   while (i && !(n&m)) { i--; m >>= 1; }
   return i;
}
