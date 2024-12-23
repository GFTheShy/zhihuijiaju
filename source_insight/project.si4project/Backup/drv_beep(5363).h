#ifndef _DRV_BEEP_H_
#define _DRV_BEEP_H_
#include "stm32f4xx.h"

void beep_init(void);
void beep_open(void);
void beep_close(void);
void beep_func(uint16_t tmp);

#endif
