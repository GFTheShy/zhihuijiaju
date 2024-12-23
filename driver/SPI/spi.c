#include "spi.h"
#if 0
/*
文件：SPI通信协议 --W25Q128FLAH存储芯片  
开发方式：标准库方式
芯片：stm32f407vet6
*/
#include "main.h"
//SPI1 初始化配置
void SPI1_Init(void)
{
    //1.使能时钟 GPIOB(AHB1)  SPI1(APB2)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    //2.GPIO配置并初始化
    //SCK    MOSI  MISO
    GPIO_InitTypeDef gpio_struct;
    gpio_struct.GPIO_Mode=GPIO_Mode_AF;//复用模式
    gpio_struct.GPIO_OType=GPIO_OType_PP;
    gpio_struct.GPIO_Pin=SPI1_MISO_PIN|SPI1_MOSI_PIN|SPI1_SCK_PIN;
    gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;//上拉
    gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_struct);
    //CS引脚
    gpio_struct.GPIO_Mode=GPIO_Mode_OUT;//输出模式
    gpio_struct.GPIO_OType=GPIO_OType_PP;
    gpio_struct.GPIO_Pin=WQ25_CS_PIN;
    GPIO_Init(GPIOB,&gpio_struct);
    //注意 先将CS处于不工作状态  拉高
    GPIO_SetBits(GPIOB,WQ25_CS_PIN);
    //3.引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
    //4.spi结构体
    SPI_InitTypeDef spi_struct;
    spi_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
    //时钟极性 和 时钟相位决定工作模式  与从机一致  选择为模式3  CPOL :1 高电平   CPHA:1 ：第二个边沿采集数据
    spi_struct.SPI_CPHA=SPI_CPHA_2Edge;
    spi_struct.SPI_CPOL=SPI_CPOL_High;
    spi_struct.SPI_CRCPolynomial=7;//可以省略  7 参考的校验值
    spi_struct.SPI_DataSize=SPI_DataSize_8b;
    spi_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_struct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB
    spi_struct.SPI_Mode=SPI_Mode_Master;//主机模式
    spi_struct.SPI_NSS=SPI_NSS_Soft;//软件管理
    SPI_Init(SPI1,&spi_struct);
    //5.spi使能
    SPI_Cmd(SPI1,ENABLE);
    
}

//SPI是一个环形结构传输  发送一个字节的同时 也会接收一个字节的数据
u8 SPI_READ_WRITE_DATA(u8 data)
{
    //发送
    while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//不为空一直循环 数据发完为止
    SPI_I2S_SendData(SPI1,data);
    //接收
    while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);//为空的时候就一直等待
    return SPI_I2S_ReceiveData(SPI1);
    
}

//读取FLASH闪存芯片的ID
u16 read_w25_ID(void)
{
    u16 ID=0;
    //1.spi初始化 调用
    SPI1_Init();
    //2.拉低CS引脚  进行通信  选择通信
    GPIO_ResetBits(GPIOB,WQ25_CS_PIN);
    //3.发送指令  90h
    SPI_READ_WRITE_DATA(0x90);
    //4.发送24位 地址 0x000000  3个字节
    SPI_READ_WRITE_DATA(0x00);
    SPI_READ_WRITE_DATA(0x00);
    SPI_READ_WRITE_DATA(0x00);
    //5.读设备ID  发空地址 00
    ID=SPI_READ_WRITE_DATA(0x00)<<8;//第一个字节
    ID|=SPI_READ_WRITE_DATA(0x00);
    GPIO_SetBits(GPIOB,WQ25_CS_PIN);
    return ID;
    
}
#endif

void SPI1_Init(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=SPI1_MISO_PIN|SPI1_MOSI_PIN|SPI1_SCK_PIN;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, & GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=WQ25_CS_PIN;
	GPIO_Init(GPIOB, & GPIO_InitStruct);
	GPIO_SetBits( GPIOB,  GPIO_Pin_2);

	GPIO_PinAFConfig( GPIOB,  GPIO_PinSource3,  GPIO_AF_SPI1);
	GPIO_PinAFConfig( GPIOB,  GPIO_PinSource4,  GPIO_AF_SPI1);
	GPIO_PinAFConfig( GPIOB,  GPIO_PinSource5,  GPIO_AF_SPI1);

	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;
//	SPI_InitStruct.SPI_CRCPolynomial=7;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;
	SPI_Init( SPI1, & SPI_InitStruct);

	SPI_Cmd( SPI1, ENABLE);
}

u8 SPI_READ_WRITE_DATA(u8 data)
{
	while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData( SPI1,  data);
	while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData( SPI1);
}

u16 READ_WQ25_ID(void)
{
	u16 ID=0;
	SPI1_Init();
	
	GPIO_ResetBits( GPIOB,  GPIO_Pin_2);

	SPI_READ_WRITE_DATA(0x90);

	SPI_READ_WRITE_DATA(0x00);
	SPI_READ_WRITE_DATA(0x00);
	SPI_READ_WRITE_DATA(0x00);

	ID = SPI_READ_WRITE_DATA(0x00) << 8;
	ID |= SPI_READ_WRITE_DATA(0x00);

	GPIO_SetBits( GPIOB,  GPIO_Pin_2);

	return ID;
}
