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
#ifndef __MTHREAD_H__
#define __MTHREAD_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"




int hdas_thread_creat(void);

void lan_thread_entry(void *par);
void key1_thread_entry(void *par);
void key2_thread_entry(void *par);
void key3_thread_entry(void *par);
void led1_thread_entry(void *par);
//void led2_thread_entry(void *par);




#endif /* __mthread_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
