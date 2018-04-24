#include "sys.h"
#include "myiic.h"
#include "iic_lcd1602.h"
#include "delay.h"

#define PCF8574_ADDR    (0x27<<1)
/*
  P0     <->   RS
  P1     <->   RW
  P2     <->   EN
  P4-P7  <->  DB4-DB7
*/
void PCF8574_write_port(unsigned char data)
{
  IIC_Start();
  IIC_Send_Byte(PCF8574_ADDR);
  IIC_Wait_Ack();
  IIC_Send_Byte(data);
  IIC_Wait_Ack();
  IIC_Stop();
}


void wite_com(unsigned char com)
{
  unsigned char com1,com2;
  com1 = com |0x0f;
  PCF8574_write_port(com1&0xfc);  // RS =0;RW=0;EN=1;
  delay_ms(2);
  PCF8574_write_port(com1&0xf8);  // RS =0;RW=0;EN=0;
  
  //低四位
  com2 = com<<4;
  com2 = com2|0x0f;
  PCF8574_write_port(com2&0xfc);  // RS =0;RW=0;EN=1;
  delay_ms(2);
  PCF8574_write_port(com2&0xf8);  // RS =0;RW=0;EN=0;
}


void write_data(unsigned char data)
{
  unsigned char data1,data2;
  data1 = data |0x0f;
  PCF8574_write_port(data1&0xfd);  // RS =0;RW=0;EN=1;
  delay_ms(2);
  PCF8574_write_port(data1&0xf9);  // RS =0;RW=0;EN=0;
  
  //低四位
  data2 = data<<4;
  data2 = data2|0x0f;
  PCF8574_write_port(data2&0xfd);  // RS =0;RW=0;EN=1;
  delay_ms(2);
  PCF8574_write_port(data2&0xf9);  // RS =0;RW=0;EN=0;
}

void init_lcd1602(void)
{
  IIC_Init();
  PCF8574_write_port(0x08);
  wite_com(0x0f);
  wite_com(0x28);//显示模式设置高两位，设置4线
  PCF8574_write_port(0x0c); // RS =0;RW=0;EN=1;
  PCF8574_write_port(0x08); // RS =0;RW=0;EN=0; 
  wite_com(0x28);
  wite_com(0x01);//显示清屏
  wite_com(0x0c);//开显示 ，不显示光标
  wite_com(0x06);//光标自动加一
}

void lcd1602_set_pos(unsigned char x,unsigned char y)
{
  unsigned char addr;
  switch(y)
  {
    case 0:addr = 0x80 + x*8;break;
    case 1:addr = 0x80 + 0x40 + x*8;break;
    default:return;
  }
  wite_com(addr);
}






