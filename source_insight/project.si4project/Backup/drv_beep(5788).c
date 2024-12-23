#include "drv_beep.h"
//beep:GPIOB_PIN_10
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOB,  GPIO_Pin_10);

}
void beep_open(void)
{
	GPIO_SetBits(GPIOB,  GPIO_Pin_10);
}
void beep_close(void)
{
	GPIO_ResetBits(GPIOB,  GPIO_Pin_10);
}
void beep_func(uint16_t tmp)
{
	
}

