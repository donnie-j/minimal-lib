/* Simple malloc() for very low memory embedded systems. */
#include <stdlib.h>
#include <unistd.h>

#define SLOP 16

#define to_flag(X)  (                  (unsigned long) (X) &  1 )
#define set_flag(X) (                 ((unsigned long) (X) |  1))
#define clr_flag(X) (                 ((unsigned long) (X) & ~1))
#define to_ptr(X)   ((unsigned long *) clr_flag(X))
#define to_size(X)  ((unsigned long)to_ptr(*(X))-(unsigned long)(X))

static unsigned long *_mptr;

void *
malloc(size_t s)
{
   void *p;
   unsigned long *q, *r;

   if (!s) return (void *)0;
   s = (s + 2 * sizeof(unsigned long) - 1) & (~(sizeof(unsigned long) - 1));

   /* first run */
   if (!_mptr) {
      p = q = sbrk(s + sizeof(unsigned long));
      r = (void *)q + s;
      *q = set_flag(r);
      *r = clr_flag(p);
      _mptr = p;
      return q+1;
   }

   /* look for a free block */
   for (q=_mptr; to_ptr(*q)!=_mptr; q=to_ptr(*q)) {
      if (!to_flag(*q) && to_size(q) >= s && to_size(q) < s+SLOP) {
         *q = set_flag(*q);
         return q+1;
      }
   }

   /* add a new block on the end */
   p = sbrk(s);
   r = (void *)q + s;
   *q = set_flag(r);
   *r = clr_flag(_mptr);
   return q+1;
}

void
free(void *m)
{
   unsigned long *p = m;

   if (!p) return;

   p--;
   *p = clr_flag(*p);
}
