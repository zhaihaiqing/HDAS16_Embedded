/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/*MAC地址首字节必须为偶数
  如果多块W5100s网络适配板在同一现场工作，请使用不同的MAC地址
*/

/*
socket发送缓存定义为16K，采集400组数据或超时（1S）后发送一次，这样在20K的采样率下，每隔20mS发送一组数据，建议采样率不超过100K


设备采用TCP服务器模式，采集到的数据通过以太网发送出去

*/


	
	
	
	


void CPU_CACHE_Enable(void);
void MPU_Config( void );

void HAL_Get_CPU_RCC_Clock(void);
	
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void Uart_commend_process(void)
{
	//串口指令
		if(Uart_Rx_flag==1)
		{
			
			//进入命令处理函数,首先判断收到的数据是否正确，如果正确，则将数据存入EEPROM，然后继续运行，新参数将在重启后生效。
			// 7E 45 7E 45 01 xx xx xx xx SUM     设置IPV4地址
			// 7E 45 7E 45 02 xx xx xx xx SUM			设置子网掩码
			// 7E 45 7E 45 03 xx xx xx xx SUM			设置网关
			
			//检查帧头、数据长度、检验和正确性
			
			
			if((Uart_Rxbuff[0]==0x7e) && (Uart_Rxbuff[1]==0x45) && (Uart_Rxbuff[2]==0x7e) && (Uart_Rxbuff[3]==0x45) )
			{
				if(Uart_Rxbuff[4] == 0x01)
				{
					NetworkPar.lip[0]=Uart_Rxbuff[5];
					NetworkPar.lip[1]=Uart_Rxbuff[6];
					NetworkPar.lip[2]=Uart_Rxbuff[7];
					NetworkPar.lip[3]=Uart_Rxbuff[8];
					
					log_info("RX NetworkPar.lip dat,Uart_Rxcount:%d\r\n",Uart_Rxcount);
					
				}
				else if(Uart_Rxbuff[4] == 0x02)
				{
					NetworkPar.sub[0]=Uart_Rxbuff[5];
					NetworkPar.sub[1]=Uart_Rxbuff[6];
					NetworkPar.sub[2]=Uart_Rxbuff[7];
					NetworkPar.sub[3]=Uart_Rxbuff[8];
					
					log_info("RX NetworkPar.sub dat,Uart_Rxcount:%d\r\n",Uart_Rxcount);
				}
				else if(Uart_Rxbuff[4] == 0x03)
				{
					NetworkPar.gw[0]=Uart_Rxbuff[5];
					NetworkPar.gw[1]=Uart_Rxbuff[6];
					NetworkPar.gw[2]=Uart_Rxbuff[7];
					NetworkPar.gw[3]=Uart_Rxbuff[8];
					
					log_info("RX NetworkPar.gw dat,Uart_Rxcount:%d\r\n",Uart_Rxcount);
				}
				else if(Uart_Rxbuff[4] == 0x04)
				{
					NetworkPar.port=((uint16_t)Uart_Rxbuff[5]<<8)|Uart_Rxbuff[6];
					
					log_info("RX NetworkPar.port dat,Uart_Rxcount:%d\r\n",Uart_Rxcount);
				}
				else
				{
					log_info("Uart dat error,Uart_Rxcount:%d\r\n",Uart_Rxcount);
				}
				
				
				I2C_EE_BufferWrite(0xa0,0x0,(uint8_t *)&NetworkPar,sizeof(NetworkPar));
				
				//log_info("Uart_Rx:0x%x,Uart_Rxcount:%d\r\n",Uart_Rxbuff[0],Uart_Rxcount);
			}
			else
			{
				log_info("Uart_Rx Error\r\n");
			}
			
			Uart_Rx_flag=0;
			Uart_Rxcount=0;			
		}
}




int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart8,(uint8_t*)&ch,1,10);
	return ch;
}

NetworkPar_type NetworkPar_def={0};	//定义默认网络参数


