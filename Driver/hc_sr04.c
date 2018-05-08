#include "hc_sr04.h"
#include "delay.h"



float distance[2];


//��ʱ��4ͨ��1���벶������

TIM_ICInitTypeDef  TIM4_ICInitStructure;

void TIM4_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;  //PB6 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);			//PB6 ����
	
	//��ʼ����ʱ��4 TIM4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM4���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
		//��ʼ��TIM4���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC2S=01 	ѡ������� IC2ӳ�䵽TI2��
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI2��
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
}

u8  TIM4CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ
u16	TIM4CH1_CAPTURE_F;	
u8  TIM4CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH2_CAPTURE_VAL;	//���벶��ֵ
u16	TIM4CH2_CAPTURE_F;	
/*
//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{ 
 	if((TIM4CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {	    
			if(TIM4CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH1_CAPTURE_VAL=0XFFFF;
				}else TIM4CH1_CAPTURE_STA++;
			}	 
		}
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
    {	
			if(TIM4CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH1_CAPTURE_VAL=TIM_GetCapture1(TIM4);
		   	TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
      else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH1_CAPTURE_STA=0;			//���
				TIM4CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM4,0);
				TIM4CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   	TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}
*/

//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
    {	
			if(TIM4CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH1_CAPTURE_VAL=TIM_GetCapture1(TIM4);
				if(TIM4CH1_CAPTURE_VAL>TIM4CH1_CAPTURE_F)
				{
					TIM4CH1_CAPTURE_VAL = TIM4CH1_CAPTURE_VAL-TIM4CH1_CAPTURE_F;
				}
				else
				{
					TIM4CH1_CAPTURE_VAL = TIM4CH1_CAPTURE_VAL+0xFFFF-TIM4CH1_CAPTURE_F;
				}
				distance[0] = TIM4CH1_CAPTURE_VAL*340/2000;
				//distance[0] /= 10; //cm
				TIM4CH1_CAPTURE_STA = 0;
		   	TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
      else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH1_CAPTURE_F = TIM4->CNT;//��¼��ʱ��ֵ
				TIM4CH1_CAPTURE_STA=0X40;		//��ǲ�����������
		   	TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}	

    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//����2���������¼�
    {	
			if(TIM4CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH2_CAPTURE_VAL=TIM_GetCapture2(TIM4);
				if(TIM4CH2_CAPTURE_VAL>TIM4CH2_CAPTURE_F)
				{
					TIM4CH2_CAPTURE_VAL = TIM4CH2_CAPTURE_VAL-TIM4CH2_CAPTURE_F;
				}
				else
				{
					TIM4CH2_CAPTURE_VAL = TIM4CH2_CAPTURE_VAL+0xFFFF-TIM4CH2_CAPTURE_F;
				}
				distance[1] = TIM4CH2_CAPTURE_VAL*340/2000;
				//distance[1] /= 10; //cm
				TIM4CH2_CAPTURE_STA = 0;
		   	TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC2P=0 ����Ϊ�����ز���
			}
      else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH2_CAPTURE_F = TIM4->CNT;//��¼��ʱ��ֵ
				TIM4CH2_CAPTURE_STA=0X40;		//��ǲ�����������
		   	TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
			}	

		}	
		
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
}

void Init_hc_sr04(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_8;  //PB5 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PB5 ����
  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//�������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_14;  //PB6 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_8);						 //PB5 
	
	TIM4_Cap_Init(0XFFFF,72-1);  //��1Mhz ��Ƶ�ʼ��� 
}

float temp=0; 
void capture_duty(void)
{
/*
  if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
  {
    temp=TIM4CH1_CAPTURE_STA&0X3F;
    temp*=65536;//���ʱ���ܺ�
    temp+=TIM4CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
		temp = 340*temp/2000;
    TIM4CH1_CAPTURE_STA=0; //������һ�β���
  }
	*/
}

void hc_sr04_duty(void) //50ms����
{
	static u8  tim=0;
	tim++;
	if(tim>10)
	{
		tim = 0;
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_8);								//PB5
		//delay_us(20);
		delay_ms(1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_8);						 	//PB5
		
	}
}

