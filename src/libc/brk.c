/* Simple heap for very low memory embedded systems. */
#include <unistd.h>
#include <linker_symbols.h>

static void *_heap = _end;

int
brk(void *p)
{
   _heap = p;
   return 0;
}
