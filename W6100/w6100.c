//*****************************************************************************
//
//! \file w6100.c
//! \brief W6100 HAL Interface.
//! \version 1.0.0
//! \date 2018/08/22
//! \par  Revision history//!       
//!       <2018/08/22> 1st Release
//! \author DKay
//! \copyright
//!
//! Copyright (c)  2013, WIZnet Co., LTD.
//! All rights reserved.
//!
//! Redistribution and use in source and binary forms, with or without
//! modification, are permitted provided that the following conditions
//! are met:
//!
//!     * Redistributions of source code must retain the above copyright
//! notice, this list of conditions and the following disclaimer.
//!     * Redistributions in binary form must reproduce the above copyright
//! notice, this list of conditions and the following disclaimer in the
//! documentation and/or other materials provided with the distribution.
//!     * Neither the name of the <ORGANIZATION> nor the names of its
//! contributors may be used to endorse or promote products derived
//! from this software without specific prior written permission.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//! THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************
#include <stdio.h>
#include "w6100.h"
#include "socket.h"
//#include "usart.h"

#define _W6100_SPI_OP_          0x00

#if   (_WIZCHIP_ == W6100)
//////////////////////////////////////////////////
void     WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb )
{
   uint8_t spi_data[4];

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_))
   AddrSel |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);

   //if(!WIZCHIP.IF.SPI._read_burst || !WIZCHIP.IF.SPI._write_burst) 	// byte operation
   if(!WIZCHIP.IF.SPI._write_burst) 	// byte operation
   {
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
    	WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
		WIZCHIP.IF.SPI._write_byte(wb);
   }
   else									// burst operation
   {
		spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
		spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
		spi_data[2] = (AddrSel & 0x000000FF) >> 0;
		spi_data[3] = wb;
		WIZCHIP.IF.SPI._write_burst(spi_data, 4);
   }
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
	  WIZCHIP.IF.BUS._write_data(IDM_AR0,(AddrSel & 0xFF0000) >>  16);
		WIZCHIP.IF.BUS._write_data(IDM_AR1,(AddrSel & 0x00FF00) >>  8);
    WIZCHIP.IF.BUS._write_data(IDM_BSB,(AddrSel & 0x0000FF));	
    WIZCHIP.IF.BUS._write_data(IDM_DR,wb);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100. !!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}

uint8_t  WIZCHIP_READ(uint32_t AddrSel)
{
   uint8_t ret;
   uint8_t spi_data[3];

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ ==  _WIZCHIP_IO_MODE_SPI_))
   AddrSel |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);

   if(!WIZCHIP.IF.SPI._read_burst || !WIZCHIP.IF.SPI._write_burst) 	// byte operation
   {
	    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
	    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
	   	WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
   }
   else																// burst operation
   {
		spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
		spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
		spi_data[2] = (AddrSel & 0x000000FF) >> 0;
		WIZCHIP.IF.SPI._write_burst(spi_data, 3);
   }

   ret = WIZCHIP.IF.SPI._read_byte();
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
		WIZCHIP.IF.BUS._write_data(IDM_AR0,(AddrSel & 0xFF0000) >>  16);
		WIZCHIP.IF.BUS._write_data(IDM_AR1,(AddrSel & 0x00FF00) >>  8);
    WIZCHIP.IF.BUS._write_data(IDM_BSB,(AddrSel & 0x0000FF));
    ret = WIZCHIP.IF.BUS._read_data(IDM_DR);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100S. !!!"   
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
   return ret;
}

void     WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint8_t spi_data[3];
   uint16_t i;

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_))
   AddrSel |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);

   if(!WIZCHIP.IF.SPI._write_burst) 	// byte operation
   {
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
		for(i = 0; i < len; i++)
			WIZCHIP.IF.SPI._write_byte(pBuf[i]);
   }
   else									// burst operation
   {
		spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
		spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
		spi_data[2] = (AddrSel & 0x000000FF) >> 0;
		WIZCHIP.IF.SPI._write_burst(spi_data, 3);
		WIZCHIP.IF.SPI._write_burst(pBuf, len);
   }
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
	WIZCHIP.IF.BUS._write_data(IDM_AR0,(AddrSel & 0xFF0000) >>  16);
    WIZCHIP.IF.BUS._write_data(IDM_AR1,(AddrSel & 0x00FF00) >>  8);
    WIZCHIP.IF.BUS._write_data(IDM_BSB,(AddrSel & 0x0000FF));
    for(i = 0 ; i < len; i++)
       WIZCHIP.IF.BUS._write_data(IDM_DR,pBuf[i]);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100S. !!!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}

void     WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint8_t spi_data[3];
   uint16_t i;

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if((_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_))
   AddrSel |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);

   if(!WIZCHIP.IF.SPI._read_burst || !WIZCHIP.IF.SPI._write_burst) 	// byte operation
   {
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
		WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
		for(i = 0; i < len; i++)
		   pBuf[i] = WIZCHIP.IF.SPI._read_byte();
   }
   else																// burst operation
   {
		spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
		spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
		spi_data[2] = (AddrSel & 0x000000FF) >> 0;
		WIZCHIP.IF.SPI._write_burst(spi_data, 3);
		WIZCHIP.IF.SPI._read_burst(pBuf, len);
   }
