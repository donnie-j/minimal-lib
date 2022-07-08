#ifndef __LED_H__
#define __LED_H__

class led {
  public:
   led();
   ~led();
   unsigned char set(unsigned char v);
  private:
   unsigned long *p;
};

#endif /*  __LED_H__ */
