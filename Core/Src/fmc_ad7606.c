/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"


//FMC_NE1首地址：0x6000 0000
//FMC_NE2首地址：0x6400 0000


#define READ_AD7606_A_RESULT()	*(__IO uint16_t *)0x60003000	//对应NE1
#define READ_AD7606_B_RESULT()	*(__IO uint16_t *)0x64003000	//对应NE2








/*
*********************************************************************************************************
*	函 数 名: bsp_InitAD7606
*	功能说明: 配置连接外部SRAM的GPIO和FSMC
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAD7606(void)
{
	

	AD7606_SetOS(AD_OS_NO);		/* 无过采样 */
	
	AD7606_Reset();				/* 复位 */
	
	
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_SetOS
*	功能说明: 配置AD7606数字滤波器，也就设置过采样倍率。
*			  通过设置 AD7606_OS0、OS1、OS2口线的电平组合状态决定过采样倍率。
*			  启动AD转换之后，AD7606内部自动实现剩余样本的采集，然后求平均值输出。
*
*			  过采样倍率越高，转换时间越长。
*			  0、无过采样时，AD转换时间 = 3.45us - 4.15us
*			  1、2倍过采样时 = 7.87us - 9.1us
*			  2、4倍过采样时 = 16.05us - 18.8us
*			  3、8倍过采样时 = 33us - 39us
*			  4、16倍过采样时 = 66us - 78us
*			  5、32倍过采样时 = 133us - 158us
*			  6、64倍过采样时 = 257us - 315us
*
*	形    参: _ucOS : 过采样倍率, 0 - 6
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_SetOS(uint8_t _ucOS)
{
	switch (_ucOS)
	{
	case AD_OS_X2: 
		OS2_0();
		OS1_0();
		OS0_1();
		break;

	case AD_OS_X4: 
		OS2_0();
		OS1_1();
		OS0_0();
		break;

	case AD_OS_X8: 
		OS2_0();
		OS1_1();
		OS0_1();
		break;

	case AD_OS_X16: 
		OS2_1();
		OS1_0();
		OS0_0();
		break;

	case AD_OS_X32: 
		OS2_1();
		OS1_0();
		OS0_1();
		break;

	case AD_OS_X64: 
		OS2_1();
		OS1_1();
		OS0_0();
		break;

	case AD_OS_NO: 
	     default : 
		OS2_0();
		OS1_0();
		OS0_0();
		break;
	}
}


/*
*********************************************************************************************************
* 函 数 名 : AD7606_Reset
* 功能说明  : 硬件复位AD7606，复位之后恢复到正常工作状态。
* 形    参: 无
* 返 回 值 : 无
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	RESET_0(); /* 退出复位状态 */

	RESET_1(); /* 进入复位状态 */
	RESET_1(); /* 仅用于延迟。 RESET复位高电平脉冲宽度最小50ns。 */
	RESET_1();
	RESET_1();

	RESET_0(); /* 退出复位状态 */
}



uint16_t AD_dat[960]={0};
uint16_t AD_buff[964]={0};

//uint16_t AD_dat[16]={0};
uint16_t AD_A_dat[8]={0};
uint16_t AD_B_dat[8]={0};

uint8_t AD_flag=0;

uint8_t AD_S_count=0;

//两片ADC的转换信号为同一个，所以用一个中断去读16路数据，应该是可行的

void AD7606_A_ISR(void)
{
	//memset(AD_dat,0,16);
	
	AD_dat[AD_S_count*16+0] = READ_AD7606_A_RESULT();	/* 读第1路样本 */
	AD_dat[AD_S_count*16+1] = READ_AD7606_A_RESULT();	/* 读第2路样本 */
	AD_dat[AD_S_count*16+2] = READ_AD7606_A_RESULT();	/* 读第3路样本 */
	AD_dat[AD_S_count*16+3] = READ_AD7606_A_RESULT();	/* 读第4路样本 */
	AD_dat[AD_S_count*16+4] = READ_AD7606_A_RESULT();	/* 读第5路样本 */
	AD_dat[AD_S_count*16+5] = READ_AD7606_A_RESULT();	/* 读第6路样本 */
	AD_dat[AD_S_count*16+6] = READ_AD7606_A_RESULT();	/* 读第7路样本 */
	AD_dat[AD_S_count*16+7] = READ_AD7606_A_RESULT();	/* 读第8路样本 */
	
	AD_dat[AD_S_count*16+8] = READ_AD7606_B_RESULT();	/* 读第1路样本 */
	AD_dat[AD_S_count*16+9] = READ_AD7606_B_RESULT();	/* 读第2路样本 */
	AD_dat[AD_S_count*16+10] = READ_AD7606_B_RESULT();	/* 读第3路样本 */
	AD_dat[AD_S_count*16+11] = READ_AD7606_B_RESULT();	/* 读第4路样本 */
	AD_dat[AD_S_count*16+12] = READ_AD7606_B_RESULT();	/* 读第5路样本 */
	AD_dat[AD_S_count*16+13] = READ_AD7606_B_RESULT();	/* 读第6路样本 */
	AD_dat[AD_S_count*16+14] = READ_AD7606_B_RESULT();	/* 读第7路样本 */
	AD_dat[AD_S_count*16+15] = READ_AD7606_B_RESULT();	/* 读第8路样本 */
	
	AD_S_count++;
	if(AD_S_count>=ONE_FRAME_DP_NUM)
	{
		AD_S_count=0;
		AD_flag = 0x01;
		memcpy(&AD_buff[2],AD_dat,ONE_PACKET_DAT_SIZE*ONE_FRAME_DP_NUM);
		//log_info("ADC Sampling...\r\n");
	}
	
	
	//AD_flag = 0x01;
	
}

//void AD7606_B_ISR(void)
//{
//	
//	
////	AD_dat[8] = READ_AD7606_B_RESULT();	/* 读第1路样本 */
////	AD_dat[9] = READ_AD7606_B_RESULT();	/* 读第2路样本 */
////	AD_dat[10] = READ_AD7606_B_RESULT();	/* 读第3路样本 */
////	AD_dat[11] = READ_AD7606_B_RESULT();	/* 读第4路样本 */
////	AD_dat[12] = READ_AD7606_B_RESULT();	/* 读第5路样本 */
////	AD_dat[13] = READ_AD7606_B_RESULT();	/* 读第6路样本 */
////	AD_dat[14] = READ_AD7606_B_RESULT();	/* 读第7路样本 */
////	AD_dat[15] = READ_AD7606_B_RESULT();	/* 读第8路样本 */
//	
//	AD_flag |= 0x10;
//	
//}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == AD_BUSY1_Pin)
	{
		AD7606_A_ISR();
	}
//	else if(GPIO_Pin == AD_BUSY2_Pin)
//	{
//		AD7606_B_ISR();
//	}
	else if(GPIO_Pin == W_INT_Pin)
	{
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
	}
}




















/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
