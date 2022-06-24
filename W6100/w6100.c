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





/*
一、设置步骤：
		1：初始化IO、SPI等操作
		2：设置MAC地址，IPV4
		3：设置是否启用DHCP
		4：设置IP地址
		5：初始化socket，设置发送接收缓存大小
		
二、引脚描述
		SPI模式下，仅需要设置SPI4根数据线、INT、RST
		
三、socket
共计8个socket，默认2K发送或接收缓存，最大不超过16K
*/




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


///***
//*@brief: send data in BUS mode 
//*@parame: addr: register address
//*         data: value write to register
//*@return: none
//*****/
//void IINCHIP_BusSendData(uint32_t addr,uint8_t data)
//{
//	 *((volatile uint8_t*)addr) = data;

//}
///***
//*@brief: read data in BUS mode 
//*@parame: addr: register address
//*         data: value read from register
//*@return: register value
//*****/
//uint8_t IINCHIP_BusReadData(uint32_t addr)
//{
//	 return *((volatile uint8_t*)addr) ;

//}

///***
//*@brief:  pull down cs pin 
//*@parame: none
//*@return: none
//*****/
//void IINCHIP_CSoff(void)
//{
//  WIZ_CS(LOW);
//}

///***
//*@brief: pull up cs pin 
//*@parame: none
//*@return: none
//*****/
//void IINCHIP_CSon(void)
//{
//   WIZ_CS(HIGH);
//}

///***
//*@brief: send data in SPI mode 
//*@parame: data: value write to register
//*@return: none
//*****/
//uint8_t IINCHIP_SpiSendData(uint8_t dat)
//{
//   return(SPI2_SendByte(dat));
//}




//////////////////////////////////////////////////
void WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb )
{
   uint8_t tAD[4];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
   tAD[3] = wb;

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._s_e_l_e_c_t_();

#if( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);
   WIZCHIP.IF.SPI._write_byte_buf(tAD, 4);

#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   WIZCHIP.IF.BUS._write_data_buf(IDM_AR0, tAD, 4, 1);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100. !!!"
#endif

   WIZCHIP.CS._d_e_s_e_l_e_c_t_();
   WIZCHIP_CRITICAL_EXIT();
}

uint8_t  WIZCHIP_READ(uint32_t AddrSel)
{
   uint8_t ret;
   uint8_t tAD[3];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._s_e_l_e_c_t_();

#if( (_WIZCHIP_IO_MODE_ ==  _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);
   WIZCHIP.IF.SPI._write_byte_buf(tAD, 3);
   ret = WIZCHIP.IF.SPI._read_byte();
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   WIZCHIP.IF.BUS._write_data_buf(IDM_AR0,tAD,3,1);
   ret = WIZCHIP.IF.BUS._read_data(IDM_DR);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!"   
#endif

   WIZCHIP.CS._d_e_s_e_l_e_c_t_();
   WIZCHIP_CRITICAL_EXIT();
   return ret;
}

void WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
   uint8_t tAD[3];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);


   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._s_e_l_e_c_t_();

#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);

   WIZCHIP.IF.SPI._write_byte_buf(tAD, 3);
   WIZCHIP.IF.SPI._write_byte_buf(pBuf, len);

#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   WIZCHIP.IF.BUS._write_data_buf(IDM_AR0,tAD, 3, 1);
   WIZCHIP.IF.BUS._write_data_buf(IDM_DR,pBuf,len, 0);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!!"
#endif

   WIZCHIP.CS._d_e_s_e_l_e_c_t_();
   WIZCHIP_CRITICAL_EXIT();
}

void WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
   uint8_t tAD[3];
   tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   tAD[2] = (uint8_t)(AddrSel & 0x000000ff);

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._s_e_l_e_c_t_();

#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);
   WIZCHIP.IF.SPI._write_byte_buf(tAD,3);
   WIZCHIP.IF.SPI._read_byte_buf(pBuf, len);
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
   WIZCHIP.IF.BUS._write_data_buf(IDM_AR0,tAD,3,1);
   WIZCHIP.IF.BUS._read_data_buf(IDM_DR,pBuf,len,0);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W6100. !!!!"
#endif
   WIZCHIP.CS._d_e_s_e_l_e_c_t_();
   WIZCHIP_CRITICAL_EXIT();
}

datasize_t getSn_TX_FSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(_Sn_TX_FSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(_Sn_TX_FSR_(sn),1));
   }while (val != prev_val);
   return val;
}

datasize_t getSn_RX_RSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(_Sn_RX_RSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(_Sn_RX_RSR_(sn),1));
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






































/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
