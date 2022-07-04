
#include "lcd-drv.h"
#include "font5x7.h"

static int ffsl(long i) { return i ? __builtin_ctzl(i) + 1 : 0; }

void
key_precharge()
{
  volatile int i;

  KEYPORT = KEY_NONE;
  for (i=0; i<3; i++) {}
  KEYPORT = KEY_PRECHARGE_ALL;
  for (i=0; i<3; i++) {}
}
 
int
key_wait(int h)
{
  volatile int i;
  int res;

  key_precharge();
  while (1) {
    KEYPORT = KEY_IDLE;
    for (i=0; i<12000; i++) {;} /* about 20ms */
    KEYPORT = KEY_PRECHARGE_READ;
    for (i=0; i<12; i++) {;}    /* about 20us */

    KEYPORT = KEY_IDLE;
    for (i=0; i<12; i++) {;}    /* about 20us */
    res=KEYPORT & KEY_ALL;

    if ((res == KEY_ALL && !h) || (res != KEY_ALL && h)) break;
  }

  return res ^ KEY_ALL;
}

int
key()
{
  volatile int i;
  int x, res;

  key_precharge();
  for (x=0; x<7; x++) {
    KEYPORT = KEY_NONE ^ (1<<x);
    for (i=0; i<120; i++) {;}
    res=KEYPORT & KEY_ALL;
    KEYPORT = KEY_PRECHARGE_ALL;
    for (i=0; i<12; i++) {;}
    if ((res & KEY_ALL) != KEY_ALL) return (x<<3) | ffsl(res ^ KEY_ALL);
  }
  return res ^ KEY_ALL;
}

unsigned char lcd_init_data[] = { 0x40, 0xA1, 0xC0, 0xA6, 0xA2, 0x2F, 0xF8, 0x00, 0x23, 0x81, 0x1F, 0xAC, 0x00, 0xAF };

void
lcd_data(unsigned int v)
{
  while((LCDDATA) & 1) {;}
  LCDDATA = v;
}

void
lcd_inst(unsigned int v)
{
  while((LCDDATA) & 1) {;}
  LCDINST = v;
}

void
lcd_loc(unsigned int x, unsigned int y)
{
  lcd_inst(0xB0 | (y & 3));
  lcd_inst(0x10 | (((x*6) & 0xf0) >> 4));
  lcd_inst(0x00 |  ((x*6) & 0x0f));
}

void
lcd_puts(const char *s)
{
  int i, j;
  unsigned char v[4];
  unsigned int *l = (void *)v;

  i = 0;
  for (;*s;s++) {
    for (j=0; j<6; j++) {
      v[i++] = j == 5 ? 0 : font[(*s - FONT5X7_START)*5 + j];
      if (i>3) {
        lcd_data(*l);
        i = 0;
      }
    }
  }
  for (;i<4;i++) v[i] = 0;
  lcd_data(*l);
}

__attribute__ ((constructor))
void
lcd_init()
{
  volatile int i,j;

  key_precharge();

  for (i=0; i<60000; i++) {}
  for (i=0; i<sizeof(lcd_init_data); i++) lcd_inst(lcd_init_data[i]);

  for (i=0; i<4; i++) {
    lcd_loc(0, i); lcd_puts ("");
    for (j=0; j<132; j++) lcd_data(0);
  }
}
