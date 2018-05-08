#include "syn7318.h"
#include "sys.h"
#include "usart.h"
#include "string.h"


#define  DEFAULT_DIC	"×ó×ª 1|Ïò×ó×ª 1|Çë×ó×ª 1|ÓÒ×ª 2|ÏòÓÒ×ª 2|ÇëÓÒ×ª 2|"  

u8 syn_7318=0;
u8 syn_7318_cmd=0;


void wait_wake_up(void)
{
	u1_sendbyte(0xFD);
	u1_sendbyte(0x00);
	u1_sendbyte(0x02);
	u1_sendbyte(0x51);
	u1_sendbyte(0x09);
}

void distinguish(void)
{
	u1_sendbyte(0xFD);
	u1_sendbyte(0x00);
	u1_sendbyte(0x02);
	u1_sendbyte(0x10);//ÓïÒôÊ¶±ðÃüÁî
	u1_sendbyte(0x05);//´Êµä
}
void check_state(void)
{
	u1_sendbyte(0xFD);
	u1_sendbyte(0x00);
	u1_sendbyte(0x01);
	u1_sendbyte(0x21);
}
//ÉèÖÃ´Êµä
void set_dic(char *buf)
{
	u16 len;
	char *p = buf;
	len = strlen(buf)+3;
	u1_sendbyte(0xFD);
	u1_sendbyte(len>>8);
	u1_sendbyte(len&0xFF);
	u1_sendbyte(0x1F);//ÉèÖÃ´ÊµäÃüÁî
	u1_sendbyte(0x01);//±àÂë¸ñÊ½GBK
	u1_sendbyte(0x05);//´Êµä±àºÅ
	while(*p!=0)
	{
		u1_sendbyte(*p);
		p++;
	}
}

void SYN7318_duty(void)//10ms
{
	int i=0;
	 static u8 tim=0;
	tim++;
	if(tim>19) //200ms
	{
		tim = 0;
		check_state();
	}
	
	
	
	if(USART_RX_STA&0x8000)
	{	
		while(i < (USART_RX_STA&0x7fff))
		{
			if(USART_RX_BUF[i]==0xFC)
			{
				if(USART_RX_BUF[i+3]==0x01)
				{
					syn_7318_cmd = USART_RX_BUF[i+8];
					syn_7318 = 1; //¼ÌÐøÊ¶±ð
				}
				if(USART_RX_BUF[i+3]==0x4F)//¿ÕÏÐ×´Ì¬¡£
				{
					syn_7318 = 1; //¼ÌÐøÊ¶±ð
				}
			}
			i++;
		}
		USART_RX_STA =0;
	}
	
	switch(syn_7318)
	{
		case 1:
		{
			//set_dic(DEFAULT_DIC);
			syn_7318 = 0;
			distinguish();
		}
		break;
		default :break;
	}
}
