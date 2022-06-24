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




/*MAC地址首字节必须为偶数
  如果多块W5100s网络适配板在同一现场工作，请使用不同的MAC地址
*/
uint8_t mac[6]={0x00,0x08,0xdc,0x22,0x33,0x33};

/*定义默认IP信息*/
uint8_t local_ip[4] = {192,168,1,233};                        // 定义W5100s默认IP地址
uint8_t subnet[4] = {255,255,255,0};                          // 定义W5100s默认子网掩码
uint8_t gateway[4] = {192,168,1,1};                           // 定义W5100s默认网关
uint8_t dns_server[4] = {114,114,114,114};                    // 定义W5100s默认DNS

uint16_t local_port = 8080;                                   // 定义本地端口

/*定义远端IP信息*/
uint8_t  remote_ip[4] = {192,168,1,100};                      // 远端IP地址
uint16_t remote_port = 8080;                                  // 远端端口号


/*
socket发送缓存定义为16K，采集400组数据或超时（1S）后发送一次，这样在20K的采样率下，每隔20mS发送一组数据，建议采样率不超过100K


设备采用TCP服务器模式，采集到的数据通过以太网发送出去

*/



wiz_NetInfo gWIZNETINFO = { .mac = {0x00,0x08,0xdc,0x57,0x57,0x20},
							.ip = {192,168,0,13},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 0, 1},
							.dns = {168, 126, 63, 1},
							.dhcp = NETINFO_STATIC,
							.lla = {0xfe,0x80, 0x00,0x00,
								 0x00,0x00, 0x00,0x00,
								 0x02,0x08, 0xdc,0xff,
								 0xfe,0x57, 0x57,0x61},
				            .gua={0x20,0x01,0x02,0xb8,
								 0x00,0x10,0x00,0x01,
								 0x02,0x08,0xdc,0xff,
								 0xfe,0x57,0x57,0x61},
				            .sn6={0xff,0xff,0xff,0xff,
								 0xff,0xff,0xff,0xff,
								 0x00,0x00,0x00, 0x00,
								 0x00,0x00,0x00,0x00},
				            .gw6={0xfe, 0x80, 0x00,0x00,
								  0x00,0x00,0x00,0x00,
								  0x02,0x00, 0x87,0xff,
								  0xfe,0x08, 0x4c,0x81}
							};


uint8_t WIZ_Dest_IP[4] = {192, 168, 0, 232};                  //DST_IP Address


uint8_t DestIP6_L[16] = {0xfe,0x80, 0x00,0x00,
						  0x00,0x00, 0x00,0x00,
                          0x31,0x71,0x98,0x05,
                          0x70,0x24,0x4b,0xb1
						};

uint8_t DestIP6_G[16] = {0x20,0x01,0x02,0xb8,
                          0x00,0x10,0x00,0x01,
                          0x31,0x71,0x98,0x05,
                          0x70,0x24,0x4b,0xb1
                         };

uint8_t Router_IP[16]= {0xff,0x02,0x00,0x00,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x02
                         };
uint8_t data_buf[2048];


void print_network_information(void);

uint8_t rxData[2];


	
												 
void csEnable(void)
{
	HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_RESET);
}



void csDisable(void)
{
	HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_SET);
}



void spiWriteByte(uint8_t tx)
{
	uint8_t rx;
	//HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, 10);
	HAL_SPI_Transmit(&hspi1,&tx,1,0xffff);
	
}



uint8_t spiReadByte(void)
{
	uint8_t rx = 0, tx = 0xFF;
	HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, 10);
	return rx;
}


void W6100Initialze(void)
{
	intr_kind temp;
	unsigned char W6100_AdrSet[2][4] = {{4,4,0,0},{4,4,0,0}};
	/*
	 */
	temp = IK_DEST_UNREACH;

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W6100_AdrSet) == -1)
	{
		log_info("W6100 initialized fail.\r\n");
	}

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
	{
		log_info("W6100 interrupt\r\n");
	}
	log_info("interrupt mask: %02x\r\n",getIMR());

	do{//check phy status.
		if(ctlwizchip(CW_GET_PHYLINK,(void*)&temp) == -1){
			log_info("Unknown PHY link status.\r\n");
		}
	}while(temp == PHY_LINK_OFF);
}









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

uint32_t AD_count=0;





	



