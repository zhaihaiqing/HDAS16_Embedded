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


ALIGN(RT_ALIGN_SIZE)


static struct rt_thread lan_thread;			/*��̫����������߳̿��ƿ飬���ݷ��������ַ�ʽ,һ��������ACK����ѯ������Ϣ��
																					��һ�����������͵����ݰ�
																				*/
static char lan_thread_stack[512];				//�̶߳�ջ
#define LAN_THREAD_PRIORITY          6		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define LAN_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С


//���������߳�
static struct rt_thread key1_thread;			//�¶Ȳɼ��߳̿��ƿ�
static char key1_thread_stack[512];				//�̶߳�ջ
#define KEY1_THREAD_PRIORITY          10		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define KEY1_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

//���������߳�
static struct rt_thread key2_thread;			//�¶Ȳɼ��߳̿��ƿ�
static char key2_thread_stack[512];				//�̶߳�ջ
#define KEY2_THREAD_PRIORITY          10		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define KEY2_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

//���������߳�
static struct rt_thread key3_thread;			//�¶Ȳɼ��߳̿��ƿ�
static char key3_thread_stack[512];				//�̶߳�ջ
#define KEY3_THREAD_PRIORITY          10		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define KEY3_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

//LED1��ʾ�߳�
static struct rt_thread led1_thread;			//����ʱ�߳̿��ƿ�
static char led1_thread_stack[512];				//�̶߳�ջ
#define LED1_THREAD_PRIORITY          11		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define LED1_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С


int hdas_thread_creat(void)
{		

	rt_thread_init(&lan_thread,					//�߳̿��ƿ�
                   "lan_thread",					//�߳̿��ƿ�����
                   lan_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &lan_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(lan_thread_stack),		//�߳�ջ��С
                   LAN_THREAD_PRIORITY, 			//�߳����ȼ�
									 LAN_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
									 
	rt_thread_init(&key1_thread,					//�߳̿��ƿ�
                   "key1_thread",					//�߳̿��ƿ�����
                   key1_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &key1_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(key1_thread_stack),		//�߳�ջ��С
                   KEY1_THREAD_PRIORITY, 			//�߳����ȼ�
									 KEY1_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
									 
	rt_thread_init(&key2_thread,					//�߳̿��ƿ�
                   "key2_thread",					//�߳̿��ƿ�����
                   key2_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &key2_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(key2_thread_stack),		//�߳�ջ��С
                   KEY2_THREAD_PRIORITY, 			//�߳����ȼ�
									 KEY2_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С							 
									 
	rt_thread_init(&key3_thread,					//�߳̿��ƿ�
                   "key3_thread",					//�߳̿��ƿ�����
                   key3_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &key3_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(key3_thread_stack),		//�߳�ջ��С
                   KEY3_THREAD_PRIORITY, 			//�߳����ȼ�
									 KEY3_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С									 
									 
									 
	rt_thread_init(&led1_thread,					//�߳̿��ƿ�
                   "led1_thread",					//�߳̿��ƿ�����
                   led1_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &led1_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(led1_thread_stack),		//�߳�ջ��С
                   LED1_THREAD_PRIORITY, 			//�߳����ȼ�
									 LED1_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С								 
									 
									 
	rt_thread_startup(&key1_thread); 
	rt_thread_startup(&key2_thread); 
	rt_thread_startup(&key3_thread); 
	rt_thread_startup(&led1_thread);
	rt_thread_startup(&lan_thread);									 
									 
	
	return 0;
	
}



uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t ret=0;
	
	HAL_SPI_TransmitReceive(&hspi1,&TxByte,&ret,1,0xffff);
	
	return ret;		//����
}



void lan_thread_entry(void *par)
{
	
		uint8_t rx[3]={0};
		uint8_t tx[3]={0};
		
	
	rt_thread_mdelay(200);
	
	
		W_RSTN_L();
		rt_thread_mdelay(50);
		W_RSTN_H();
		rt_thread_mdelay(3000);
	
	
	while(1)
	{
		
		W_CSN_L();
		
		rx[0]=0;
		rx[1]=0;
		rx[2]=0;
		
		tx[0]=0x00;
		tx[1]=0x03;
		tx[2]=0x01;
		
//		HAL_SPI_TransmitReceive(&hspi1,&tx[0],&rx[0],1,0xffff);
//		HAL_SPI_TransmitReceive(&hspi1,&tx[1],&rx[1],1,0xffff);
//		HAL_SPI_TransmitReceive(&hspi1,&tx[2],&rx[2],1,0xffff);
		
		HAL_SPI_Transmit(&hspi1,tx,3,0xffff);
		HAL_SPI_Receive(&hspi1,rx,3,0xffff);
		
		//HAL_SPI_Receive_IT(&hspi1,rx,2);
		//HAL_SPI_TransmitReceive_IT(&hspi1,tx,rx,2);
		
		
		
		log_info("rx[0]:0x%x rx[1]:0x%x rx[2]:0x%x\r\n",rx[0],rx[1],rx[2]);
		
		W_CSN_H();
		
		
		rt_thread_mdelay(500);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		
		rt_thread_mdelay(500);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	}
}





void key1_thread_entry(void *par)
{
	while(1)
	{
		if(!READ_KEY1())
		{
			while(!READ_KEY1())
			{
				rt_thread_mdelay(10);
			}
			log_info("Press Key1\r\n");
		}
		else
		{
			rt_thread_mdelay(20);
		}
		
	}
}

void key2_thread_entry(void *par)
{
	while(1)
	{
		if(!READ_KEY2())
		{
			while(!READ_KEY2())
			{
				rt_thread_mdelay(10);
			}
			log_info("Press Key2\r\n");
		}
		else
		{
			rt_thread_mdelay(20);
		}
		
	}
}


void key3_thread_entry(void *par)
{
	while(1)
	{
		if(!READ_KEY3())
		{
			while(!READ_KEY3())
			{
				rt_thread_mdelay(10);
			}
			log_info("Press Key3\r\n");
		}
		else
		{
			rt_thread_mdelay(20);
		}
		
	}
}

void led1_thread_entry(void *par)
{
	while(1)
	{
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		
//		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		
		rt_thread_mdelay(50);
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		
//		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		
		rt_thread_mdelay(50);
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
