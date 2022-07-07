#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lcd-drv.h"

static char * keys[] = {
/*        0       1       2       3        4     5     6       7 */
/* 00 */ "",     "SUM+", "STO",  "",      "UP", "DN", "SHFT", "",
/* 08 */ "EXIT", "1/x",  "RCL",  "ENTER", "",   "",   "",     "",
/* 10 */ "",     "SRQT", "R DN", "x y",   "7",  "4",  "1",    "0",
/* 18 */ "",     "LOG",  "SIN",  "+/-",   "8",  "5",  "2",    ".",
/* 20 */ "",     "LN",   "COS",  "E",     "9",  "6",  "3",    "R/S",
/* 28 */ "",     "XEQ",  "TAN",  "BS",    "/",  "*",  "-",    "+" };

extern char version_string[];

int
main (int argc, char *argv[])
{
  volatile int i;
  int nk = 0;
  unsigned long *p;

  lcd_loc(0, 0);
  lcd_puts ("LCD init");
  for (i=0; i<8; i++) {
    lcd_data(
              (1<<(i+0 )) |
              (1<<(i+8 )) |
              (1<<(i+16)) |
              (1<<(i+24)));
  }
  lcd_loc(0, 1);
  lcd_puts(version_string);

  sleep(5);
  ldprintf(1, "sbrk(0)=%08X", sbrk(0));

  ldprintf(2, "*p=%.08x", p=malloc(1023));
  ldprintf(3, "m  %.08x", malloc(512));
  ldprintf(3, "m  %.08x", malloc( 98));
  ldprintf(3, "m  %.08x", malloc(16331));
  free(p);
  ldprintf(3, "free p");
  ldprintf(2, "m=p%.08x", p=malloc(1023));

#if 1
  for (i=0;;) i=ldprintf(3, "Key %04d: %02X %s      ", nk++, i, keys[i]);
#endif
}
