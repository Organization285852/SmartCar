#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

#ifdef __cplusplus
extern "C"
{
#endif

	 
void  SysTick_Configuration(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
uint32_t millis( void );

#ifdef __cplusplus
}
#endif
	
#endif





























