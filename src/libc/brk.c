#include <unistd.h>

extern int __end;
static void *endp = &__end;

int
brk (void *addr)
{
	endp = addr;
	return 1;
}

void *
sbrk (int incr){
	endp += incr;
	return endp;
}
