#include <stdio.h>
#include <ctype.h>
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

static char key_map[] = {
/*        0    1    2    3    4    5    6    7 */
/* 00 */ '_', 'a', 'm', '_', 'u', 'i', '~', '_',
/* 08 */ '_', 'v', 'r', 'z', '_', '_', '_', '_',
/* 10 */ '_', 'q', 'd', 'w', '7', '4', '1', '0',
/* 18 */ '_', 'o', 's', 'n', '8', '5', '2', '.',
/* 20 */ '_', 'l', 'c', 'e', '9', '6', '3', '\\',
/* 28 */ '_', 'x', 't', 'b', '/', '*', '-', '+' };

extern char version_string[];

int
main (int argc, char *argv[])
{
  char buf[64];
  int i, k, ch;
  int s = 0;
  int nk = 0;
  unsigned long *p;

  lcd_loc(0, 1);
  lcd_puts(version_string);

  sleep(2);

  while (1) {
    sprintf(buf, "[%c]%03d Key wait      ", s?'S':' ', nk++);
    lcd_loc(0,1); lcd_puts(buf);
    key_wait(1);
    k = key();
    ch = key_map[k];
    if (ch == '~') {
      s ^= 1;
      key_wait(0);
      continue;
    }
    if (s) ch = toupper(ch);
    s = 0;
 
    sprintf(buf, "Got [%c] for %s      ", ch, keys[k]);
    lcd_loc(0,1); lcd_puts(buf);

    key_wait(0);
  }
}
