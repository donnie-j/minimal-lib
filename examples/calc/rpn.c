#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "rpn.h"
#include "lcd-drv.h"

struct _val zero_fp = { 1, 0, 0, 0, 0, 0 };
static double stack[STACK_LEN];
static int sp = STACK_LEN;

fp_comp_t
decomp(double v, int eng)
{
   fp_comp_t d;
   unsigned int t;
   unsigned int m=100000;

   if (!v) return zero_fp;

   d.s = v>=0.0 ? 1 : -1;
     v = v>=0.0 ? v : -v;

   /* extract 6 significant digits */
   d.e = d.p = 5;
   while (v < m   ) { v *= 10; d.e--; }
   while (v>= 10*m) { v /= 10; d.e++; }

   /* round */
   v += 0.5;
   while (v>= 10*m) { v /= 10; d.e++; }

   /* engineering mode exp adjust n, u, m, k, M... */
   if (eng) while (d.e%3) { m /= 10; d.e--; d.p--; }

   t   = v;
   d.i  = t/m;
   d.f = t - (d.i)*m;
   d.flags = 0;

   return d;
}

double
comp(fp_comp_t v)
{
   double ret;

   ret = v.f;
   while (v.p--) { ret /= 10; }
   ret += v.i;
   while (v.e > 0) { ret *= 10; v.e--; }
   while (v.e < 0) { ret /= 10; v.e++; }
   if (v.s < 0) return -ret;
   return ret;
}

static char *
itoa(char *p, unsigned int n, int d)
{
   int i=1;
   unsigned int v;

   if (!d) return p;
   while (--d)       i*=10;
   while (i*10 <= n) i*=10;
   while (i) {
      v = n/i;
      *(p++) = '0'+v;
      n -= v*i;
      i /= 10;
   }
   return p;
}

char *
print_decomp(char *buf, fp_comp_t d, int f, int e)
{
   char *p = buf;
   *(p++) = d.s>0 ? ' ':'-';
   p      = itoa(p, d.i, 1);
   if (f || e) {
      *(p++) = '.';
      p      = itoa(p, d.f, d.p);
      if (e) {
         *(p++) = 'e';
         *(p++) = (d.e<0 || d.flags & 4) ? '-':'+';
         p      = itoa(p, abs(d.e), 1);
      }
   }
   while(p<buf+16) *(p++) = ' ';
   *p = 0;
   return buf;
}

fp_comp_t
key_edit(fp_comp_t d, char in)
{
   d.flags &=7;
   if (d.e<0) { d.e = -d.e; d.flags ^= 4; }

   switch (d.flags&3) {
   case 0:
      d = zero_fp;
      d.flags++;
      if (in == 'e') d.i=1;
   case 1:
      if (isdigit(in)) { d.i = d.i*10 + in-'0'; }
      if (in == 'n') d.s = -d.s;
      if (in == '.') d.flags++;
      if (in == 'e') d.flags = 3;
      if (in == 'b') d.i /= 10;
      break;
   case 2:
      if (isdigit(in)) { d.f = d.f*10 + in-'0'; d.p++; }
      if (in == 'n') d.s = -d.s;
      if (in == 'e') d.flags++;
      if (in == 'b') {
         d.f /= 10;
         if (!d.p) d.flags--;
         else d.p--;
      }
      break;
   case 3:
      if (isdigit(in)) { d.e = d.e*10 + in-'0'; }
      if (in == 'n') d.flags ^= 4;
      if (in == 'b') {
         if (!d.e) d.flags = 2;
         d.e /= 10;
      }
      break;
   }
   if (d.e && d.flags & 4) { d.e = -d.e; d.flags &= ~4; }

   return d;
}

void
push(double v)
{
   if (sp) stack[--sp] = v;
}

double
pop()
{
   if (sp<STACK_LEN) return stack[sp++];
   return 0.0;
}

fp_comp_t
key_process(fp_comp_t d, char in)
{
   char buf[32];
   double x = 0.0;
   double y = 0.0;
   int i;

   x = pop();
   if (strchr(   "+-*/wvqQsScCtTDdz" , in)) {
      if (strchr("+-*/wd"            , in)) y = pop();
      switch (in) {
      case '+': x = y + x; break;
      case '-': x = y - x; break;
      case '*': x = y * x; break;
      case 't': y = sin(x); x = cos(x);
      case '/': x = y     / (x ? x : 1e-300); break; // avoid div by zero
      case 'w': push(x); x = y; break;
      case 'v': x = (1.0) / (x ? x : 1e-300); break; // avoid div by zero
      case 'q': x = sqrt(x); break;
      case 'Q': x = x*x; break;
      case 's': x =  sin(x); break;
      case 'S': x = asin(x); break;
      case 'c': x =  cos(x); break;
      case 'C': x = acos(x); break;
      // case 't': x =  tan(x); break;
      case 'T': x = atan(x); break;
      case 'D': push(x); x = M_PI; break;
      case 'z': push(x); break;
      case 'd': x = y; break;
      }
      d=decomp(x,1);
      d.flags = 0x10;
      push(x);
   } else {
       if (d.flags & 0x10) push(x);
       d = key_edit(d, in);
       push(comp(d));
   }

   for (i=3; i; i--) {
      if (sp+i < STACK_LEN) lcd_puts(0, 3-i, print_decomp(buf, decomp(stack[sp+i], 1), 1, 1));
      else                  lcd_clr(3-i);
   }

   if (!(d.flags & 7)) lcd_puts(0, 3, print_decomp(buf, decomp(stack[sp], 1), 1, 1));
   else                lcd_puts(0, 3, print_decomp(buf, d, d.flags == 2, (d.flags&3) == 3 ));

   return d;
}
