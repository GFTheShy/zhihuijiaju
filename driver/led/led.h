#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"

void led_init(void);
void led_open(int n);
void led_close(int n);
void led_openAll(void);
void led_closeAll(void);
void led_func(uint16_t light);
#endif