uint32_t AD_count=0;
uint8_t buff[2048]={0};
uint8_t LAN_Link_Flag=0;
uint8_t test_dat=0;
uint8_t test_buff[128]={0};
uint8_t test_buff2[128]={0};
uint32_t tick_count=0;
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	uint16 local_port=5000;
	uint8_t rx[3]={0};
	uint8_t tx[3]={0};
	
	uint32_t count=0;
	
	uint8 sn;
	
	

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  /* USER CODE BEGIN Init */
	HAL_Init();
	

	SystemClock_Config();
	
  /* USER CODE END Init */

  /* Configure the system clock */
  

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	MX_UART8_Init();
	
  //MX_ETH_Init();
  MX_FMC_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  
  /* USER CODE BEGIN 2 */

	HAL_Get_CPU_RCC_Clock();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	bsp_InitAD7606();
	
	HAL_GPIO_WritePin(APOW_CTRL_GPIO_Port, APOW_CTRL_Pin, GPIO_PIN_SET);
	
	
	log_info("Hardware Init OK!!!\r\n");
	
	WDI_TOGGLE();
	
	HAL_Delay(100);
	
	
	////初始化网络参数
	
	NetworkPar_def.lip[0]=192;
	NetworkPar_def.lip[1]=168;
	NetworkPar_def.lip[2]=1;
	NetworkPar_def.lip[3]=200;
	
	NetworkPar_def.sub[0]=255;
	NetworkPar_def.sub[1]=255;
	NetworkPar_def.sub[2]=255;
	NetworkPar_def.sub[3]=0;
	
	NetworkPar_def.gw[0]=192;
	NetworkPar_def.gw[1]=168;
	NetworkPar_def.gw[2]=1;
	NetworkPar_def.gw[3]=1;
	
	NetworkPar_def.port=5000;
	
	WDI_TOGGLE();
	
	if(I2C_EE_BufferRead(0xa0,0x0,(void *)&NetworkPar,sizeof(NetworkPar)) != HAL_OK)
	{
		memcpy((uint8_t *)&NetworkPar,(uint8_t *)&NetworkPar_def,sizeof(NetworkPar_def));	//给网络复制默认参数
		local_port=NetworkPar_def.port;
	}
	else
	{
		local_port=NetworkPar.port;
	}
	
	if(NetworkPar_def.lip[0]<127)
	{
		memcpy((uint8_t *)&NetworkPar,(uint8_t *)&NetworkPar_def,sizeof(NetworkPar_def));	//给网络复制默认参数
		local_port=NetworkPar_def.port;
	}
	else
	{
		local_port=NetworkPar.port;
	}
	
	//检查网络参数正确性。否则将复制默认参数
	

	
	
	
//#if 1
//	
//	for(uint8_t i=0;i<128;i++)
//		test_buff2[i]=i;
//	
//	while(1)
//	{
//	
//	//I2C_EE_ByteWrite(0xa0,0x00,0x55);
//		
//		
//		I2C_EE_BufferWrite(0xa0,0x0,test_buff2,32);
//	
//		HAL_Delay(100);
//		
//	I2C_EE_BufferRead(0xa0,0x0,test_buff,32);
//		
//		for(uint8_t i=0;i<32;i++)
//		{
//			log_info("test_buff[%d]:0x%x\r\n",i,test_buff[i]);
//		}
//	
//	log_info("I2C test_buff[0]=0x%x\r\n",test_buff[0]);
//	
//		HAL_Delay(5000);
//		
//	}
//	
//#endif
	
	
	

	//hdas_thread_creat();	//执行创建任务函数，开始RTOS
	
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
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			count++;
			
			Uart_commend_process();	//串口指令解析
		
		log_info("[%d]wait PHY_LINK...\r\n",count);
	}
	
	LAN_Link_Flag=1;
	log_info("PHY_LINK\r\n");
	
	
	
	//数据帧的帧头和帧尾格式
	
	AD_buff[0]=0x5555;
	AD_buff[1]=0x5555;
	AD_buff[962]=0xffff;
	AD_buff[963]=0xffff;
	
	
