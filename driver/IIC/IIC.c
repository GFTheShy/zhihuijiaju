#include "IIC.h"
#if 0
//IIC初始化
void IIC_init(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin=IIC_SDA_PIN | IIC_SCL_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
}
//设置为输入模式
void SDA_MODE_INPUT(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
}
//设置为输出模式
void SDA_MODE_OUTPUT(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
}
//起始信号
void Start_Signal(void)
{
	SDA_MODE_OUTPUT();
	
	SDA_Write(1);
	SCL_Write(1);
	drv_Systick_delayus(5);

	SDA_Write(0);
	drv_Systick_delayus(5);

	SCL_Write(0);
}
//停止信号
void Stop_Signal(void)
{
	SDA_MODE_OUTPUT();

	SDA_Write(0);
	SCL_Write(0);
	drv_Systick_delayus(5);

	SDA_Write(1);
	drv_Systick_delayus(5);

	SCL_Write(1);
	drv_Systick_delayus(5);
}
//从机应答
u8 get_Ask(void)
{
	SCL_Write(0);
	SDA_MODE_INPUT();

	SCL_Write(1);
	drv_Systick_delayus(5);

	if(GPIO_ReadInputDataBit(IIC_PORT, IIC_SDA_PIN)==RESET)
	{
		SCL_Write(0);
		drv_Systick_delayus(5);
		return 0;
	}
	return 1;
}

//主机应答
void Send_Ask(u8 ask)
{
	SCL_Write(0);
	SDA_MODE_OUTPUT();

	SCL_Write(0);
	drv_Systick_delayus(5);

	if(ask)
	{
		SDA_Write(1);
	}
	else
	{
		SDA_Write(0);
	}

	SCL_Write(1);
	drv_Systick_delayus(5);

	SCL_Write(0);
	drv_Systick_delayus(5);
}
//IIC写数据
void IIC_WriteByte(u8 data)
{
	printf("data:%d\r\n",data);
	
	SCL_Write(0);
	SDA_MODE_OUTPUT();

	SCL_Write(0);
	drv_Systick_delayus(2);
	for(int i=7;i>=0;i--)
	{
		if(data & (0x01<<i))
		{
			SDA_Write(1);
		}
		else
		{
			SDA_Write(0);
		}
		SCL_Write(1);
		drv_Systick_delayus(2);

		SCL_Write(0);
		drv_Systick_delayus(2);
	}
}
//IIC 读一个字节
u8 IIC_ReadByte(void)
{
	u8 data=0;

	SCL_Write(0);
	SDA_MODE_INPUT();

	SCL_Write(0);
	drv_Systick_delayus(2);

	for(int i=7;i>=0;i--)
	{
		SCL_Write(1);
		drv_Systick_delayus(2);

		if(GPIO_ReadInputDataBit(IIC_PORT,  IIC_SDA_PIN)==SET)
		{
			data |= 1<<i;
		}

		SCL_Write(0);
		drv_Systick_delayus(2);
	}
	SCL_Write(1);
	drv_Systick_delayus(2);

	printf("ii_data %d\r\n",data);

	return data;
}

//AT24C02	写操作
void AT24C02_WriteData(u8 addr,u8 data)
{
	
	printf("writ_data:%d\r\n",data);
	printf("writ_addr:%d\r\n",addr);
	Start_Signal();

	IIC_WriteByte(0xa0);

	if(get_Ask())	{printf("IIC_WriteByte_error");return;}

	IIC_WriteByte(addr);

	if(get_Ask())	{printf("addr_error");return;}

	IIC_WriteByte(data);

	if(get_Ask())	{printf("data_error");return;}

	Stop_Signal();
}
//AT24C02	读操作
u8 AT24C02_ReadData(u8 addr)
{
	u8 data=0;

	Start_Signal();

	IIC_WriteByte(0xa0);

	if(get_Ask())	Stop_Signal();

	IIC_WriteByte(addr);

	if(get_Ask())	Stop_Signal();


	Start_Signal();

	IIC_WriteByte(0xa1);

	if(get_Ask())	Stop_Signal();

	data=IIC_ReadByte();
	
	printf("read_data:%d\r\n",data);

	Send_Ask(1);

	Stop_Signal();

	return data;
}

#endif

/*
文件：IIC通信协议 ---EEPROM/AT24C02存储芯片  
开发方式：标准库方式
芯片：stm32f407vet6
*/

//IIC初始化
void IIC_Init(void)
{
	//1.使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.gpio配置  
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//开漏
	gpio_struct.GPIO_Pin=IIC_SDA_PIN|IIC_SCL_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT,&gpio_struct);
}
//设置SDA模式为输入模式
void SDA_MODE_INPUT(void)
{
	//1.使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.gpio配置  
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_IN;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;
	gpio_struct.GPIO_Pin=IIC_SDA_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT,&gpio_struct);
}

//设置SDA模式为输出模式
void SDA_MODE_OUTPUT(void)
{
	//1.使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.gpio配置  
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//开漏
	gpio_struct.GPIO_Pin=IIC_SDA_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(IIC_PORT,&gpio_struct);
}


