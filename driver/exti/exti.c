#include "exti.h"

void exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource4);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource5);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource6);
	
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOC,  EXTI_PinSource13);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line4 | EXTI_Line5 | EXTI_Line6 |EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);


	//key1
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(& NVIC_InitStruct);

	//key2 3
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(& NVIC_InitStruct);

	//key4
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(& NVIC_InitStruct);
}

