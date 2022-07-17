/***********************************************************************************
成都浩然电子有限公司
WIZnet官方一级代理商,自09年起一直蝉联销量第一，为客户提供技术、产品、售后等全方位服务
电话：028-86127089     0755-86066647
传真：028-86127039
网址：http://www.hschip.com
日期：2020-03

官方淘宝  
电脑端:http://shop325030069.taobao.com/index.htm
手机端:https://shop325030069.m.taobao.com


硬件平台：
浩然电子评估板  HS-EVBW6100/STM32    (HS-EVBW5100S/STM32底板全兼容)
WIZnet技术交流QQ群：       722479032
W5100S/W6100技术交流QQ群： 690436248

										 
SPI模式或间接总线模式通过 wizchip_conf.h 文件的
//   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_BUS_INDIR_     //  间接总线
   #define _WIZCHIP_IO_MODE_         _WIZCHIP_IO_MODE_SPI_VDM_           //  SPI模式
来进行选择
***********************************************************************************/


#include "w6100.h"
#include "SPI.h"
#include <stdio.h>
#include "w6100_init.h"

#define _WIZCHIP_SPI_VDM_OP_    0x00
#define _WIZCHIP_SPI_FDM_LEN1_  0x01
#define _WIZCHIP_SPI_FDM_LEN2_  0x02
#define _WIZCHIP_SPI_FDM_LEN4_  0x03
//
// If you want to use SPI FDM mode, Feel free contact to WIZnet. 
// http://forum.wiznet.io
//

#if _WIZCHIP_ == 6100
////////////////////////////////////////////////////////////////////////////////////////


#define _W6100_SPI_OP_          _WIZCHIP_SPI_VDM_OP_

/***
*@brief: send data in BUS mode 
*@parame: addr: register address
*         data: value write to register
*@return: none
*****/
void IINCHIP_BusSendData(uint32_t addr,uint8_t data)
{
	 *((volatile uint8_t*)addr) = data;

}
/***
*@brief: read data in BUS mode 
*@parame: addr: register address
*         data: value read from register
*@return: register value
*****/
uint8_t IINCHIP_BusReadData(uint32_t addr)
{
	 return *((volatile uint8_t*)addr) ;

}

/***
*@brief:  pull down cs pin 
*@parame: none
*@return: none
*****/
void IINCHIP_CSoff(void)
{
  //WIZ_CS(LOW);
	HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_RESET);
}

/***
*@brief: pull up cs pin 
*@parame: none
*@return: none
*****/
void IINCHIP_CSon(void)
{
   //WIZ_CS(HIGH);
	HAL_GPIO_WritePin(W_CSN_GPIO_Port,W_CSN_Pin,GPIO_PIN_SET);
}

/***
*@brief: send data in SPI mode 
*@parame: data: value write to register
*@return: none
*****/
//uint8_t IINCHIP_SpiSendData(uint8_t dat)
//{
//   return(SPI1_SendByte(dat));
//}


uint8_t IINCHIP_SpiSendData(uint8_t dat)
{
	//uint8_t rx = 0, tx = 0xFF;
	uint8_t rx = 0;
	HAL_SPI_TransmitReceive(&hspi1, &dat, &rx, 1, 10);
	return rx;
}


//uint8_t IINCHIP_SpiSendBufData(uint8_t dat)
//{
//	//uint8_t rx = 0, tx = 0xFF;
//	uint8_t rx = 0;
//	HAL_SPI_TransmitReceive(&hspi1, &dat, &rx, 1, 10);
//	return rx;
//}




//////////////////////////////////////////////////
void WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb )
{
   uint8_t tAD[4];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
   tAD[3] = wb;

   //WIZCHIP_CRITICAL_ENTER();
  // WIZCHIP.CS._s_e_l_e_c_t_();
     IINCHIP_CSoff(); 
	
#if( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
    tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);
    //IINCHIP_SpiSendData( 0xf0);
	  IINCHIP_SpiSendData(tAD[0]);// Address byte 1
    IINCHIP_SpiSendData(tAD[1] );// Address byte 2
    IINCHIP_SpiSendData(tAD[2]);// Address byte 1
    IINCHIP_SpiSendData(tAD[3] );// Address byte 2
    

#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   IINCHIP_BusSendData(IDM_AR0 ,tAD[0] );
	 IINCHIP_BusSendData(IDM_AR1 ,tAD[1] );
	 IINCHIP_BusSendData(IDM_BSR ,tAD[2] );
	 IINCHIP_BusSendData(IDM_DR, tAD[3]);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100. !!!"
#endif
   IINCHIP_CSon(); 
  // WIZCHIP.CS._d_e_s_e_l_e_c_t_();
  // WIZCHIP_CRITICAL_EXIT();
}

#define MODE_SPI  1
#define MODE_BUS  0


uint8_t  WIZCHIP_READ(uint32_t AddrSel)
{
   uint8_t ret;
   uint8_t tAD[3];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);

  // WIZCHIP_CRITICAL_ENTER();
  // WIZCHIP.CS._s_e_l_e_c_t_();
 IINCHIP_CSoff(); 
#if( (_WIZCHIP_IO_MODE_ ==  _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);
   IINCHIP_SpiSendData( tAD[0]);	
   IINCHIP_SpiSendData( tAD[1]);
   IINCHIP_SpiSendData( tAD[2]);	// 控制段
   ret = IINCHIP_SpiSendData(0x00);      
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   IINCHIP_BusSendData(IDM_AR0 ,tAD[0]);
	 IINCHIP_BusSendData(IDM_AR1 ,tAD[1]);
	 IINCHIP_BusSendData(IDM_BSR ,tAD[2] );
	 ret = IINCHIP_BusReadData(IDM_DR);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!"   
