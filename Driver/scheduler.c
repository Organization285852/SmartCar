/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��scheduler.c
 * ����    ���������
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
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
void Loop_check()  //TIME INTTERRUPT 1msִ��һ��
{
	loop.time++; //u16
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;

	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //ÿ�ۼ�һ�Σ�֤��������Ԥ������(1ms)��û�����ꡣ
	}
	else
	{	
		loop.check_flag = 1;	//�ñ�־λ��ѭ�����������
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
			syn_7318_cmd = next_cmd;//����ǰ��
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

void Duty_20ms(void) //�������ƺͱ���
{
	static u8 speed=20;
  duty_time[4][1] = GetSysTime_us()/1000000.0f - duty_time[4][0];
	
	if( ((syn_7318_cmd!=5)&&(syn_7318_cmd!=0))&&(next_cmd==0))
	{
		if((distance[1]<90)&&(distance[1]>5))//ǰ���ϰ�
		{
			if(R_IN&&L_IN)//���Ҷ���
			{
				syn_7318_cmd = 4;//����
			}
			else if(R_IN)
			{
				syn_7318_cmd = 2;//��ת
			}
			else if(L_IN)
			{
				syn_7318_cmd = 1;//��ת
			}
			else
			{
				syn_7318_cmd = 1;//��ת
			}
			next_cmd = 3;
		}
		else
		{
			if(R_IN&&L_IN)//���Ҷ���
			{
				syn_7318_cmd = 4;//����
				next_cmd = 2;
			}
			else if(R_IN)
			{
				syn_7318_cmd = 2;//��ת
				next_cmd = 3;
			}
			else if(L_IN)
			{
				syn_7318_cmd = 1;//��ת
				next_cmd = 3;
			}
		}
	}
	if((distance[0]>50))//��̨��
	{
			motor_stop(1);
			motor_stop(2);
			next_cmd = 0;
	}
  switch(syn_7318_cmd)
	{
		case 1://��ת
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,1);
			set_motor_speed(2,speed);
		
		}break;
		case 2://��ת
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,speed);
			set_motor_speed(2,1);
		}break;
		case 3://ǰ��
		{
			Set_motor_dir(1,1);
			Set_motor_dir(2,1);
			set_motor_speed(1,speed);
			set_motor_speed(2,speed);
		}break;
		case 4://����
		{
			Set_motor_dir(1,0);
			Set_motor_dir(2,0);
			set_motor_speed(1,speed);
			set_motor_speed(2,speed);
		}break;
		case 5://ֹͣ
		{
			motor_stop(1);
			motor_stop(2);
			next_cmd = 0;
		}break;
		case 6://����
		{
			if(speed<80)
			{
				speed += 10;
				set_motor_speed(1,speed);
				set_motor_speed(2,speed);
			}
		}
		case 7://����
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

void Duty_Loop(void)   					//�����������Ϊ1ms���ܵĴ���ִ��ʱ����ҪС��1ms��
{
	if( loop.check_flag == 1 )
	{
		Duty_1ms();							//����1ms������
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}	
		loop.check_flag = 0;		//ѭ��������ϱ�־
	}
}


	/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
	

