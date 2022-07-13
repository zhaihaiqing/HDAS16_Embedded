/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMC_AD7606_H__
#define __FMC_AD7606_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* 过采样倍率 */
typedef enum
{
	AD_OS_NO = 0,
	AD_OS_X2 = 1,
	AD_OS_X4 = 2,
	AD_OS_X8 = 3,
	AD_OS_X16 = 4,
	AD_OS_X32 = 5,
	AD_OS_X64 = 6
}AD7606_OS_E;




#define AD_BUSY1_Pin GPIO_PIN_4
#define AD_BUSY1_GPIO_Port GPIOE
#define AD_BUSY1_EXTI_IRQn EXTI4_IRQn
#define AD_BUSY2_Pin GPIO_PIN_6
#define AD_BUSY2_GPIO_Port GPIOB
#define AD_BUSY2_EXTI_IRQn EXTI9_5_IRQn
#define AD_FRST1_Pin GPIO_PIN_13
#define AD_FRST1_GPIO_Port GPIOC
#define AD_FRST2_Pin GPIO_PIN_4
#define AD_FRST2_GPIO_Port GPIOK
#define AD_OS0_Pin GPIO_PIN_8
#define AD_OS0_GPIO_Port GPIOI
#define AD_OS1_Pin GPIO_PIN_3
#define AD_OS1_GPIO_Port GPIOE
#define AD_OS2_Pin GPIO_PIN_9
#define AD_OS2_GPIO_Port GPIOB
#define AD_RST_Pin GPIO_PIN_6
#define AD_RST_GPIO_Port GPIOE




#define OS0_1() HAL_GPIO_WritePin(AD_OS0_GPIO_Port, AD_OS0_Pin, GPIO_PIN_SET)
#define OS0_0() HAL_GPIO_WritePin(AD_OS0_GPIO_Port, AD_OS0_Pin, GPIO_PIN_RESET)

#define OS1_1() HAL_GPIO_WritePin(AD_OS1_GPIO_Port, AD_OS1_Pin, GPIO_PIN_SET)
#define OS1_0() HAL_GPIO_WritePin(AD_OS1_GPIO_Port, AD_OS1_Pin, GPIO_PIN_RESET)

#define OS2_1() HAL_GPIO_WritePin(AD_OS2_GPIO_Port, AD_OS2_Pin, GPIO_PIN_SET)
#define OS2_0() HAL_GPIO_WritePin(AD_OS2_GPIO_Port, AD_OS2_Pin, GPIO_PIN_RESET)




/* AD7606复位口线, 在扩展的74HC574上 */
#define RESET_1() HAL_GPIO_WritePin(AD_RST_GPIO_Port, AD_RST_Pin, GPIO_PIN_SET)
#define RESET_0() HAL_GPIO_WritePin(AD_RST_GPIO_Port, AD_RST_Pin, GPIO_PIN_RESET)



extern uint16_t AD_dat[960];
extern uint16_t AD_buff[964];
extern uint16_t AD_A_dat[8];
extern uint16_t AD_B_dat[8];
extern uint8_t AD_flag;


void bsp_InitAD7606(void);
void AD7606_SetOS(uint8_t _ucOS);
void AD7606_Reset(void);

void AD7606_A_ISR(void);
void AD7606_B_ISR(void);



#endif /* __FMC_AD7606_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
