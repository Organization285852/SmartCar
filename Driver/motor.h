#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define A_H    PAout(1)
#define A_L    PAout(2)
#define B_H    PAout(3)
#define B_L    PAout(4)

#define TARR    1000

void motor_init(void);
void set_motor_speed(u8 ch,u8 speed);
void Set_motor_dir(u8 ch,u8 dir);
void motor_stop(u8 ch);

#endif