#endif

  // WIZCHIP.CS._d_e_s_e_l_e_c_t_();
  // WIZCHIP_CRITICAL_EXIT();
	 IINCHIP_CSon(); 
   return ret;
}

void WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
	 uint8_t rx[2048] = {0};
	 uint16_t idx = 0;		// idx定义为正在写入的第几个数
   uint8_t tAD[3];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);

   IINCHIP_CSoff();  
   // WIZCHIP_CRITICAL_ENTER();
   //WIZCHIP.CS._s_e_l_e_c_t_();

#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);
   //	printf("write\r\n");																
   if(len == 0) log_info("Unexpected2 length 0\r\n");			// 写入数据为空；len表示写入数据的长度
   IINCHIP_SpiSendData( tAD[0]);		// 地址段，提供16位偏移地址（0000 0000 0000 0000）
   IINCHIP_SpiSendData( tAD[1]);		// 控制段，共8位（0000 0000 高5位BSB位为00000表示通用寄存器）
   IINCHIP_SpiSendData( tAD[2]);    // 控制段+4(0000 0100 RWB位置1表示写入，OM位为00表示SPI工作模式为VDM)
	
	 HAL_SPI_TransmitReceive(&hspi1, pBuf, rx, len, 50);
	
//   for(idx = 0; idx < len; idx++)                				// 数据段，写入数据值
//   {
//     IINCHIP_SpiSendData( pBuf[idx]);											// MCU通过SPI发送数据
//   }

#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   IINCHIP_BusSendData(IDM_AR0 ,tAD[0]);
	 IINCHIP_BusSendData(IDM_AR1 ,tAD[1]);
	 	 IINCHIP_BusSendData(IDM_BSR ,tAD[2] );
	 for(idx = 0; idx < len; idx++)                // Write data in loop
   {
		 IINCHIP_BusSendData(IDM_DR, pBuf[idx]);
	 }
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!!"
#endif

  // WIZCHIP.CS._d_e_s_e_l_e_c_t_();
  // WIZCHIP_CRITICAL_EXIT();
	IINCHIP_CSon(); 
}

void WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
   uint8_t tAD[3];
	 uint16_t idx = 0;		
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);

  // WIZCHIP_CRITICAL_ENTER();
  // WIZCHIP.CS._s_e_l_e_c_t_();
	IINCHIP_CSoff();        
if(len == 0)																				// len定义为读取数据的长度
  {	
   // printf("Unexpected2 length 0\r\n");								// 读取数据长度为0
  }
#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);
   IINCHIP_SpiSendData(tAD[0]);		// 地址段
  IINCHIP_SpiSendData( tAD[1]);		// 控制段
  IINCHIP_SpiSendData( tAD[2]);    		// 控制段
  for(idx = 0; idx < len; idx++)                    	// 数据段，读取数据值
  {
    pBuf[idx] = IINCHIP_SpiSendData(0x00);							// 将MCU通过SPI发送过来的数据存放在buf数组中
  }
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   IINCHIP_BusSendData(IDM_AR0 ,tAD[0]);
	 IINCHIP_BusSendData(IDM_AR1 ,tAD[1]);
	 	 IINCHIP_BusSendData(IDM_BSR ,tAD[2] );
	 for(idx = 0; idx < len; idx++)                // Write data in loop
   {
		 pBuf[idx] =IINCHIP_BusReadData(IDM_DR);
	 }
	
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!!"
#endif
	 IINCHIP_CSon();  
}

datasize_t getSn_TX_FSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(W6100_Sn_TX_FSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(W6100_Sn_TX_FSR_(sn),1));
   }while (val != prev_val);
   return val;
}

datasize_t getSn_RX_RSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(W6100_Sn_RX_RSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(W6100_Sn_RX_RSR_(sn),1));
   }while (val != prev_val);
   return val;
}

void wiz_send_data(uint8_t sn, uint8_t *wizdata, datasize_t len)
{
   datasize_t ptr = 0;
   uint32_t addrsel = 0;
   ptr = getSn_TX_WR(sn);
   addrsel = ((uint32_t)ptr << 8) + WIZCHIP_TXBUF_BLOCK(sn);
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);
   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, datasize_t len)
{
   datasize_t ptr = 0;
   uint32_t addrsel = 0;
   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   addrsel = ((uint32_t)ptr << 8) + WIZCHIP_RXBUF_BLOCK(sn);
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

void wiz_recv_ignore(uint8_t sn, datasize_t len)
{
   setSn_RX_RD(sn,getSn_RX_RD(sn)+len);
}


/// @cond DOXY_APPLY_CODE
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
/// @endcond
void wiz_mdio_write(uint8_t phyregaddr, uint16_t var)
{
   setPHYRAR(phyregaddr);
   setPHYDIR(var);
   setPHYACR(PHYACR_WRITE);
   while(getPHYACR());  //wait for command complete
}

uint16_t wiz_mdio_read(uint8_t phyregaddr)
{
   setPHYRAR(phyregaddr);
   setPHYACR(PHYACR_READ);
   while(getPHYACR());  //wait for command complete
   return getPHYDOR();
}
/// @cond DOXY_APPLY_CODE
#endif
/// @endcond

////////////////////////////////////////////////////////////////////////////////////////
#endif
