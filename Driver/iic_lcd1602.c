#include "sys.h"
#include "myiic.h"
#include "iic_lcd1602.h"
#include "delay.h"
#include "DHT11.h"

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
  delay_ms(1);
  PCF8574_write_port(com1&0xf8);  // RS =0;RW=0;EN=0;
  
  //低四位
  com2 = com<<4;
  com2 = com2|0x0f;
  PCF8574_write_port(com2&0xfc);  // RS =0;RW=0;EN=1;
  delay_ms(1);
  PCF8574_write_port(com2&0xf8);  // RS =0;RW=0;EN=0;
}


void write_data(unsigned char data)
{
  unsigned char data1,data2;
  data1 = data |0x0f;
  PCF8574_write_port(data1&0xfd);  // RS =1;RW=0;EN=1;
  delay_ms(1);
  PCF8574_write_port(data1&0xf9);  // RS =1;RW=0;EN=0;
  
  //低四位
  data2 = data<<4;
  data2 = data2|0x0f;
  PCF8574_write_port(data2&0xfd);  // RS =1;RW=0;EN=1;
  delay_ms(1);
  PCF8574_write_port(data2&0xf9);  // RS =1;RW=0;EN=0;
}

void init_lcd1602(void)
{
	delay_ms(50);
  IIC_Init();
	wite_com(0x28);//四线模式四设置
	delay_us(100);
	wite_com(0x28);//四线模式四设置
	delay_us(100);
	wite_com(0x28);//四线模式四设置
	
	PCF8574_write_port(0x2c); // RS =0;RW=0;EN=1;
	delay_ms(2);
  PCF8574_write_port(0x28); // RS =0;RW=0;EN=0; 使能4线
	
	delay_ms(2);
	wite_com(0x28);//四线模式四设置
	delay_ms(2);
	wite_com(0x01);//显示清屏
	delay_ms(2);
  wite_com(0x0C);//开显
	delay_ms(2);
  wite_com(0x06);//光标自动加一
	delay_ms(2);
}

void lcd1602_set_pos(unsigned char x,unsigned char y)
{
  unsigned char addr;
  switch(y)
  {
    case 0:addr = 0x80 + x;break;
    case 1:addr = 0x80 + 0x40 + x;break;
    default:return;
  }
  wite_com(addr);
}



void  display_str(u8 x,u8 y,u8 *p)
{
  lcd1602_set_pos(x,y); 
  for(;*p!='\0';p++)
  write_data(*p);
}
u8 shu[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
void display_num(u8 x ,u8 y,u8 num)
{
	u8 ge,shi,bai;								  
	lcd1602_set_pos(x,y);
	ge=num%10;
	shi=num%100/10;
	bai = num/100;
	if(bai)
	{
		write_data(shu[bai]);
		write_data(0x30+shi);
		write_data(shu[ge]);
	}
	else
	{
		if(shi)
		{
			write_data(shu[shi]);
			write_data(shu[ge]);
		}
		else
		{
			write_data(shu[ge]);
		}
	}
}

extern 	u8 T,H;
extern float distance[2];
void lcd1602_duty(void)
{
	static u8 tim=0;
	tim++;
	if(tim>20)
	{
		tim = 0;
		DHT11_Read_Data(&T,&H);
		display_num(2,0,T);
		write_data(0xdf);//显示 °
		display_num(10,0,H);
		
		display_str(0,1,"                ");
		if(distance[1]<400)
		{
			display_str(0,1,"D1:   cm");
			display_num(3,1,(u8)(distance[1]/10));
		}
		if((distance[0]>50))
		{
			display_str(8,1,"D2:   cm");
			display_num(11,1,(u8)(distance[0]/10));
			LED0 ^= 1;
			
		}
		else
		{
			LED0 = 1;
		}
	}
}





