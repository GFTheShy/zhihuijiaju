#include "fan.h"

void fan_init(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init( GPIOB,& GPIO_InitStruct);

	GPIO_ResetBits( GPIOB,  GPIO_Pin_15);
}

void fan_open(void)
{
GPIO_SetBits( GPIOB,  GPIO_Pin_15);
}
void fan_close(void)
{
GPIO_ResetBits( GPIOB,  GPIO_Pin_15);
}
void fan_setSpeed(int val)
{
	
}
void fan_func(uint16_t humi)
{
	
}