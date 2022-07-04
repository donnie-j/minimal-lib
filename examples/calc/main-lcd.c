#include <stdio.h>
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
  char buf[64];
  int nk = 0;

  lcd_loc(0, 0);
  lcd_puts ("LCD init\n");
  for (i=0; i<8; i++) {
    lcd_data(
              (1<<(i+0 )) |
              (1<<(i+8 )) |
              (1<<(i+16)) |
              (1<<(i+24)));
  }

  sprintf(buf, "This %d", 314159);
  lcd_loc(0,1); lcd_puts(buf);

  for (;;) {
    lcd_loc(0,2); lcd_puts("Wait...     ");

    sprintf(buf, "Keyscan: %02X", key_wait(1));
    lcd_loc(0,2); lcd_puts(buf);

    i = key();
    sprintf(buf, "Key %04d: %02X %s      ", ++nk, i, keys[i]);
    lcd_loc(0,3); lcd_puts(buf);

    key_wait(0);
  }
}
