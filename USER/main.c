

#include "sys.h"
#include "delay.h"
#include "scheduler.h"

int main(void)
{
  SysTick_Configuration();
  while(1)
  {
    Duty_Loop();
  }
  return 0;
}



