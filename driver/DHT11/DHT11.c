#include "DHT11.h"
//PA3
void SET_DHT11_MODE_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, & GPIO_InitStruct);
}

void SET_DHT11_MODE_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, & GPIO_InitStruct);
}
//1为响应成功
int Start_ask(void)
{
	SET_DHT11_MODE_OUT();
	DHT_HIGH;
	DHT_LOW;
	drv_Systick_delayms(20);
	DHT_HIGH;
	drv_Systick_delayus(30);

	SET_DHT11_MODE_IN();
	int t=0;	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t<100)
	{
		drv_Systick_delayus(1);
		t++;
	}
	if(t>=100)
	{
		return 1;
	}

	t=0;	
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)&&t<80)//如果一直维持低电平 就循环 ---》等待变高点平
	{
		t++;
		drv_Systick_delayus(1);
		
	}	//printf("t=%d\r\n",t);
	if(t>=80)
	{
		return 1;
	}
	//等待 变低
	t=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t<80)
	{
		drv_Systick_delayus(1);
		t++;
	}
	//printf("t1=%d\r\n",t1);
	if(t>=80)
	{
		return 1;
	}
	return 0;
}

u8 Read_Bit(void)
{
	SET_DHT11_MODE_IN();
	uint8_t retry=0;
	while(DHT_READ==SET&&retry<100) {retry++;drv_Systick_delayus(1);}
	retry=0;
	while(DHT_READ==RESET&&retry<100) {retry++;drv_Systick_delayus(1);}
	drv_Systick_delayus(50);//40us以后如果读到仍旧是高电平 意味着  这一位数据为1
	if(DHT_READ==SET)
	{
		return 1;
	}
	return 0;
}

u8 Read_byte(void)
{
	u8 data=0;
	int i;
	for(i=7;i>=0;i--)
	{
		data |= Read_Bit()<<i;
	}
	return data;
}

int DHT11_WORK(u8 *tmp,u8 *humi)
{
	u8 buf[5];
	if(Start_ask())		return 1;
	for(int i=0;i<5;i++)
	{
		buf[i]=Read_byte();
	}
	if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
	{
		*humi=buf[0];		
		*tmp=buf[2];
	}
	SET_DHT11_MODE_OUT();
		
	return 0;
}