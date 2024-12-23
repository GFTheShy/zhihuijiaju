#include "main.h"

uint16_t tmp=0,humi=0,light=0;//温度(%RH) 湿度(C°) 光照(Lux)

int main()
{
	led_init();
	drv_Systick_Init(168);
	drv_usart1_init();
	ADC_config();
	drv_tim1_led2pwm_init();
	while(1)
	{
		//更新温湿度
		DHT11_WORK(&tmp, &humi);
		printf("tmp:%d \%RH humi:%d C\r\n",tmp,humi);
		//更新光照
		light=getAdcAveragel(5);
		printf("light:%d Lux\r\n",light);

		//调用检测 自动控制的模块
		led_func(light);
		beep_func(tmp);


		//间隔两秒更新
		drv_Systick_delayms(2000);
	}
	return 0;
}


int fputc(int ch,FILE *fp)
{
	while(USART_GetFlagStatus( USART1,  USART_FLAG_TXE)==RESET);
	USART_SendData(  USART1,  ch);
}

