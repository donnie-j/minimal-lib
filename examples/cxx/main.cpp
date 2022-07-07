#include <stdio.h>

class led {
  public:
   led();
   ~led();
   unsigned char set(unsigned char v);
  private:
   unsigned long *p;
};

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

int
main(int argc, char *argv[])
{
   class led l;
   int i;

   for (i=0x5a; i; i=l.set(i)>>1) {}
}
