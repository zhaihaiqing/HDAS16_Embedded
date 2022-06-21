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
	RESET_1(); /* �������ӳ١� RESET��λ�ߵ�ƽ���������С50ns�� */
	RESET_1();
	RESET_1();

	RESET_0(); /* �˳���λ״̬ */
}


uint16_t AD_A_dat[8]={0};
uint16_t AD_B_dat[8]={0};

uint8_t AD_flag=0;

void AD7606_A_ISR(void)
{
	
	
	AD_A_dat[0] = READ_AD7606_A_RESULT();	/* ����1·���� */
	AD_A_dat[1] = READ_AD7606_A_RESULT();	/* ����2·���� */
	AD_A_dat[2] = READ_AD7606_A_RESULT();	/* ����3·���� */
	AD_A_dat[3] = READ_AD7606_A_RESULT();	/* ����4·���� */
	AD_A_dat[4] = READ_AD7606_A_RESULT();	/* ����5·���� */
	AD_A_dat[5] = READ_AD7606_A_RESULT();	/* ����6·���� */
	AD_A_dat[6] = READ_AD7606_A_RESULT();	/* ����7·���� */
	AD_A_dat[7] = READ_AD7606_A_RESULT();	/* ����8·���� */
	
	AD_flag |= 0x01;
	
}

void AD7606_B_ISR(void)
{
	
	
	AD_B_dat[0] = READ_AD7606_A_RESULT();	/* ����1·���� */
	AD_B_dat[1] = READ_AD7606_A_RESULT();	/* ����2·���� */
	AD_B_dat[2] = READ_AD7606_A_RESULT();	/* ����3·���� */
	AD_B_dat[3] = READ_AD7606_A_RESULT();	/* ����4·���� */
	AD_B_dat[4] = READ_AD7606_A_RESULT();	/* ����5·���� */
	AD_B_dat[5] = READ_AD7606_A_RESULT();	/* ����6·���� */
	AD_B_dat[6] = READ_AD7606_A_RESULT();	/* ����7·���� */
	AD_B_dat[7] = READ_AD7606_A_RESULT();	/* ����8·���� */
	
	AD_flag |= 0x10;
	
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == AD_BUSY1_Pin)
	{
		AD7606_A_ISR();
	}
	else if(GPIO_Pin == AD_BUSY2_Pin)
	{
		AD7606_B_ISR();
	}
}




















/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/