#if 1	
	while(1)//socket操作测试
	{
		//网络发送
		
		
		//LED0_TOGGLE();
		
		WDI_TOGGLE();
		
		//tick_count=HAL_GetTick();
		if(wizphy_getphylink()==PHY_LINK_OFF)
		{
			for(sn=0;sn<7;sn++)
			{
				close(sn);
			}
			
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			WDI_TOGGLE();
			HAL_Delay(250);
			
			log_info("PHY_LINK_OFF\r\n");
		}
		else 	//采集到一组数据
		{			
			tcps_senddata_ipv4(0, local_port,buff);
		}
		
		//tick_count=HAL_GetTick()-tick_count;
		
		//log_info("tick:%d\r\n",tick_count);
		
		WDI_TOGGLE();
		
		Uart_commend_process();	//串口指令解析
		
		
		
		//测试代码，输出脉冲信号
#ifdef SELF_TRIGGER_TEST
		HAL_Delay(100);
		
		{
			uint16_t ix=60;
			while(ix--)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
				//HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
				HAL_Delay(1);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
				//HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
				HAL_Delay(1);
			}
		}
#endif
		
		
		
		
	}
#endif
	
	
#if	0
  while (1)//AD7606操作测试
  {
		HAL_Delay(10);
		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_RESET);

		HAL_Delay(10);
		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_SET);
		

		if(AD_flag==0x01)
		{
			AD_flag=0;
			AD_count++;
			
			log_info("AD_count=%d\r\n",AD_count);
			
			for(uint8_t i=0;i<=15;i++)
			{
				log_info("AD_dat[%d]:0x%x\r\n",i,AD_dat[i]);
			}
			
//			for(uint8_t i=0;i<=7;i++)
//			{
//				log_info("AD_B_dat[%d]:0x%x\r\n",i,AD_B_dat[i]);
//			}
			log_info("\r\n");
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
#endif
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}


void MPU_Config( void )
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* 禁止 MPU */
	HAL_MPU_Disable();

	/* 配置AXI SRAM的MPU属性为Write through, read allocate，no write allocate */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = 0x24000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
	
//	/* 配置FMC扩展IO的MPU属性为Device或者Strongly Ordered */
//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = 0x60000000;
//	MPU_InitStruct.Size             = ARM_MPU_REGION_SIZE_64KB;	
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;	/* 不能用MPU_ACCESS_CACHEABLE;会出现2次CS、WE信号 */
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
//	
//	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
	/* 配置SRAM3的属性为Write through, read allocate，no write allocate */
//    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//    MPU_InitStruct.BaseAddress      = 0x38000000;
//    MPU_InitStruct.Size             = ARM_MPU_REGION_SIZE_64KB;	
//    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
//    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
//    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//    MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
//    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
//    MPU_InitStruct.SubRegionDisable = 0x00;
//    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

//    HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/*使能 MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}


/* USER CODE BEGIN 4 */
void HAL_Get_CPU_RCC_Clock(void)
{
	log_info("[HAL_Get_CPU_RCC_Clock]HAL_RCC_SysClockFreq:%dMHz\r\n",HAL_RCC_GetSysClockFreq()/1000000);
	log_info("[HAL_Get_CPU_RCC_Clock]HAL_RCC_HCLKFreq:%dMHz\r\n",HAL_RCC_GetHCLKFreq()/1000000);
	log_info("[HAL_Get_CPU_RCC_Clock]HAL_RCC_PCLK1Freq:%dMHz\r\n",HAL_RCC_GetPCLK1Freq()/1000000);
	log_info("[HAL_Get_CPU_RCC_Clock]HAL_RCC_PCLK2Freq:%dMHz\r\n",HAL_RCC_GetPCLK2Freq()/1000000);	
	
	//log_info("[%s][%s][%d]Error_Handler\r\n",__FILE__,__func__,__LINE__);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
