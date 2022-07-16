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


//FMC_NE1�׵�ַ��0x6000 0000
//FMC_NE2�׵�ַ��0x6400 0000


#define READ_AD7606_A_RESULT()	*(__IO uint16_t *)0x60003000	//��ӦNE1
#define READ_AD7606_B_RESULT()	*(__IO uint16_t *)0x64003000	//��ӦNE2








/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAD7606
*	����˵��: ���������ⲿSRAM��GPIO��FSMC
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitAD7606(void)
{
	

	AD7606_SetOS(AD_OS_NO);		/* �޹����� */
	
	AD7606_Reset();				/* ��λ */
	
	
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_SetOS
*	����˵��: ����AD7606�����˲�����Ҳ�����ù��������ʡ�
*			  ͨ������ AD7606_OS0��OS1��OS2���ߵĵ�ƽ���״̬�������������ʡ�
*			  ����ADת��֮��AD7606�ڲ��Զ�ʵ��ʣ�������Ĳɼ���Ȼ����ƽ��ֵ�����
*
*			  ����������Խ�ߣ�ת��ʱ��Խ����
*			  0���޹�����ʱ��ADת��ʱ�� = 3.45us - 4.15us
*			  1��2��������ʱ = 7.87us - 9.1us
*			  2��4��������ʱ = 16.05us - 18.8us
*			  3��8��������ʱ = 33us - 39us
*			  4��16��������ʱ = 66us - 78us
*			  5��32��������ʱ = 133us - 158us
*			  6��64��������ʱ = 257us - 315us
*
*	��    ��: _ucOS : ����������, 0 - 6
*	�� �� ֵ: ��
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
* �� �� �� : AD7606_Reset
* ����˵��  : Ӳ����λAD7606����λ֮��ָ�����������״̬��
* ��    ��: ��
* �� �� ֵ : ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	RESET_0(); /* �˳���λ״̬ */

	RESET_1(); /* ���븴λ״̬ */
	RESET_1(); /* �������ӳ١� RESET��λ�ߵ�ƽ��������С50ns�� */
	RESET_1();
	RESET_1();

	RESET_0(); /* �˳���λ״̬ */
}



uint16_t AD_dat[960]={0};
uint16_t AD_buff[964]={0};

//uint16_t AD_dat[16]={0};
uint16_t AD_A_dat[8]={0};
uint16_t AD_B_dat[8]={0};

uint8_t AD_flag=0;

uint8_t AD_S_count=0;

//��ƬADC��ת���ź�Ϊͬһ����������һ���ж�ȥ��16·���ݣ�Ӧ���ǿ��е�

void AD7606_A_ISR(void)
{
	//memset(AD_dat,0,16);
	
	AD_dat[AD_S_count*16+0] = READ_AD7606_A_RESULT();	/* ����1·���� */
	AD_dat[AD_S_count*16+1] = READ_AD7606_A_RESULT();	/* ����2·���� */
	AD_dat[AD_S_count*16+2] = READ_AD7606_A_RESULT();	/* ����3·���� */
	AD_dat[AD_S_count*16+3] = READ_AD7606_A_RESULT();	/* ����4·���� */
	AD_dat[AD_S_count*16+4] = READ_AD7606_A_RESULT();	/* ����5·���� */
	AD_dat[AD_S_count*16+5] = READ_AD7606_A_RESULT();	/* ����6·���� */
	AD_dat[AD_S_count*16+6] = READ_AD7606_A_RESULT();	/* ����7·���� */
	AD_dat[AD_S_count*16+7] = READ_AD7606_A_RESULT();	/* ����8·���� */
	
	AD_dat[AD_S_count*16+8] = READ_AD7606_B_RESULT();	/* ����1·���� */
	AD_dat[AD_S_count*16+9] = READ_AD7606_B_RESULT();	/* ����2·���� */
	AD_dat[AD_S_count*16+10] = READ_AD7606_B_RESULT();	/* ����3·���� */
	AD_dat[AD_S_count*16+11] = READ_AD7606_B_RESULT();	/* ����4·���� */
	AD_dat[AD_S_count*16+12] = READ_AD7606_B_RESULT();	/* ����5·���� */
	AD_dat[AD_S_count*16+13] = READ_AD7606_B_RESULT();	/* ����6·���� */
	AD_dat[AD_S_count*16+14] = READ_AD7606_B_RESULT();	/* ����7·���� */
	AD_dat[AD_S_count*16+15] = READ_AD7606_B_RESULT();	/* ����8·���� */
	
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
////	AD_dat[8] = READ_AD7606_B_RESULT();	/* ����1·���� */
////	AD_dat[9] = READ_AD7606_B_RESULT();	/* ����2·���� */
////	AD_dat[10] = READ_AD7606_B_RESULT();	/* ����3·���� */
////	AD_dat[11] = READ_AD7606_B_RESULT();	/* ����4·���� */
////	AD_dat[12] = READ_AD7606_B_RESULT();	/* ����5·���� */
////	AD_dat[13] = READ_AD7606_B_RESULT();	/* ����6·���� */
////	AD_dat[14] = READ_AD7606_B_RESULT();	/* ����7·���� */
////	AD_dat[15] = READ_AD7606_B_RESULT();	/* ����8·���� */
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
