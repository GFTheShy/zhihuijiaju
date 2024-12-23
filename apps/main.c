#include "main.h"

volatile uint16_t tmp=0,humi=0,light=0;//温度(C°) 湿度(%RH) 光照(Lux)
int oled_ui=1;	//三个界面 
int led1_flag=0,beep_flag=0,fan_flag=0;//灯 蜂鸣器 风扇是否开启的标志位 0:关闭 1：开启
int fan_mux=0,beep_mux=0;//标识是否是自动还是手动控制 0：自动 1：手动

u8 key1_num = '*';
u8 key2_num = '*';
u8 key3_num = '*';
//密码
u8 key1_true = '1';
u8 key2_true = '2';
u8 key3_true = '3';
u8 Cnt_Error = 0;//超过3次密码错误报警

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
	drv_tim10_init();//3s自动退出手动控制模式
		
	oled_begin_mv();//开机动画
	drv_tim1_fanpwm_init();//初始化风扇的pwm
	drv_usart2_init();
	wifi_conected();
	while(1)
	{
		//更新温湿度
		DHT11_WORK(&tmp, &humi);

		//更新光照
		light=getAdcAveragel(5);

		//发送数据给客户端
		wifi_func();
				
		//调用检测 自动控制的模块
		led_func(light);
		beep_func(tmp);
		fan_func(humi);
		
		//更新oled界面显示
		oled_func();
		//间隔100ms更新
		drv_Systick_delayms(100);
	}
	return 0;
}


int fputc(int ch,FILE *fp)
{
	while(USART_GetFlagStatus( USART1,  USART_FLAG_TXE)==RESET);
	USART_SendData(  USART1,  ch);
}

