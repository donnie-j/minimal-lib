#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif

extern "C" void free(void *);

void operator delete (void *p)
{
    free (p);
}

void operator delete [] (void *p)
{
    free (p);
}

void operator delete (void *p, size_t)
{
    free (p);
}
