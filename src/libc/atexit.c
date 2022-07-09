#include <stdlib.h>

#define ATEXIT_ENTS 2

static void (*_atexit_funcs[ATEXIT_ENTS])(void);

int
atexit(void (*function)(void))
{
   int i;

   for (i=0; i<ATEXIT_ENTS; i++) {
      if (!_atexit_funcs[i]) {
         _atexit_funcs[i] = function;
         return 0;
      }
   }
   return -1;
}

__attribute__ ((destructor)) void
_atexit_run()
{
   int i;

   for (i=ATEXIT_ENTS; i;) if (_atexit_funcs[--i]) _atexit_funcs[i]();
}
