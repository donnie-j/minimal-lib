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

   d.s = v>=0 ? 1 : -1;
     v = v> 0 ? v : -v;

   /* extract 6 significant digits */
   d.e = d.p = 5;
   while (v < m   && v) { v *= 10; d.e--; }
   while (v>= 10*m    ) { v /= 10; d.e++; }

   /* engineering mode exp adjust n, u, m, k, M... */
   if (eng) while (d.e%3) { m /= 10; d.e--; d.p--; }

   t   = v+0.5;
   d.i  = t/m;
   d.f = t - (d.i)*m;

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

void
print_decomp(fp_comp_t d, int f, int e)
{
   char buf[64];

   if (e)      sprintf(buf, "%c%d.%.*de%+d\n", d.s>0 ? ' ':'-', d.i, d.p, d.f, d.e);
   else if (f) sprintf(buf, "%c%d.%.*d\n",     d.s>0 ? ' ':'-', d.i, d.p, d.f     );
   else        sprintf(buf, "%c%d\n",          d.s>0 ? ' ':'-', d.i               );
   lcd_loc(0,2); lcd_puts(buf); lcd_puts("           ");
}

fp_comp_t
key_edit(fp_comp_t d, char in)
{
   switch (d.flags) {
   case 0:
   case 10:
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
   case 7:
      if (isdigit(in)) { d.e = d.e*10 + in-'0'; }
      if (in == 'n') {
         if (!d.e) d.flags ^= 4;
         d.e = -d.e;
      }
      if (d.e && d.flags & 4) { d.e = -d.e; d.flags &= 4; }
      if (in == 'b') {
         if (!d.e) d.flags = 2;
         d.e /= 10;
      }
      break;
   }
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
   double x = 0.0;
   double y = 0.0;

   if (strchr("+-*/wvqQsScCtT d", in)) {
      if (d.flags && d.flags != 10) push(comp(d));
      x = pop();
   }
   if (strchr("+-*/w"           , in)) y = pop();
   if (strchr("+-*/wvqQsScCtTD" , in)) {
      d.flags = 0;
      switch (in) {
      case '+': x = y + x; break;
      case '-': x = y - x; break;
      case '*': x = y * x; break;
      case '/': x = y     / (x ? x : 1e-300); break; // avoid div by zero
      case 'w': push(x); x = y; break;
      case 'v': x = (1.0) / (x ? x : 1e-300); break; // avoid div by zero
#if 0
      case 'q': x = sqrt(x); break;
      case 'Q': x = x*x; break;
      case 's': x =  sin(x); break;
      case 'S': x = asin(x); break;
      case 'c': x =  cos(x); break;
      case 'C': x = acos(x); break;
      case 't': x =  tan(x); break;
      case 'T': x = atan(x); break;
#endif
      case 'D': x = M_PI; break;
      }
      push(x); 
   } else if (in=='z') {
       if (!d.flags) d=decomp(stack[sp], 1);
       push(comp(d));
       d.flags = 10;
   } else {
       d = key_edit(d, in);
   }

   if (!d.flags ) print_decomp(decomp(stack[sp], 1), 1, 1);
   if ( d.flags ) print_decomp(d, d.flags == 2, d.flags == 3 || d.flags == 7);

   return d;
}
#if 0
int
main (int argc, char *argv[])
{
   fp_comp_t d = zero_fp;
   char *p;
   int i;
   int a=1;

   if (argc < 2) exit(0);

   p=argv[a++];
   while (*p) {
      d = key_process(d, *p);
      if (!*(++p) && a<argc) p = argv[a++];
   }
   for (i=STACK_LEN; i>sp; ) print_decomp(decomp(stack[--i], 1), 1, 1);

   return 0;
}
#endif
