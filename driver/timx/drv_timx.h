#ifndef _DRV_TIMX_H
#define _DRV_TIMX_H
#include "stm32f4xx.h"

void drv_tim6_init(void);

void drv_tim9_init(void);

void drv_tim1_led2pwm_init(void);
//fan tim1 ch3n pb15
void drv_tim1_fanpwm_init(void);

void drv_tim10_init(void);//当fan手动关闭10s后进入自动模式
#endif