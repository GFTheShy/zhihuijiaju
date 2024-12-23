#ifndef _DRV_BEEP_H_
#define _DRV_BEEP_H_
#include "stm32f4xx.h"

void drv_Systick_Init(u8 sysclk);

void drv_Systick_delayxms(u32 nms);

void drv_Systick_delayms(u16 nms);

void drv_Systick_delayus(u32 nus);

void delay_us(u32 nus);

#endif

