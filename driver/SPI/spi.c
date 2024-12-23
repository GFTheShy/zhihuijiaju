#include "spi.h"
#if 0
/*
�ļ���SPIͨ��Э�� --W25Q128FLAH�洢оƬ  
������ʽ����׼�ⷽʽ
оƬ��stm32f407vet6
*/
#include "main.h"
//SPI1 ��ʼ������
void SPI1_Init(void)
{
    //1.ʹ��ʱ�� GPIOB(AHB1)  SPI1(APB2)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    //2.GPIO���ò���ʼ��
    //SCK    MOSI  MISO
    GPIO_InitTypeDef gpio_struct;
    gpio_struct.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
    gpio_struct.GPIO_OType=GPIO_OType_PP;
    gpio_struct.GPIO_Pin=SPI1_MISO_PIN|SPI1_MOSI_PIN|SPI1_SCK_PIN;
    gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;//����
    gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_struct);
    //CS����
    gpio_struct.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
    gpio_struct.GPIO_OType=GPIO_OType_PP;
    gpio_struct.GPIO_Pin=WQ25_CS_PIN;
    GPIO_Init(GPIOB,&gpio_struct);
    //ע�� �Ƚ�CS���ڲ�����״̬  ����
    GPIO_SetBits(GPIOB,WQ25_CS_PIN);
    //3.���Ÿ���
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
    //4.spi�ṹ��
    SPI_InitTypeDef spi_struct;
    spi_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
    //ʱ�Ӽ��� �� ʱ����λ��������ģʽ  ��ӻ�һ��  ѡ��Ϊģʽ3  CPOL :1 �ߵ�ƽ   CPHA:1 ���ڶ������زɼ�����
    spi_struct.SPI_CPHA=SPI_CPHA_2Edge;
    spi_struct.SPI_CPOL=SPI_CPOL_High;
    spi_struct.SPI_CRCPolynomial=7;//����ʡ��  7 �ο���У��ֵ
    spi_struct.SPI_DataSize=SPI_DataSize_8b;
    spi_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_struct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB
    spi_struct.SPI_Mode=SPI_Mode_Master;//����ģʽ
    spi_struct.SPI_NSS=SPI_NSS_Soft;//�������
    SPI_Init(SPI1,&spi_struct);
    //5.spiʹ��
    SPI_Cmd(SPI1,ENABLE);
    
}

//SPI��һ�����νṹ����  ����һ���ֽڵ�ͬʱ Ҳ�����һ���ֽڵ�����
u8 SPI_READ_WRITE_DATA(u8 data)
{
    //����
    while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//��Ϊ��һֱѭ�� ���ݷ���Ϊֹ
    SPI_I2S_SendData(SPI1,data);
    //����
    while(SPI_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);//Ϊ�յ�ʱ���һֱ�ȴ�
    return SPI_I2S_ReceiveData(SPI1);
    
}

//��ȡFLASH����оƬ��ID
u16 read_w25_ID(void)
{
    u16 ID=0;
    //1.spi��ʼ�� ����
    SPI1_Init();
    //2.����CS����  ����ͨ��  ѡ��ͨ��
    GPIO_ResetBits(GPIOB,WQ25_CS_PIN);
    //3.����ָ��  90h
    SPI_READ_WRITE_DATA(0x90);
    //4.����24λ ��ַ 0x000000  3���ֽ�
    SPI_READ_WRITE_DATA(0x00);
    SPI_READ_WRITE_DATA(0x00);
    SPI_READ_WRITE_DATA(0x00);
    //5.���豸ID  ���յ�ַ 00
    ID=SPI_READ_WRITE_DATA(0x00)<<8;//��һ���ֽ�
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
