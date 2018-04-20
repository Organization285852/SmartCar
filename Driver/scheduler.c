/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * 作者   ：匿名科创
 * 文件名  ：scheduler.c
 * 描述    ：任务调度
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/
#include "scheduler.h"
#include "delay.h"

s16 loop_cnt;

loop_t loop;
void Loop_check()  //TIME INTTERRUPT 1ms执行一次
{
	loop.time++; //u16
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;

	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //每累加一次，证明代码在预定周期(1ms)内没有跑完。
	}
	else
	{	
		loop.check_flag = 1;	//该标志位在循环的最后被清零
	}
}

float duty_time[6][2];
void Duty_1ms(void)
{
  
  duty_time[0][1] = GetSysTime_us()/1000000.0f - duty_time[0][0];
  
	duty_time[0][0] = GetSysTime_us()/1000000.0f;
}

void Duty_2ms(void)
{
  
  duty_time[1][1] = GetSysTime_us()/1000000.0f - duty_time[1][0];
	duty_time[1][0] = GetSysTime_us()/1000000.0f;
}

void Duty_5ms(void)
{
  
	duty_time[2][1] = GetSysTime_us()/1000000.0f - duty_time[2][0];
	duty_time[2][0] = GetSysTime_us()/1000000.0f;

}


void Duty_10ms(void)
{
  duty_time[3][1] = GetSysTime_us()/1000000.0f - duty_time[3][0];
  
	duty_time[3][0] = GetSysTime_us()/1000000.0f;
}

void Duty_20ms(void)
{
  duty_time[4][1] = GetSysTime_us()/1000000.0f - duty_time[4][0];
  
	duty_time[4][0] = GetSysTime_us()/1000000.0f;
}
u16 t;
void Duty_50ms(void)
{
  duty_time[5][1] = GetSysTime_us()/1000000.0f - duty_time[5][0];
  duty_time[5][0] = GetSysTime_us()/1000000.0f;
}

void Duty_Loop(void)   					//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{
	if( loop.check_flag == 1 )
	{
		Duty_1ms();							//周期1ms的任务
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//周期2ms的任务
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			
			Duty_10ms();					//周期10ms的任务
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//周期20ms的任务
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//周期50ms的任务
		}	
		loop.check_flag = 0;		//循环运行完毕标志
	}
}


	/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
	

