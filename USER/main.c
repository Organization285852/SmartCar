

#include "sys.h"
#include "delay.h"
#include "scheduler.h"
#include "hc_sr04.h"
#include "usart.h"
#include "iic_lcd1602.h"
#include "motor.h"
#include "DHT11.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PB.5 输出高
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  SysTick_Configuration();
	init_lcd1602();
	display_str(0,0,"T:   C ");
	display_str(8,0,"H:  %");
	motor_init();
	DHT11_Init();
	uart_init(115200);
	LED_Init();
	Init_hc_sr04();
  while(1)
  {
    Duty_Loop();
  }
  return 0;
}