#elif ( (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) )
	WIZCHIP.IF.BUS._write_data(IDM_AR0,(AddrSel & 0xFF0000) >>  16);
	WIZCHIP.IF.BUS._write_data(IDM_AR1,(AddrSel & 0x00FF00) >>  8);
    WIZCHIP.IF.BUS._write_data(IDM_BSB,(AddrSel & 0x0000FF));	
    for(i = 0 ; i < len; i++)
       pBuf[i]	= WIZCHIP.IF.BUS._read_data(IDM_DR);
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5100S. !!!!"
#endif
   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}




void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;

//   if(len == 0)  return;
   ptr = getSn_TX_WR(sn);
   //M20140501 : implict type casting -> explict type casting
   //addrsel = (ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   //
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);

   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;

   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   //M20140501 : implict type casting -> explict type casting
   //addrsel = ((ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   //
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;

   setSn_RX_RD(sn,ptr);
}


void wiz_recv_ignore(uint8_t sn, uint16_t len)
{
   uint16_t ptr = 0;

   ptr = getSn_RX_RD(sn);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

void wiz_mdio_write(uint8_t PHYMDIO_regadr, uint16_t var)
{
    WIZCHIP_WRITE(PHYRAR,PHYMDIO_regadr);
    WIZCHIP_WRITE(PHYDIR1, (uint8_t)(var >> 8));
    WIZCHIP_WRITE(PHYDIR0, (uint8_t)(var));
    WIZCHIP_WRITE(PHYACR, PHYACR_WRITE);
    while(WIZCHIP_READ(PHYACR));  //wait for command complete
}

uint16_t wiz_mdio_read(uint8_t PHYMDIO_regadr)
{
    WIZCHIP_WRITE(PHYRAR,PHYMDIO_regadr);
    WIZCHIP_WRITE(PHYACR, PHYACR_READ);
    while(WIZCHIP_READ(PHYACR));  //wait for command complete
    return ((uint16_t)WIZCHIP_READ(PHYDOR1) << 8) | WIZCHIP_READ(PHYDOR0);
}

void chk_ip_version(uint8_t sn)
{
	uint8_t ESR = 0;

	if (getSn_SR(sn) == SOCK_ESTABLISHED)
	{
		ESR = getSn_ESR(sn);
		if (ESR&4)	;	//log_info("IPv6 client is connected\r\n");
		else		;	//log_info("IPv4 client is connected\r\n");
	}
	else	;	//log_info("socket isn`t established\r\n");	
}

int8_t getDestAddr(uint8_t sn, uint8_t * dest_addr)		//only TCP
{
	uint8_t dest_ver;
	
	if (getSn_SR(sn) != SOCK_ESTABLISHED) return SOCKERR_SOCKSTATUS;
	
	dest_ver = getSn_ESR(sn);
	if (dest_ver & 0x04)  //IPv6
	{
		getSn_DIP6R(sn,dest_addr);
	}
	else	getSn_DIPR(sn,dest_addr);
	
	return 0;
}

void wiz_delay_ms(uint32_t milliseconds)
{
	uint32_t i;
	for(i = 0 ; i < milliseconds ; i++)
	{
		//Write any values to clear the TCNTCLKR register
		setTCNTCLR();

		// Wait until counter register value reaches 10.(10 = 1ms : TCNTR is 100us tick counter register)
		while(getTCNTR() < 0x0a){}
	}
}

uint16_t getSn_RX_RSR(uint8_t s)
{
	uint16_t val_halfword=0,val1_halfword=0;
    uint8_t val[2]={0, };
    uint8_t val1[2]={0, };
	do{
		WIZCHIP_READ_BUF(Sn_RX_RSR(s), val1, 2);
		val1_halfword = (uint16_t)(val1[0]<<8)+ val1[1];
		if (val1_halfword != 0){
		    WIZCHIP_READ_BUF(Sn_RX_RSR(s), val, 2);
		    val_halfword = (uint16_t)(val[0]<<8)+ val[1];
		}
	}while (val_halfword != val1_halfword);
	return val_halfword;
}

void setSLPIPR(uint8_t * addr)
{
           WIZCHIP_WRITE_BUF(SLPIPR12, addr, 4); 
}

void getSLPIPR(uint8_t * addr)
{
           WIZCHIP_READ_BUF(SLPIPR12, addr, 4); 
}

void setSLPIP6R(uint8_t * addr)
{
           WIZCHIP_WRITE_BUF(SLPIPR00, addr, 16); 
}

void getSLPIP6R(uint8_t * addr)
{
           WIZCHIP_READ_BUF(SLPIPR00, addr, 16); 
}
#endif


//#endif
