#include "led.h"

led::led()
{
   p = (unsigned long *)0xABCD0000;
   set(-1);
}

led::~led()
{
   set(5);
}

unsigned char
led::set(unsigned char v)
{
   return *p = v;
}
