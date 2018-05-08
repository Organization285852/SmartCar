
#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "sys.h"


typedef struct{
	unsigned char d1;
}SR04;


void Init_hc_sr04(void);

void hc_sr04_duty(void);

void capture_duty(void);

extern float distance[2];


#define R_IN   (!PBin(14))
#define L_IN  (!PBin(9))

#endif



