#ifndef _FAN_H
#define _FAN_H
#include "stm32f4xx.h"

void fan_init(void);
void fan_setSpeed(int val);//val:0~1000
void fan_func(uint16_t humi);
#endif