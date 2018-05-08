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
#include "hc_sr04.h"
#include "iic_lcd1602.h"
#include "syn7318.h"
#include "motor.h"
#include "dht11.h"

s16 loop_cnt;

loop_t loop;
u8 T,H;
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
int t1ms=0;
u8 next_cmd=0;
void Duty_1ms(void)
{
  
  duty_time[0][1] = GetSysTime_us()/1000000.0f - duty_time[0][0];
  
	duty_time[0][0] = GetSysTime_us()/1000000.0f;
	if(next_cmd)
	{
		t1ms++;
		if(t1ms>1500)
		{
			syn_7318_cmd = next_cmd;//继续前进
			t1ms = 0;
			next_cmd = 0;
		}
	}
}
void Duty_2ms(void)
{
	capture_duty();
  
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
  SYN7318_duty();
	duty_time[3][0] = GetSysTime_us()/1000000.0f;
}

void Duty_20ms(void) //语音控制和避障
{
	static u8 speed=20;
  duty_time[4][1] = GetSysTime_us()/1000000.0f - duty_time[4][0];
	
	if( ((syn_7318_cmd!=5)&&(syn_7318_cmd!=0))&&(next_cmd==0))
	{
		if((distance[1]<90)&&(distance[1]>5))//前方障碍
		{
			if(R_IN&&L_IN)//左右都有
			{
				syn_7318_cmd = 4;//后退
			}
			else if(R_IN)
			{
				syn_7318_cmd = 2;//左转
			}
			else if(L_IN)
			{
				syn_7318_cmd = 1;//右转
			}
			else
			{
				syn_7318_cmd = 1;//右转
			}
			next_cmd = 3;
		}
		else
		{
			if(R_IN&&L_IN)//左右都有
			{
				syn_7318_cmd = 4;//后退
				next_cmd = 2;
			}
			else if(R_IN)
			{
				syn_7318_cmd = 2;//左转
				next_cmd = 3;
			}
			else if(L_IN)
			{
				syn_7318_cmd = 1;//右转
				next_cmd = 3;
			}
		}
	}
	if((distance[0]>50))//有台阶
	{
			motor_stop(1);
			motor_stop(2);
			next_cmd = 0;
	}
  switch(syn_7318_cmd)
	{
		case 1://右转
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,1);
			set_motor_speed(2,speed);
		
		}break;
		case 2://左转
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,speed);
			set_motor_speed(2,1);
		}break;
		case 3://前进
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,speed);
			set_motor_speed(2,speed);
		}break;
		case 4://后退
		{
			Set_motor_dir(1,0);
			Set_motor_dir(2,0);
			set_motor_speed(1,speed);
			set_motor_speed(2,speed);
		}break;
		case 5://停止
		{
			motor_stop(1);
			motor_stop(2);
			next_cmd = 0;
		}break;
		case 6://加速
		{
			if(speed<80)
			{
				speed += 10;
				set_motor_speed(1,speed);
				set_motor_speed(2,speed);
			}
		}
		case 7://减速
		{
			if(speed>30)
			{
				speed -= 10;
				set_motor_speed(1,speed);
				set_motor_speed(2,speed);
			}
		}
		default:break;
	}
	duty_time[4][0] = GetSysTime_us()/1000000.0f;
}
u16 t;
void Duty_50ms(void)
{
	hc_sr04_duty();
  duty_time[5][1] = GetSysTime_us()/1000000.0f - duty_time[5][0];
  duty_time[5][0] = GetSysTime_us()/1000000.0f;
	lcd1602_duty();
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
	

