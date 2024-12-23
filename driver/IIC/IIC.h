#ifndef _IIC_H
#define _IIC_H
#include "stm32f4xx.h"
#include "drv_delay.h"

#define IIC_PORT	GPIOB	//iic端口
#define IIC_SDA_PIN	GPIO_Pin_7	//SDA
#define IIC_SCL_PIN	GPIO_Pin_6	//SCL

#define SDA_Write(N)	(N?GPIO_SetBits(IIC_PORT, IIC_SDA_PIN):GPIO_ResetBits(IIC_PORT, IIC_SDA_PIN))
#define SCL_Write(N)	(N?GPIO_SetBits(IIC_PORT, IIC_SCL_PIN):GPIO_ResetBits(IIC_PORT, IIC_SCL_PIN))

//IIC初始化
void IIC_Init(void);
//设置为输入模式
void SDA_MODE_INPUT(void);
//设置为输出模式
void SDA_MODE_OUTPUT(void);
//起始信号
void Start_Signal(void);
//停止信号
void Stop_Signal(void);
//从机应答
u8 get_Ask(void);
//主机应答
void Send_Ask(u8 ask);
//IIC写数据
void IIC_WriteByte(u8 data);
//IIC 读一个字节
u8 IIC_ReadByte(void);

//AT24C02	写操作
void AT24C02_WriteData(u8 addr,u8 data);
//AT24C02	读操作
u8 AT24C02_ReadData(u8 addr);
#endif

