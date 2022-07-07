/* Simple heap for very low memory embedded systems. */
#include <unistd.h>
#include <linker_symbols.h>

static void *_heap = _end;

void *
sbrk(int n)
{
   void *p = _heap;
   _heap += n;
   return p;
}

void *
malloc(int n) __attribute__ ((weak, alias ("sbrk")));
