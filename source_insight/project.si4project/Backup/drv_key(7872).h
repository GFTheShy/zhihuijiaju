#ifndef _DRV_KEY_H
#define _DRV_KEY_H
#include "stm32f4xx.h"

#include "led.h"
#include "drv_beep.h"
#include "drv_delay.h"

void key_init(void);
void key_func(void);
extern int key1_flag,key2_flag,key3_flag,key4_flag;

#endif

