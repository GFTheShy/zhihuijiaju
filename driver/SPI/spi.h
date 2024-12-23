#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f4xx.h"

#define SPI1_SCK_PIN	GPIO_Pin_3
#define SPI1_MISO_PIN	GPIO_Pin_4
#define SPI1_MOSI_PIN	GPIO_Pin_5
#define WQ25_CS_PIN	GPIO_Pin_2
void SPI1_Init(void);
u8 SPI_READ_WRITE_DATA(u8 data);
u16 READ_WQ25_ID(void);
#endif

