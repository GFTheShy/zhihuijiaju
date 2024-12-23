#ifndef _ADC_H
#define _ADC_H
#include "stm32f4xx.h"

void ADC_config(void);
u16 Get_ADC_Value();
unsigned short int getAdcAveragel(unsigned char times);
#endif