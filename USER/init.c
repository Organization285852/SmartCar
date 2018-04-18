#include "init.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"    
#include "usart3.h"
#include "A7.h"  
#include "string.h"
#include "stdio.h"
#include "timer.h"
#include "malloc.h"
#include "lcd.h"
#include "hb_0004.h"
#include "w25qxx.h"
#include "usart2.h"
#include "iic.h"
#include "transfer.h"
#include "led.h"
#include "ADC1.h"
#include "key.h"
void SysInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Configuration();
	my_mem_init(SRAMIN);		//初始化内部内存池
	W25QXX_Init();
	//蓝牙初始化	 
	hb_0004Init();
	usart2_init(57600);
	usart3_init(115200);
	A7_GPIO_init();
	TIM3_Int_Init(1000,72-1);
	TIM1_Int_Init(1000,72-1);//数据采样计时，标准
	IIC_Init();
	BMD101_Init();
	ADC1Init();
	LED_Init();
	EXTI8_Init();
	TIM4_PWM_Init(1000,1);
}