void print_network_information(void)
{
	wizchip_getnetinfo(&gWIZNETINFO);
	log_info("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	log_info("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	log_info("SN Mask	   : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	log_info("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	log_info("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	log_info("LLA  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\r\n",gWIZNETINFO.lla[0],gWIZNETINFO.lla[1],gWIZNETINFO.lla[2],gWIZNETINFO.lla[3],\
									gWIZNETINFO.lla[4],gWIZNETINFO.lla[5],gWIZNETINFO.lla[6],gWIZNETINFO.lla[7],\
									gWIZNETINFO.lla[8],gWIZNETINFO.lla[9],gWIZNETINFO.lla[10],gWIZNETINFO.lla[11],\
									gWIZNETINFO.lla[12],gWIZNETINFO.lla[13],gWIZNETINFO.lla[14],gWIZNETINFO.lla[15]);
	log_info("GUA  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\n\r",gWIZNETINFO.gua[0],gWIZNETINFO.gua[1],gWIZNETINFO.gua[2],gWIZNETINFO.gua[3],\
									gWIZNETINFO.gua[4],gWIZNETINFO.gua[5],gWIZNETINFO.gua[6],gWIZNETINFO.gua[7],\
									gWIZNETINFO.gua[8],gWIZNETINFO.gua[9],gWIZNETINFO.gua[10],gWIZNETINFO.gua[11],\
									gWIZNETINFO.gua[12],gWIZNETINFO.gua[13],gWIZNETINFO.gua[14],gWIZNETINFO.gua[15]);
	log_info("SN6  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\n\r",gWIZNETINFO.sn6[0],gWIZNETINFO.sn6[1],gWIZNETINFO.sn6[2],gWIZNETINFO.sn6[3],\
									gWIZNETINFO.sn6[4],gWIZNETINFO.sn6[5],gWIZNETINFO.sn6[6],gWIZNETINFO.sn6[7],\
									gWIZNETINFO.sn6[8],gWIZNETINFO.sn6[9],gWIZNETINFO.sn6[10],gWIZNETINFO.sn6[11],\
									gWIZNETINFO.sn6[12],gWIZNETINFO.sn6[13],gWIZNETINFO.sn6[14],gWIZNETINFO.sn6[15]);
	log_info("GW6  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\r\n",gWIZNETINFO.gw6[0],gWIZNETINFO.gw6[1],gWIZNETINFO.gw6[2],gWIZNETINFO.gw6[3],\
									gWIZNETINFO.gw6[4],gWIZNETINFO.gw6[5],gWIZNETINFO.gw6[6],gWIZNETINFO.gw6[7],\
									gWIZNETINFO.gw6[8],gWIZNETINFO.gw6[9],gWIZNETINFO.gw6[10],gWIZNETINFO.gw6[11],\
									gWIZNETINFO.gw6[12],gWIZNETINFO.gw6[13],gWIZNETINFO.gw6[14],gWIZNETINFO.gw6[15]);


}
















int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_ETH_Init();
  MX_FMC_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */

	HAL_Get_CPU_RCC_Clock();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
	
	
	bsp_InitAD7606();
	
	HAL_GPIO_WritePin(APOW_CTRL_GPIO_Port, APOW_CTRL_Pin, GPIO_PIN_SET);
	
	
	
	rt_thread_mdelay(100);
	
	
	
//		W_RSTN_L();
//		rt_thread_mdelay(200);
//		
//		W_RSTN_H();
//		
//		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_RESET);
//		rt_thread_mdelay(300);
//		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_SET);
//		rt_thread_mdelay(300);
//		
//		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_RESET);
//		rt_thread_mdelay(300);
//		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_SET);
//		rt_thread_mdelay(300);
//		
//		
//		
//		
//		
//	
//	log_info("< W6100EVB Hal Driver Loop Back TEST!! >\n");

//	//rt_enter_critical();
//	
//	//rt_hw_interrupt_disable();
//	
//  /* SPI method callback registration */
//  reg_wizchip_spi_cbfunc(spiReadByte, spiWriteByte);
//  /* CS function register */
//  reg_wizchip_cs_cbfunc(csEnable,csDisable);

//	
//	
//  NETUNLOCK();
//  wizchip_setnetinfo(&gWIZNETINFO);
//  W6100Initialze();

////rt_hw_interrupt_enable();
////rt_exit_critical();



//  log_info("VERSION(%x) = %.2x \r\n", VER,getVER());
//	
//  print_network_information();

	
	
	
	
	
	
	hdas_thread_creat();	//执行创建任务函数，开始RTOS
	
	
	
	
	
	
	
	
  while (1)
  {
		rt_thread_mdelay(10);
		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_RESET);
		
		
		
		
		
		
		rt_thread_mdelay(10);
		HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_SET);
		
		
		
		
		
		if(AD_flag==0x11)
		{
			AD_flag=0;
			AD_count++;
			
			log_info("AD_count=%d\r\n",AD_count);
			
			for(uint8_t i=0;i<=7;i++)
			{
				log_info("AD_A_dat[%d]:0x%x\r\n",i,AD_A_dat[i]);
			}
			
			for(uint8_t i=0;i<=7;i++)
			{
				log_info("AD_B_dat[%d]:0x%x\r\n",i,AD_A_dat[i]);
			}
			log_info("\r\n");
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
	rt_kprintf("[HAL_Get_CPU_RCC_Clock]HAL_RCC_SysClockFreq:%dMHz\r\n",HAL_RCC_GetSysClockFreq()/1000000);
	rt_kprintf("[HAL_Get_CPU_RCC_Clock]HAL_RCC_HCLKFreq:%dMHz\r\n",HAL_RCC_GetHCLKFreq()/1000000);
	rt_kprintf("[HAL_Get_CPU_RCC_Clock]HAL_RCC_PCLK1Freq:%dMHz\r\n",HAL_RCC_GetPCLK1Freq()/1000000);
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
