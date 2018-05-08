

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
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PB.5 �����
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
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



