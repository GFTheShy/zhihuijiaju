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
extern int oled_ui;	//三个界面 
extern int beep_flag;

void beep_func(uint16_t tmp)
{
	if(oled_ui==1) return;
	if(tmp>=26)
		{
			beep_earning();
		}
	else
		{
		}
}
void beep_earning(void)
{

	beep_open();
	drv_Systick_delayms(200);
	beep_close();
	drv_Systick_delayms(200);

	beep_open();
	drv_Systick_delayms(200);
	beep_close();
	drv_Systick_delayms(200);

	beep_open();
	drv_Systick_delayms(200);
	beep_close();
	drv_Systick_delayms(1000);
}

