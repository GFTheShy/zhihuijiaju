#ifndef _DRV_BEEP_H_
#define _DRV_BEEP_H_
#include "stm32f4xx.h"
#include "drv_delay.h"

//PA3

#define DHT_HIGH GPIO_SetBits(GPIOA,  GPIO_Pin_3)  //拉高
#define DHT_LOW GPIO_ResetBits(GPIOA,  GPIO_Pin_3)  //拉低
#define DHT_READ GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_3)  //读取


void SET_DHT11_MODE_IN(void);

void SET_DHT11_MODE_OUT(void);

int Start_ask(void);

u8 Read_Bit(void);

u8 Read_byte(void);

int DHT11_WORK(u8 *tmp,u8 *humi);

#endif

