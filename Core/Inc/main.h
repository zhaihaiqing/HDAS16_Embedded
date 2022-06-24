/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rtthread.h"

#include "eth.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

#include "mthread.h"
#include "fmc_ad7606.h"

#include "w6100.h"
//#include "loopback/loopback.h"
#include "wizchip_conf.h"
#include "socket.h"
//#include "socket.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

void SystemClock_Config(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WDI_Pin GPIO_PIN_12
#define WDI_GPIO_Port GPIOI

#define APOW_CTRL_Pin GPIO_PIN_10
#define APOW_CTRL_GPIO_Port GPIOF

#define TRIGGER_OUT_Pin GPIO_PIN_5
#define TRIGGER_OUT_GPIO_Port GPIOE

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

#define W_CSN_Pin GPIO_PIN_5
#define W_CSN_GPIO_Port GPIOK
#define W_INT_Pin GPIO_PIN_10
#define W_INT_GPIO_Port GPIOA
#define W_INT_EXTI_IRQn EXTI15_10_IRQn
#define W_RSTN_Pin GPIO_PIN_9
#define W_RSTN_GPIO_Port GPIOA
#define USB_HS_RST_Pin GPIO_PIN_10
#define USB_HS_RST_GPIO_Port GPIOJ
#define LAN_G_RST_Pin GPIO_PIN_13
#define LAN_G_RST_GPIO_Port GPIOD
#define RMII_RST_Pin GPIO_PIN_12
#define RMII_RST_GPIO_Port GPIOD

#define LED0_Pin GPIO_PIN_11
#define LED0_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOH

#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOJ
#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOJ
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOJ


#define	READ_KEY1()			HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define	READ_KEY2()			HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)
#define	READ_KEY3()			HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)


#define W_RSTN_L()	HAL_GPIO_WritePin(W_RSTN_GPIO_Port,W_RSTN_Pin,GPIO_PIN_RESET);
#define W_RSTN_H()	HAL_GPIO_WritePin(W_RSTN_GPIO_Port,W_RSTN_Pin,GPIO_PIN_SET);
#define W_CSN_L()		HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_RESET);
#define W_CSN_H()		HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_SET);

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
