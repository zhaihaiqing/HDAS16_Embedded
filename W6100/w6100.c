/***********************************************************************************
�ɶ���Ȼ�������޹�˾
WIZnet�ٷ�һ��������,��09����һֱ����������һ��Ϊ�ͻ��ṩ��������Ʒ���ۺ��ȫ��λ����
�绰��028-86127089     0755-86066647
���棺028-86127039
��ַ��http://www.hschip.com
���ڣ�2020-03

�ٷ��Ա�  
���Զ�:http://shop325030069.taobao.com/index.htm
�ֻ���:https://shop325030069.m.taobao.com


Ӳ��ƽ̨��
��Ȼ����������  HS-EVBW6100/STM32    (HS-EVBW5100S/STM32�װ�ȫ����)
WIZnet��������QQȺ��       722479032
W5100S/W6100��������QQȺ�� 690436248

										 
SPIģʽ��������ģʽͨ�� wizchip_conf.h �ļ���
//   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_BUS_INDIR_     //  �������
   #define _WIZCHIP_IO_MODE_         _WIZCHIP_IO_MODE_SPI_VDM_           //  SPIģʽ
������ѡ��
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
   IINCHIP_SpiSendData( tAD[2]);	// ���ƶ�
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
	 uint16_t idx = 0;		// idx����Ϊ����д��ĵڼ�����
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
   if(len == 0) log_info("Unexpected2 length 0\r\n");			// д������Ϊ�գ�len��ʾд�����ݵĳ���
   IINCHIP_SpiSendData( tAD[0]);		// ��ַ�Σ��ṩ16λƫ�Ƶ�ַ��0000 0000 0000 0000��
   IINCHIP_SpiSendData( tAD[1]);		// ���ƶΣ���8λ��0000 0000 ��5λBSBλΪ00000��ʾͨ�üĴ�����
   IINCHIP_SpiSendData( tAD[2]);    // ���ƶ�+4(0000 0100 RWBλ��1��ʾд�룬OMλΪ00��ʾSPI����ģʽΪVDM)
	
	 HAL_SPI_TransmitReceive(&hspi1, pBuf, rx, len, 50);
	
//   for(idx = 0; idx < len; idx++)                				// ���ݶΣ�д������ֵ
//   {
//     IINCHIP_SpiSendData( pBuf[idx]);											// MCUͨ��SPI��������
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
if(len == 0)																				// len����Ϊ��ȡ���ݵĳ���
  {	
   // printf("Unexpected2 length 0\r\n");								// ��ȡ���ݳ���Ϊ0
  }
#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_))
   tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);
   IINCHIP_SpiSendData(tAD[0]);		// ��ַ��
  IINCHIP_SpiSendData( tAD[1]);		// ���ƶ�
  IINCHIP_SpiSendData( tAD[2]);    		// ���ƶ�
  for(idx = 0; idx < len; idx++)                    	// ���ݶΣ���ȡ����ֵ
  {
    pBuf[idx] = IINCHIP_SpiSendData(0x00);							// ��MCUͨ��SPI���͹��������ݴ����buf������
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
