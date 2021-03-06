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


static struct rt_thread lan_thread;			/*以太网命令接收线程控制块，数据发送有两种方式,一种是命令ACK、查询返回信息，
																					另一种是主动发送的数据包
																				*/
static char lan_thread_stack[4096];				//线程堆栈
#define LAN_THREAD_PRIORITY          6		//线程优先级，按键扫描为最高优先级
#define LAN_THREAD_TIMESLICE         100		//线程的时间片大小


//按键处理线程
static struct rt_thread key1_thread;			//温度采集线程控制块
static char key1_thread_stack[512];				//线程堆栈
#define KEY1_THREAD_PRIORITY          10		//线程优先级，按键扫描为最高优先级
#define KEY1_THREAD_TIMESLICE         100		//线程的时间片大小

//按键处理线程
static struct rt_thread key2_thread;			//温度采集线程控制块
static char key2_thread_stack[512];				//线程堆栈
#define KEY2_THREAD_PRIORITY          10		//线程优先级，按键扫描为最高优先级
#define KEY2_THREAD_TIMESLICE         100		//线程的时间片大小

//按键处理线程
static struct rt_thread key3_thread;			//温度采集线程控制块
static char key3_thread_stack[512];				//线程堆栈
#define KEY3_THREAD_PRIORITY          10		//线程优先级，按键扫描为最高优先级
#define KEY3_THREAD_TIMESLICE         100		//线程的时间片大小

//LED0显示线程
static struct rt_thread led0_thread;			//倒计时线程控制块
static char led0_thread_stack[512];				//线程堆栈
#define LED0_THREAD_PRIORITY          11		//线程优先级，按键扫描为最高优先级
#define LED0_THREAD_TIMESLICE         100		//线程的时间片大小

//LED1显示线程
static struct rt_thread led1_thread;			//倒计时线程控制块
static char led1_thread_stack[512];				//线程堆栈
#define LED1_THREAD_PRIORITY          11		//线程优先级，按键扫描为最高优先级
#define LED1_THREAD_TIMESLICE         100		//线程的时间片大小



uint8_t LAN_Link_Flag=0;


int hdas_thread_creat(void)
{		

	rt_thread_init(&lan_thread,					//线程控制块
                   "lan_thread",					//线程控制块名字
                   lan_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &lan_thread_stack[0],			//线程栈起始地址
                   sizeof(lan_thread_stack),		//线程栈大小
                   LAN_THREAD_PRIORITY, 			//线程优先级
									 LAN_THREAD_TIMESLICE);		//线程时间片大小
									 
	rt_thread_init(&key1_thread,					//线程控制块
                   "key1_thread",					//线程控制块名字
                   key1_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &key1_thread_stack[0],			//线程栈起始地址
                   sizeof(key1_thread_stack),		//线程栈大小
                   KEY1_THREAD_PRIORITY, 			//线程优先级
									 KEY1_THREAD_TIMESLICE);		//线程时间片大小
									 
	rt_thread_init(&key2_thread,					//线程控制块
                   "key2_thread",					//线程控制块名字
                   key2_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &key2_thread_stack[0],			//线程栈起始地址
                   sizeof(key2_thread_stack),		//线程栈大小
                   KEY2_THREAD_PRIORITY, 			//线程优先级
									 KEY2_THREAD_TIMESLICE);		//线程时间片大小							 
									 
	rt_thread_init(&key3_thread,					//线程控制块
                   "key3_thread",					//线程控制块名字
                   key3_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &key3_thread_stack[0],			//线程栈起始地址
                   sizeof(key3_thread_stack),		//线程栈大小
                   KEY3_THREAD_PRIORITY, 			//线程优先级
									 KEY3_THREAD_TIMESLICE);		//线程时间片大小									 
									 
									 
	rt_thread_init(&led1_thread,					//线程控制块
                   "led1_thread",					//线程控制块名字
                   led1_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &led1_thread_stack[0],			//线程栈起始地址
                   sizeof(led1_thread_stack),		//线程栈大小
                   LED1_THREAD_PRIORITY, 			//线程优先级
									 LED1_THREAD_TIMESLICE);		//线程时间片大小		
										
	rt_thread_init(&led0_thread,					//线程控制块
                   "led0_thread",					//线程控制块名字
                   led0_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &led0_thread_stack[0],			//线程栈起始地址
                   sizeof(led0_thread_stack),		//线程栈大小
                   LED0_THREAD_PRIORITY, 			//线程优先级
									 LED0_THREAD_TIMESLICE);		//线程时间片大小	
									 
									 
	rt_thread_startup(&key1_thread); 
	rt_thread_startup(&key2_thread); 
	rt_thread_startup(&key3_thread); 
	rt_thread_startup(&led0_thread);
	rt_thread_startup(&led1_thread);
	rt_thread_startup(&lan_thread);									 
									 
	
	return 0;
	
}



uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t ret=0;
	
	HAL_SPI_TransmitReceive(&hspi1,&TxByte,&ret,1,0xffff);
	
	return ret;		//返回
}


uint8_t buff[2048];

void lan_thread_entry(void *par)
{
	uint16 local_port=5000;
	uint8_t rx[3]={0};
	uint8_t tx[3]={0};
	
	uint32_t count=0;
	
	uint8 sn;
		
	
	rt_thread_mdelay(200);
	
	log_info("\r\nHasion Electronics W6100 TEST\r\n\r\n");


	
	/***** 硬重启W6100 *****/
	Reset_W6100();           // 复位 W6100  2020-03-11
	//	/***** W6100的IP信息初始化 *****/
	Set_Network();														// 配置初始化IP等信息并打印 2020-03-11
	setRCR(3);
	setRTR(4000);
	
	LOG_Net_info();
	
	LAN_Link_Flag=0;
	log_info("[%d]wait PHY_LINK...\r\n",count);	
	
	while(wizphy_getphylink()==PHY_LINK_OFF) // 等待物理连接成功后，再进入到socket操作，2020-03-13
	{
		rt_thread_mdelay(1000);
		count++;
		log_info("[%d]wait PHY_LINK...\r\n",count);
	}
	
	LAN_Link_Flag=1;
	log_info("PHY_LINK\r\n");
	
	
	count=0;
	while(1)					//socket操作
	{
		
		
		if(wizphy_getphylink()==PHY_LINK_OFF)// 如果没插网线，关闭使用的socket。
		{
			for(sn=0;sn<7;sn++)
			{
			  close(sn);
			}	
			count++;
			log_info("[%d]PHY_LINK_OFF,Plase check...\r\n",count);
			
		}
		else
		{
			loopback_tcps(0, buff, local_port, AS_IPDUAL);
		}
			
		
		
		
		rt_thread_mdelay(1000);
	}
		
	
	
	
//	while(1)
//	{
//		
//		W_CSN_L();
//		
//		rx[0]=0;
//		rx[1]=0;
//		rx[2]=0;
//		
//		tx[0]=0x00;
//		tx[1]=0x02;
//		tx[2]=0x01;
//		
//		
//		HAL_SPI_Transmit(&hspi1,tx,3,0xffff);
//		
//		if(HAL_SPI_Receive(&hspi1,rx,1,0xffff) != HAL_OK)
//		{
//			log_info("SPI rx error!\r\n");
//		}
//		else
//		{
//			log_info("rx[0]:0x%x rx[1]:0x%x rx[2]:0x%x\r\n",rx[0],rx[1],rx[2]);
//		}
//		
//		W_CSN_H();
//		
//		
//		rt_thread_mdelay(500);
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//		
//		rt_thread_mdelay(500);
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
//	}
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

void led0_thread_entry(void *par)
{
	while(1)
	{
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);		
		rt_thread_mdelay(550);
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);	
		rt_thread_mdelay(50);
	}
}

void led1_thread_entry(void *par)
{
	while(1)
	{
		if(LAN_Link_Flag)
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			rt_thread_mdelay(80);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			rt_thread_mdelay(80);
		}
		else
		{
			rt_thread_mdelay(200);
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
