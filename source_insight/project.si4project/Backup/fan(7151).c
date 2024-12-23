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
extern int oled_ui;	//三个界面 
extern int fan_flag;
//val:0~1000
void fan_setSpeed(int val)
{
	TIM_SetCompare3( TIM1,  1000-val);
}
//humi:20~70
void fan_func(uint16_t humi)
{
	if(oled_ui==1) return;
	if(fan_flag==0) return;
	if(humi<40)	fan_setSpeed(0);
	else if(humi<50 &&humi>=40)	fan_setSpeed(300);
	else if(humi<60 &&humi>=50)	fan_setSpeed(600);
	else if(humi< 80&&humi>=60)	fan_setSpeed(900);
	else if(humi>=80)	fan_setSpeed(1000);
}