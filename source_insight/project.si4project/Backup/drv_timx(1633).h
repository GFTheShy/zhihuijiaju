#ifndef _DRV_TIMX_H
#define _DRV_TIMX_H
#include "stm32f4xx.h"

void drv_tim6_init(void);

void drv_tim9_init(void);

void drv_tim1_led1pwm_init(void);
void drv_tim1_led2pwm_init(void);
void drv_tim1_led3pwm_init(void);
void drv_tim2_beeppwm_init(void);//tim2-ch3

#endif