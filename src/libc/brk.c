/* Simple heap for very low memory embedded systems. */
#include <unistd.h>
#include <linker_symbols.h>

extern void *_heap;

int
brk(void *p)
{
   _heap = p;
   return 0;
}
