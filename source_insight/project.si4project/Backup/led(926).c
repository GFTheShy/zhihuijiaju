#include "led.h"

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10);
}
void led_open(int n)
{
	if(n==1)
	{
		GPIO_ResetBits(GPIOE,  GPIO_Pin_8);
	}
	else if(n==2)
	{
		GPIO_ResetBits(GPIOE,  GPIO_Pin_9);
	}
	else if(n==3)
	{
		GPIO_ResetBits(GPIOE,  GPIO_Pin_10);
	}
}
void led_close(int n)
{
	if(n==1)
	{
		GPIO_SetBits(GPIOE,  GPIO_Pin_8);
	}
	else if(n==2)
	{
		GPIO_SetBits(GPIOE,  GPIO_Pin_9);
	}
	else if(n==3)
	{
		GPIO_SetBits(GPIOE,  GPIO_Pin_10);
	}
}
void led_openAll(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10);
}
void led_closeAll(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10);
}
void led_func(uint16_t light)
{
	


}


