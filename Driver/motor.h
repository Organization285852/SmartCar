#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define A_H    PAout(0)
#define A_L    PAout(1)
#define B_H    PAout(2)
#define B_L    PAout(3)

#define TARR    50

void motor_init(void);
void set_motor_speed(u8 ch,u8 speed);
void Set_motor_dir(u8 ch,u8 dir);
void motor_stop(u8 ch);

#endif


