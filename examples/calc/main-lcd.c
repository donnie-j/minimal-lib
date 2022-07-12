#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "lcd-drv.h"
#include "rpn.h"

#if 0
static char * keys[] = {
/*        0       1       2       3        4     5     6       7 */
/* 00 */ "",     "SUM+", "STO",  "",      "UP", "DN", "SHFT", "",
/* 08 */ "EXIT", "1/x",  "RCL",  "ENTER", "",   "",   "",     "",
/* 10 */ "",     "SRQT", "R DN", "x y",   "7",  "4",  "1",    "0",
/* 18 */ "",     "LOG",  "SIN",  "+/-",   "8",  "5",  "2",    ".",
/* 20 */ "",     "LN",   "COS",  "E",     "9",  "6",  "3",    "R/S",
/* 28 */ "",     "XEQ",  "TAN",  "BS",    "/",  "*",  "-",    "+" };
#endif
static char key_map[] = {
/*        0    1    2    3    4    5    6    7 */
/* 00 */ '_', 'a', 'm', '_', 'u', 'i', '~', '_',
/* 08 */ '_', 'v', 'r', 'z', '_', '_', '_', '_',
/* 10 */ '_', 'q', 'd', 'w', '7', '4', '1', '0',
/* 18 */ '_', 'o', 's', 'n', '8', '5', '2', '.',
/* 20 */ '_', 'l', 'c', 'e', '9', '6', '3', '\\',
/* 28 */ '_', 'x', 't', 'b', '/', '*', '-', '+' };

int
main (int argc, char *argv[])
{
  fp_comp_t d = zero_fp;
  int k, ch;
  int s = 0;

  d = key_process(d, '0');
  while (1) {
    lcd_puts(21, 0, s?"S":" ");
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
 
    d = key_process(d, ch);

    key_wait(0);
  }
}
