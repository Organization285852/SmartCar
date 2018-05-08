#ifndef __IIC_LCD1602_H
#define __IIC_LCD1602_H
#include "sys.h"





void lcd1602_set_pos(unsigned char x,unsigned char y);
void init_lcd1602(void);
void lcd1602_duty(void);
void display_num(u8 x ,u8 y,u8 num);
void display_str(u8 x,u8 y,u8 *p);
void write_data(unsigned char data);









#endif



