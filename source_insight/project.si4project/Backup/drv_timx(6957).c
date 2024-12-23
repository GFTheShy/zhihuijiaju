#include "drv_timx.h"
//配置定时器6 500ms翻转一次led3 表示系统在运行 闪烁
void drv_tim6_init(void)
{
//配置定时器
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=5000-1;//500ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=8400-1;//10次一ms
	TIM_TimeBaseInit(TIM6, & TIM_TimeBaseInitStruct);
//配置定时器中断源
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
//配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(& NVIC_InitStruct);
//使能定时器
	TIM_Cmd( TIM6, ENABLE);
}

//key1消抖
void drv_tim9_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM9, ENABLE);
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=200-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=16800-1;
	TIM_TimeBaseInit(TIM9, & TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM9,  TIM_IT_Update, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd( TIM9, ENABLE);
}
//pe 9
void drv_tim1_led2pwm_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	//开时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1,ENABLE);

	//配置GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//映射
	GPIO_PinAFConfig( GPIOE,  GPIO_PinSource9,  GPIO_AF_TIM1);

	//配置定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=400;
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStruct);

	//配置定时器通道
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse=1000;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init( TIM1, &TIM_OCInitStruct);

	//使能定时器的预装载寄存器
	TIM_OC1PreloadConfig(TIM1,  TIM_OCPreload_Enable);

	//使能自动重载寄存器的arpe位
	TIM_ARRPreloadConfig( TIM1, ENABLE);

	//使能定时器
	TIM_Cmd( TIM1,ENABLE);

	//控制输出模式使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
//fan tim1 ch3n pb15
void drv_tim1_fanpwm_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	//开时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1,ENABLE);

	//配置GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//映射
	GPIO_PinAFConfig( GPIOB,  GPIO_PinSource15,  GPIO_AF_TIM1);

	//配置定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=400;
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStruct);

	//配置定时器通道
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_Pulse=500;
	TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OC1Init( TIM1, &TIM_OCInitStruct);

	//使能定时器的预装载寄存器
	TIM_OC1PreloadConfig(TIM1,  TIM_OCPreload_Enable);

	//使能自动重载寄存器的arpe位
	TIM_ARRPreloadConfig( TIM1, ENABLE);

	//使能定时器
	TIM_Cmd( TIM1,ENABLE);

	//控制输出模式使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
