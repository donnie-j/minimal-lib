#ifndef __LCD_DRV_H__
#define __LCD_DRV_H__

#define KEYPORT (*(volatile unsigned long  *)0xabcd0000)
#define KEY_PRECHARGE_READ 0x80
#define KEY_PRECHARGE_ALL  0xC0
#define KEY_IDLE           0x00
#define KEY_NONE           0x7F
#define KEY_ALL            0xFF
#define KEY_ON             0x80

#define LCDDATA (*(volatile unsigned long  *)0xabcd0044)
#define LCDINST (*(volatile unsigned long  *)0xabcd0040)

void key_precharge();
int key_wait(int h);
int key();

void lcd_data(unsigned int v);
void lcd_inst(unsigned int v);
void lcd_loc(unsigned int x, unsigned int y);
void lcd_puts(const char *s);

#endif /* __LCD_DRV_H__ */