//起始信号   SCL 为高电平时   SDA先拉低 然后SCL拉低
void Start_Signal(void)
{
	SDA_MODE_OUTPUT();//设置SDA
	//1.空闲状态  SDA  SCL拉高
	SDA_Write(1);
	SCL_Write(1);
	drv_Systick_delayus(5);//？？
	//2.拉低SDA
	SDA_Write(0);
	drv_Systick_delayus(5);
	//3.拉低SCL
	SCL_Write(0);
}
//停止信号
void Stop_Signal(void)
{
	SDA_MODE_OUTPUT();//设置SDA
	//1.空闲状态  SDA  SCL拉低
	SDA_Write(0);
	SCL_Write(0);
//	drv_Systick_delayus(5);//？？
	//2.拉高SCL
	SCL_Write(1);
	drv_Systick_delayus(5);
	//3.拉高SDA
	SDA_Write(1);
}

//从机应答 
u8 get_Ask(void)
{
	//将SCL 拉低  SDA数据无效
	SCL_Write(0);
	//2.设置SDA为输入模式
	SDA_MODE_INPUT();
	//3.拉高SCL
	SCL_Write(1);
	drv_Systick_delayus(5);//延时读数据
	//4.读SDA
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==RESET)
	{
		SCL_Write(0);
		drv_Systick_delayus(5);//？
		return 0;
	}
	return 1;
}
//主机应答
void Send_Ask(u8 ask)
{
	//将SCL 拉低  SDA数据无效
	SCL_Write(0);
	//2.设置输出模式
	SDA_MODE_OUTPUT();
	//3.SCL 拉低
	SCL_Write(0);
	drv_Systick_delayus(5);
	//4.准备数据
	if(ask)
	{
		SDA_Write(1);
	}
	else
	{
		SDA_Write(0);
	}
	//4.SCL拉高发送数据
	SCL_Write(1);
	drv_Systick_delayus(5);
	//5.拉低
	SCL_Write(0);
	drv_Systick_delayus(5);
	
}

//IIC写数据
void IIC_WriteByte(u8 data )
{
	//将SCL 拉低  SDA数据无效
	SCL_Write(0);
	//2.设置输出模式
	SDA_MODE_OUTPUT();
	//3.SCL 拉低
	SCL_Write(0);
	drv_Systick_delayus(2);
	//4.发送数据  8 bit  高位在前
	for(int i=7;i>=0;i--)
	{
		//准备数据  
		if(data & (0x01<<i))
		{
			SDA_Write(1);//写1
		}
		else
		{
			SDA_Write(0);//写0
		}
		//拉高SCL  发送数据
		SCL_Write(1);
		drv_Systick_delayus(2);
		//拉低SCL
		SCL_Write(0);
		drv_Systick_delayus(2);
	}
}
//IIC 读一个字节
u8 IIC_ReadByte()
{
	u8 data=0;
	//将SCL 拉低  SDA数据无效
	SCL_Write(0);
	//2.设置输入模式
	SDA_MODE_INPUT();
	//3.SCL 拉低
	SCL_Write(0);
	drv_Systick_delayus(2);
	//4.读数据
	for(int i=7;i>=0;i--)
	{
		//拉高SCL  发送数据
		SCL_Write(1);
		delay_us(2);
		//读数据
		if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==SET)
		{
			data |= 1<<i;
		}
		//拉低SCL  发送数据
		SCL_Write(0);
		drv_Systick_delayus(2);
	}
	//拉高SCL  发送数据
		SCL_Write(1);
		drv_Systick_delayus(2);
	return data;//返回数据
	
}


//AT24C02  写操作
void AT24C02_WriteData(u8 addr,u8 data)
{
	//1.起始信号
	Start_Signal();
	//2.发送从器件地址  :写 0xa0
	IIC_WriteByte(0xa0);
	//3.等到从机应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 deveci no ask\r\n");
		Stop_Signal();
		return ;
	}
	//4.发送存储地址 字地址
	IIC_WriteByte(addr);
	//5.应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 writewordaddr no ask\r\n");
		Stop_Signal();
		return ;
	}
	//6.写数据
	IIC_WriteByte(data);
	//7.应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 writedata no ask\r\n");
		Stop_Signal();
		return ;
	}
	//8.停止信号
	Stop_Signal();
}


//at24c02读数据
u8  AT24C02_ReadData(u8 addr)
{
	u8 data=0;
	//1.起始信号
	Start_Signal();
	//2.发送从器件地址  :写 0xa0
	IIC_WriteByte(0xa0);
	//3.等到从机应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 deveci no ask\r\n");
		Stop_Signal();
	}
	//4.发送存储地址 字地址
	IIC_WriteByte(addr);
	//5.应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 readwordaddr no ask\r\n");
		Stop_Signal();
	}
	//6.起始信号
		Start_Signal();
	//7.发送从器件地址 读的方式 :0XA1
	IIC_WriteByte(0xa1);
	//8.应答
	if(get_Ask())//1:不应答   0：应答
	{
		printf("at24c02 deveci2 no ask\r\n");
		Stop_Signal();
	}
	//9.读数据
	data=IIC_ReadByte();
	//10 主机 不应答
	Send_Ask(1);
	//11.停止信号
	Stop_Signal();
	return data;
}

