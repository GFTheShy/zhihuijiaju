#include "drv_usart.h"

void drv_usart1_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig( GPIOA,  GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig( GPIOA,  GPIO_PinSource10,  GPIO_AF_USART1);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, & GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(& NVIC_InitStruct);

	USART_ITConfig( USART1,  USART_IT_RXNE, ENABLE);
		
	USART_Cmd(USART1, ENABLE);
}
void drv_usart2_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig( GPIOD,  GPIO_PinSource5,  GPIO_AF_USART2);
	GPIO_PinAFConfig( GPIOD,  GPIO_PinSource6,  GPIO_AF_USART2);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, & GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(& NVIC_InitStruct);

	USART_ITConfig( USART2,  USART_IT_RXNE, ENABLE);
		//开启空闲接收中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART2, ENABLE);
}
void usart_sendStr(USART_TypeDef * USARTx,u8 *str)
{
	for(;*str;str++)
		{
			USART_SendData( USARTx,  *str);
			while(USART_GetFlagStatus( USARTx,  USART_FLAG_TXE)==RESET);
		}
}

//服务器
void wifi_conected(void)
{
	oled_wait();
	drv_Systick_delayms(1000);
	usart_sendStr(USART2,"AT+RST\r\n");
	drv_Systick_delayms(500);
	usart_sendStr(USART2,"AT\r\n");
	drv_Systick_delayms(500);
	usart_sendStr(USART2,"AT+CWMODE=2\r\n");
	drv_Systick_delayms(500);
//	drv_USART_SendStr(USART2,"AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"));
	usart_sendStr(USART2,"AT+CWSAP=\"lcgggg\",\"123456789\",5,4\r\n");
	drv_Systick_delayms(1000);
//	drv_USART_SendStr(USART2,"AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n",strlen("AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n"));
	usart_sendStr(USART2,"AT+CIPAP=\"192.168.43.1\"\r\n");
	drv_Systick_delayms(5000);
	usart_sendStr(USART2,"AT+CIFSR\r\n");
	drv_Systick_delayms(1000);
	usart_sendStr(USART2,"AT+CIPMUX=1\r\n");
	drv_Systick_delayms(1000);
	usart_sendStr(USART2,"AT+CIPSERVER=1,8080\r\n");
	drv_Systick_delayms(1000);
	usart_sendStr(USART2,"AT+CIPAP=\"192.168.43.1\"\r\n");
	drv_Systick_delayms(1000);
	OLED_Clear();
}

void Send_data_to_client(char *data)
{
		char send_cmd[256];
		int len=19+strlen(data);
    // 使用全局的连接ID和数据长度来构建发送命令
    sprintf(send_cmd, "AT+CIPSEND=%d,%d\r\n%s\r\n", 0,19+strlen(data), data);
		usart_sendStr(USART2,send_cmd);
}
void wifi_func(void)
{
	char temp[256];
	sprintf(temp,"tmp#%d#,humi#%d#,light#%d#",tmp,humi,light);
	Send_data_to_client(temp);
}