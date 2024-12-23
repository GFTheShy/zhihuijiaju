#include "drv_key.h"
//按键1：pe4 按键二：pe5 按键三：pe6 按键四：pc13
#if 0
#define RCC_BASE1  (0x40023800)  //RCC
#define RCC_AHB1_ADDR (volatile unsigned int *)(RCC_BASE1+0x30)  //AHB1

volatile unsigned int *ahb1_addr1;
volatile unsigned int *gpio_mode1;
volatile unsigned int *gpio_pupd;
volatile unsigned const int *gpio_read;

volatile unsigned int *gpio_mode2;
volatile unsigned int *gpio_pupd2;
volatile unsigned const int *gpio_read2;


#define GPIOE_BASE1  (0X40021000)//GPIOE 
#define GPIOE_MODE (volatile unsigned int *)(GPIOE_BASE1+0x00)
#define GPIOE_PUPD (volatile unsigned int *)(GPIOE_BASE1+0x0C)
#define GPIOE_IDR (volatile unsigned int *)(GPIOE_BASE1+0x10)

#endif

void key_init(void)
{
	//初始化led灯和蜂鸣器
	led_init();
	beep_init();

#if 0

	//开时钟
	 ahb1_addr1=RCC_AHB1_ADDR;
    *ahb1_addr1&=~(0x01<<4);
    *ahb1_addr1 |= (0x01<<4);

	*ahb1_addr1&=~(0x01<<2);
    *ahb1_addr1 |= (0x01<<2);
	//输入模式
	gpio_mode1=GPIOE_MODE;
    *gpio_mode1&=~(0x03<<2*4);
    *gpio_mode1&=~(0x03<<2*5);
	*gpio_mode1&=~(0x03<<2*6);

	gpio_mode2=(volatile unsigned int *)(0x40020800+0x00);
	*gpio_mode2&=~(0x03<<2*13);
	//上拉
	gpio_pupd=GPIOE_PUPD;
	*gpio_pupd&=~(0x03<<2*4);
	*gpio_pupd|=(0x01<<2*4);
	*gpio_pupd&=~(0x03<<2*5);
	*gpio_pupd|=(0x01<<2*5);
	*gpio_pupd&=~(0x03<<2*6);
	*gpio_pupd|=(0x01<<2*6);

	gpio_pupd2=(volatile unsigned int *)(0x40020800+0x0C);
	*gpio_pupd2&=~(0x03<<2*13);
	*gpio_pupd2|=(0x01<<2*13);
	//输入寄存器的值
	gpio_read=GPIOE_IDR;
	gpio_read2=(volatile unsigned int *)(0x40020800+0x10);
#else
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

void key_func(void)
{
#if 0
	if(!(*gpio_read&(0x01<<4)))
	{
		drv_delay_ms(5);
		if(!(*gpio_read&(0x01<<4)))
		{
			led_openAll();
		}
	}
	else if(!(*gpio_read&(0x01<<5)))
	{
		drv_delay_ms(5);
		if(!(*gpio_read&(0x01<<5)))
		{
			led_closeAll();
		}
	}
	else if(!(*gpio_read&(0x01<<6)))
	{
		drv_delay_ms(5);
		if(!(*gpio_read&(0x01<<6)))
		{
			beep_open();
		}
	}
	else if(!(*gpio_read2&(0x01<<13)))
	{
		drv_delay_ms(5);
		if(!(*gpio_read&(0x01<<13)))
		{
			beep_close();
		}
	}
#else

	if(key1_flag)
	{
		drv_Systick_delayms(20);
		if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_4)==Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_4)==Bit_RESET);
			led_openAll();
			
		}
		key1_flag=0;
	}
	else if(key2_flag)
	{
		drv_Systick_delayms(20);
		if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_5)==Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_5)==Bit_RESET);
			led_closeAll();
			
		}
		key2_flag=0;
	}
	else if(key3_flag)
	{
		drv_Systick_delayms(20);
		if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_6)==Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_6)==Bit_RESET);
			beep_open();
		}
		key3_flag=0;
	}
	else if(key4_flag)
	{
		drv_Systick_delayms(20);
		if(GPIO_ReadInputDataBit(GPIOC,  GPIO_Pin_13)==Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOC,  GPIO_Pin_13)==Bit_RESET);
			beep_close();
		}
		key4_flag=0;
	}


#endif
}

