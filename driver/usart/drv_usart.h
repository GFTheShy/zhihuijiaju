#ifndef _DRV_TIMX_H
#define _DRV_TIMX_H
#include "stm32f4xx.h"

extern uint16_t tmp,humi,light;//ÎÂ¶È(C¡ã) Êª¶È(%RH) ¹âÕÕ(Lux)
void drv_usart1_init(void);
void usart_sendStr(USART_TypeDef * USARTx,u8 *str);

void drv_usart2_init(void);
void wifi_conected(void);
void Send_data_to_client(char *data);
void wifi_func(void);
#endif

