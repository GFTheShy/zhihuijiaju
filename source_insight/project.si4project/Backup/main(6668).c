#include "main.h"

uint16_t tmp=0,humi=0,light=0;//温度(C°) 湿度(%RH) 光照(Lux)
int oled_ui=2;	//三个界面 
int led1_flag=0,beep_flag=0,fan_flag=0;//灯 蜂鸣器 风扇是否开启的标志位
int main()
{
	led_init();
	beep_init();
	drv_Systick_Init(168);
	drv_usart1_init();//串口1
	OLED_Init();
	ADC_config();//光照
	drv_tim1_led2pwm_init();//led2的pwm
	drv_tim6_init();//led3的闪烁
	
	exti_init();//按键
	key_init();
	drv_tim9_init();//按键消抖

	oled_begin_mv();//开机动画
	while(1)
	{
		//更新温湿度
		DHT11_WORK(&tmp, &humi);
		printf("tmp:%d \%RH humi:%d C\r\n",tmp,humi);
		//更新光照
		light=getAdcAveragel(5);
		printf("light:%d Lux\r\n",light);
		//更新oled界面显示
		oled_func();
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

