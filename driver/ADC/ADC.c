#include "ADC.h"
//PA0 光敏电阻
void ADC_config(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, & GPIO_InitStruct);

	ADC_DeInit();

	ADC_CommonInitTypeDef  ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(& ADC_CommonInitStruct);

	ADC_InitTypeDef  ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion=1;
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1, & ADC_InitStruct);

	ADC_Cmd(ADC1, ENABLE);

	
}

u16 Get_ADC_Value()
{
	ADC_RegularChannelConfig( ADC1,  ADC_Channel_0, 1,  ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv( ADC1);
	while(!ADC_GetFlagStatus( ADC1,  ADC_FLAG_EOC));

	return ADC_GetConversionValue(ADC1);
}

unsigned short int getAdcAveragel(unsigned char times)
{
	unsigned short int temp_val=0;
	unsigned char t;
	for(t=0;t<times;t++)
		{
			temp_val+=Get_ADC_Value();
			drv_Systick_delayms(5);
		}
	return temp_val/times;
}