/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
extern uint8_t ui;  
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{

}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s).    */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


//_Bool led3_flag;
//void EXTI4_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line4))
//	{
//		led3_flag =! led3_flag;
//	}
//	EXTI_ClearITPendingBit(EXTI_Line4);
//}

/**
  * @}
  */ 

/**
  * @}
  */ 
  //外部中断4

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus( EXTI_Line4)==SET)
	{
		TIM_ClearITPendingBit( TIM9,  TIM_IT_Update);
		TIM_Cmd(TIM9, DISABLE);
		TIM_SetCounter(TIM9, 0);
		TIM_Cmd(TIM9, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
//外部中断5 6
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus( EXTI_Line5)==SET)
	{
	TIM_ClearITPendingBit( TIM9,  TIM_IT_Update);
		TIM_Cmd(TIM9, DISABLE);
		TIM_SetCounter(TIM9, 0);
		TIM_Cmd(TIM9, ENABLE);
		EXTI_ClearITPendingBit( EXTI_Line5);
	}
	else if(EXTI_GetITStatus( EXTI_Line6)==SET)
	{
		TIM_ClearITPendingBit( TIM9,  TIM_IT_Update);
		TIM_Cmd(TIM9, DISABLE);
		TIM_SetCounter(TIM9, 0);
		TIM_Cmd(TIM9, ENABLE);
		EXTI_ClearITPendingBit( EXTI_Line6);
	}
}
//外部中断13
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13)==SET)
	{
		TIM_ClearITPendingBit( TIM9,  TIM_IT_Update);
		TIM_Cmd(TIM9, DISABLE);
		TIM_SetCounter(TIM9, 0);
		TIM_Cmd(TIM9, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
		
}
//定时器6的中断服务函数
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,  TIM_IT_Update)==SET)
	{
		GPIO_ToggleBits(GPIOE,  GPIO_Pin_10);
		TIM_ClearITPendingBit(TIM6,  TIM_IT_Update);
	}
}
extern int oled_ui;	//三个界面 
extern int led1_flag,beep_flag,fan_flag;//灯 蜂鸣器 风扇是否开启的标志位
extern u8 key1_num,key2_num,key3_num,key1_true,key2_true,key3_true,Cnt_Error;
extern int led_mux,beep_mux,fan_mux;//标识是否是自动还是手动控制 0：自动 1：手动


//定时器9 按键消抖
void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,  TIM_IT_Update)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_4)==Bit_RESET)	//key1
		{
			if(oled_ui==1)
				{
					if(key1_num == '*')
						key1_num = 48 - 1;
					if(++key1_num >= 58)
						key1_num = 48;
				}
			else if(oled_ui==2)
				{
					
				}
			else if(oled_ui==3)
				{
					if(fan_flag)
						{
							fan_setSpeed(0);
							fan_mux=1;
							OLED_Clear();
							oled_func();
						}
					else
						{
							fan_setSpeed(600);	
							fan_mux=0;
							OLED_Clear();
							oled_func();
						}
				}
		}
		else if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_5)==Bit_RESET)	//key2
		{
			if(oled_ui==1)
				{
					if(key2_num == '*')
						key2_num = 48 - 1;
					if(++key2_num >= 58)
						key2_num = 48;
				}
			else if(oled_ui==2)
				{
					
				}
			else if(oled_ui==3)
				{
					if(led1_flag)
						{
							led_close(1);
							led1_flag=0;
							led_mux=1;
							OLED_Clear();
							oled_func();
						}
					else
						{
							led_open(1);
							led1_flag=1;
							led_mux=0;
							OLED_Clear();
							oled_func();
						}
				}
		}
		else if(GPIO_ReadInputDataBit(GPIOE,  GPIO_Pin_6)==Bit_RESET)	//key3
		{
			if(oled_ui==1)
				{
					if(key3_num == '*')
						key3_num = 48 - 1;
					if(++key3_num >= 58)
						key3_num = 48;
				}
			else if(oled_ui==2)
				{
					
				}
			else if(oled_ui==3)
				{
					if(beep_flag)
						{
							beep_open();
							beep_flag=0;
							beep_mux=1;
							OLED_Clear();
							oled_func();
						}
					else
						{
							beep_close();
							beep_flag=1;
							beep_mux=0;
							OLED_Clear();
							oled_func();
						}
				}
		}
		else if(GPIO_ReadInputDataBit(GPIOC,  GPIO_Pin_13)==Bit_RESET)	//key4
		{
			if(oled_ui==1)
				{
					if(key1_num == key1_true && key2_num == key2_true &&key3_num == key3_true)
						{
							oled_ui = 2;
							OLED_Refresh();
							OLED_Clear();
						}
					else
						{
							Cnt_Error++;
							if(Cnt_Error == 3)
							{
								beep_earning();
								Cnt_Error = 0;
							}
							else
							{
								beep_close();
							}
						}
						key1_num = '*';
						key2_num = '*';
						key3_num = '*';
				}
			else if(oled_ui==2)
				{
					oled_ui=3;
					OLED_Clear();
					oled_func();
				}
			else if(oled_ui==3)
				{
					oled_ui=2;
					OLED_Clear();
					oled_func();
				}
		}
		TIM_Cmd(TIM9, DISABLE);
		TIM_ClearITPendingBit(TIM9,  TIM_IT_Update);
	}
}
//usart1
void USART1_IRQHandler(void)
{
	u16 rec_data;
	if(USART_GetITStatus( USART1,  USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit( USART1,  USART_IT_RXNE);
		rec_data=USART_ReceiveData( USART1);
		USART_SendData( USART1, rec_data);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
