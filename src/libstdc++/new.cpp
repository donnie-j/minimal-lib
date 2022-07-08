#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif

extern "C" void * malloc(unsigned int);

void * operator new (size_t size)
{
    return malloc (size);
}

void * operator new [] (size_t size)
{
    return malloc (size);
}
