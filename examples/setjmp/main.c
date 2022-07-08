#include <setjmp.h>

jmp_buf env;

void
foo()
{
   *(volatile unsigned long*)0xabcd0000 = 0x55;
   longjmp(env, 0xaa);
   while(1) {;}
}

int
main (int argc, char *argv[])
{
   if ((*(volatile unsigned long*)0xabcd0000 = setjmp(env))) return 0;
   foo();
   *(volatile unsigned long*)0xabcd0000 = 0xff;
   return -1;
}
