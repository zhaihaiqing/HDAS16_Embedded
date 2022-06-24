//*****************************************************************************
//
//! \file socket.c
//! \brief SOCKET APIs Implements file.
//! \details SOCKET APIs like as Berkeley Socket APIs. 
//! \version 1.0.0
//! \date 2018/09/20
//! \par  Revision history
//!       <2018/09/20> 1st Release
//! \author MidnightCow
//! \copyright
//!
//! Copyright (c)  2018, WIZnet Co., LTD.
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
#include "socket.h"
#include "w6100.h"
//#include "usart.h"
//#include "bsp.h"

//M20150401 : Typing Error
//#define SOCK_ANY_PORT_NUM  0xC000;
//#define SOCK_ANY_PORT_NUM  0xC000
#define SOCK_ANY_PORT_NUM  0x0400
//#define SOCK_LOCAL_PORT_NUM	0x1388

static uint16_t sock_any_port = SOCK_ANY_PORT_NUM;
//static uint16_t local_port = SOCK_LOCAL_PORT_NUM;
static uint16_t sock_io_mode = 0;

static uint16_t sock_remained_size[_WIZCHIP_SOCK_NUM_] = {0,0,};

//M20150601 : For extern decleation
//static uint8_t  sock_pack_info[_WIZCHIP_SOCK_NUM_] = {0,};
uint8_t  sock_pack_info[_WIZCHIP_SOCK_NUM_] = {0,};


#define CHECK_SOCKNUM()   \
   do{                    \
      if(sn >= _WIZCHIP_SOCK_NUM_) return SOCKERR_SOCKNUM;   \
   }while(0);             \

#define CHECK_SOCKMODE(mode)  \
   do{                     \
      if((getSn_MR(sn) & 0x0F) != mode) return SOCKERR_SOCKMODE;  \
   }while(0);              \
	 
#define CHECK_SOCKMODE_TCP()  \
   do{                     \
			switch (getSn_MR(sn)&0x0F)				\
			{														\
				case Sn_MR_TCP:						\
				case Sn_MR_TCP6:						\
				case Sn_MR_DUALT:						\
					break;										\
				default :											\
					return SOCKERR_SOCKMODE;			\
			}																		\
   }while(0);              \

#define CHECK_SOCKMODE_UDP()  \
   do{                     \
			switch (getSn_MR(sn)&0x0F)				\
			{														\
				case Sn_MR_UDP:						\
				case Sn_MR_UDP6:						\
				case Sn_MR_DUALU:						\
					break;										\
				default :											\
					return SOCKERR_SOCKMODE;			\
			}																		\
   }while(0);              \

#define CHECK_SOCKINIT()   \
   do{                     \
      if((getSn_SR(sn) != SOCK_INIT)) return SOCKERR_SOCKINIT; \
   }while(0);              \

#define CHECK_SOCKDATA()   \
   do{                     \
      if(len == 0) return SOCKERR_DATALEN;   \
   }while(0);              \

int8_t socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag)
{	
	CHECK_SOCKNUM();	
	switch(protocol&0x0F)
	{
      case Sn_MR_TCP :
				{            
            uint32_t taddr;
            getSIPR((uint8_t*)&taddr);
            if(taddr == 0) return SOCKERR_SOCKINIT;
         }
			case Sn_MR_TCP6 :
			case Sn_MR_DUALT :         
      case Sn_MR_UDP :
			case Sn_MR_UDP6 :
			case Sn_MR_DUALU :
      case Sn_MR_MACRAW :
			case Sn_MR_IPRAW :
			case Sn_MR_IPRAW6 :
         break;   
      default :
         return SOCKERR_SOCKMODE;
	}	
	
	if((flag & 0x0F) != 0) return SOCKERR_SOCKFLAG;	   
	if(flag != 0)
	{
   	switch(protocol)
   	{
   	   case Sn_MR_TCP:   		      
					if((flag & (SF_TCP_FPSH|SF_TCP_NODELAY|SF_IO_NONBLOCK))==0) return SOCKERR_SOCKFLAG;
   	      break;
   	   case Sn_MR_UDP:
					if((flag & (SF_IGMP_VER2|SF_BROAD_BLOCK|SF_MULTI_ENABLE))==0) return SOCKERR_SOCKFLAG;
   	      break;
   	   default:
   	      break;
   	}
   }
	close(sn);
	setSn_MR(sn,(protocol|flag));	 
	if(!port)
	{
	   port = sock_any_port++;
	   if(sock_any_port == 0xFFF0) sock_any_port = SOCK_ANY_PORT_NUM;
	}
   setSn_PORT(sn,port);	
   setSn_CR(sn,Sn_CR_OPEN);
   while(getSn_CR(sn));
   sock_io_mode &= ~(1 <<sn);
	sock_io_mode |= ((flag & SF_IO_NONBLOCK) << sn);   
   sock_remained_size[sn] = 0;
   
   sock_pack_info[sn] = PACK_COMPLETED;
   while(getSn_SR(sn) == SOCK_CLOSED) ;
   return sn;
}	   

int8_t close(uint8_t sn)
{
	CHECK_SOCKNUM();
	setSn_CR(sn,Sn_CR_CLOSE);
   /* wait to process the command... */
	while( getSn_CR(sn) );
	/* clear all interrupt of the socket. */
	setSn_IRCLR(sn, 0xFF);
	//A20150401 : Release the sock_io_mode of socket n.
	sock_io_mode &= ~(1<<sn);	
	sock_remained_size[sn] = 0;
	sock_pack_info[sn] = 0;
	while(getSn_SR(sn) != SOCK_CLOSED);
	return SOCK_OK;
}

int8_t listen(uint8_t sn)
{
	CHECK_SOCKNUM();
	CHECK_SOCKMODE_TCP();
	CHECK_SOCKINIT();
	setSn_CR(sn,Sn_CR_LISTEN);
	while(getSn_CR(sn));
	while(getSn_SR(sn) != SOCK_LISTEN)
	{
		close(sn);
		return SOCKERR_SOCKCLOSED;
	}
	return SOCK_OK;
}


int8_t connect(uint8_t sn, uint8_t * addr, uint16_t port, uint8_t addrlen)
{	
	uint8_t loop_cnt;
	
	CHECK_SOCKNUM();
	CHECK_SOCKMODE_TCP();
	CHECK_SOCKINIT();

	if(addrlen == 16)
	{
		for (loop_cnt=0; loop_cnt<16; loop_cnt++)
		{
			if (addr[loop_cnt]== 0xFF)
			{
				if (loop_cnt==15)	return SOCKERR_IPINVALID;
			}
			else	break;
		}
		for (loop_cnt =0; loop_cnt<16; loop_cnt++)
		{
			if (addr[loop_cnt]== 0x00)
			{
				if (loop_cnt==15)	return SOCKERR_IPINVALID;
			}
			else	break;
		}
	}
	else if(addrlen == 4)
	{
		for (loop_cnt=0; loop_cnt<4; loop_cnt++)
		{
			if (addr[loop_cnt]== 0xFF)
			{
				if (loop_cnt==3)	return SOCKERR_IPINVALID;
			}
			else	break;
		}
		for (loop_cnt=0; loop_cnt<4; loop_cnt++)
		{
			if (addr[loop_cnt]== 0x00)
			{
				if (loop_cnt==3)	return SOCKERR_IPINVALID;
			}
			else	break;
		}
	}
	else return SOCKERR_IPINVALID;
	
	if(port == 0) return SOCKERR_PORTZERO;
	if (addrlen == 16)
	{
		setSn_DIP6R(sn,addr);
	}
	else if (addrlen == 4)
	{
		setSn_DIPR(sn,addr);
	}
	else return SOCKERR_IPINVALID;	

	setSn_DPORT(sn,port);

	if (addrlen == 16)
	{
		setSn_CR(sn,Sn_CR_CONNECT6);
	}
	else if (addrlen == 4)
	{
		setSn_CR(sn,Sn_CR_CONNECT);
	}
	else return SOCKERR_IPINVALID;
	
	while(getSn_CR(sn));
	while(getSn_SR(sn) != SOCK_ESTABLISHED)
	{
		if (getSn_IR(sn) & Sn_IR_TIMEOUT)
		{
			setSn_IRCLR(sn, Sn_IR_TIMEOUT);
            return SOCKERR_TIMEOUT;
		}

		if (getSn_SR(sn) == SOCK_CLOSED)
		{
			return SOCKERR_SOCKCLOSED;
		}
	}   
   return SOCK_OK;
}

int8_t disconnect(uint8_t sn)
{
	CHECK_SOCKNUM();
	CHECK_SOCKMODE_TCP();
	setSn_CR(sn,Sn_CR_DISCONNECT);
	/* wait to process the command... */
	while(getSn_CR(sn));
	if(sock_io_mode & (1<<sn)) return SOCK_BUSY;
	while(getSn_SR(sn) != SOCK_CLOSED)
	{
		if(getSn_IR(sn) & Sn_IR_TIMEOUT)
		{
			close(sn);
			return SOCKERR_TIMEOUT;
		}
	}
	return SOCK_OK;
}

int32_t send(uint8_t sn, uint8_t * buf, uint16_t len)
{
	uint8_t tmp=0;
	uint16_t freesize=0;
	uint8_t status=0;
   
	CHECK_SOCKNUM();
	CHECK_SOCKMODE_TCP();
//dkay	CHECK_SOCKDATA();
	tmp = getSn_SR(sn);
	if(tmp != SOCK_ESTABLISHED && tmp != SOCK_CLOSE_WAIT) return SOCKERR_SOCKSTATUS;

	freesize = getSn_TxMAX(sn)*1024;
	if (len > freesize) len = freesize; // check size not to exceed MAX size.
	while(1)
	{
		freesize = getSn_TX_FSR(sn);
		tmp = getSn_SR(sn);
		if ((tmp != SOCK_ESTABLISHED) && (tmp != SOCK_CLOSE_WAIT))
		{
			close(sn);
			return SOCKERR_SOCKSTATUS;
		}
		if( (sock_io_mode & (1<<sn)) && (len > freesize) ) return SOCK_BUSY;
		if(len <= freesize) break;
	}
	wiz_send_data(sn, buf, len);
	setSn_CR(sn,Sn_CR_SEND);
	/* wait to process the command... */
	while(getSn_CR(sn));	
	
	while ( (WIZCHIP_READ(Sn_IR(sn) ) & Sn_IR_SENDOK) != Sn_IR_SENDOK )
	{        
		status = WIZCHIP_READ(Sn_SR(sn));
		if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT) )
		{
			//log_info("SEND_OK Problem!!\r\n");
			close(sn);
			return 0;
		}
	}	
	WIZCHIP_WRITE( Sn_IRCLR(sn) , Sn_IR_SENDOK);
	
	return (int32_t)len;
}


int32_t recv(uint8_t sn, uint8_t * buf, uint16_t len)
{
	uint8_t  tmp = 0;
	uint16_t maxsize = 0;

	CHECK_SOCKNUM();
	CHECK_SOCKMODE_TCP();
	CHECK_SOCKDATA();
   
	maxsize = getSn_RxMAX(sn)*1024;	
	if(maxsize < len) len = maxsize;
      

	while(1)
	{
		tmp = getSn_SR(sn);
		if (tmp != SOCK_ESTABLISHED)
		{
			if(tmp == SOCK_CLOSE_WAIT)
			{
				if(len != 0) break;
				else if(getSn_TX_FSR(sn) == maxsize)
				{
					close(sn);
					return SOCKERR_SOCKSTATUS;
				}
			}
			else
			{
				close(sn);
				return SOCKERR_SOCKSTATUS;
			}
		}
		if((sock_io_mode & (1<<sn)) && (len == 0)) return SOCK_BUSY;
		if(len != 0) break;
	};		
	wiz_recv_data(sn, buf, len);	
	
	setSn_CR(sn,Sn_CR_RECV);	
	while(getSn_CR(sn));    
	return (int32_t)len;
}

int32_t sendto(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port, uint8_t addrlen)
{
	uint8_t tmp = 0;
	uint8_t loop_cnt;
	uint16_t freesize = 0;


	CHECK_SOCKNUM();
	if (addrlen == 16)	CHECK_SOCKDATA();
	switch(getSn_MR(sn) & 0x0F)
	{
		case Sn_MR_UDP:
		case Sn_MR_UDP6:
		case Sn_MR_DUALU:
		case Sn_MR_MACRAW:
		case Sn_MR_IPRAW:
		case Sn_MR_IPRAW6:
			break;
		default:
			return SOCKERR_SOCKMODE;
   }
//dkay   CHECK_SOCKDATA();   
	if(addrlen == 16)
	{
		for (loop_cnt=0; loop_cnt<16; loop_cnt++)
		{
			if (addr[loop_cnt]== 0x00)
			{
				if (loop_cnt==15 && ((getSn_MR(sn)&Sn_MR_MACRAW) != Sn_MR_MACRAW))	return SOCKERR_IPINVALID;
			}
			else	break;
		}
	}
	else
	{		
		for (loop_cnt=0; loop_cnt<4; loop_cnt++)
		{
			if (addr[loop_cnt]== 0x00)
			{
				if (loop_cnt==3 && ((getSn_MR(sn)&Sn_MR_MACRAW) != Sn_MR_MACRAW))	return SOCKERR_IPINVALID;
			}
			else	break;
		}

	}
	tmp = getSn_MR(sn);
	if(port  == 0)
	{
		if((tmp != Sn_MR_IPRAW6) && (tmp != Sn_MR_IPRAW) && (tmp != Sn_MR_MACRAW))
			return SOCKERR_PORTZERO;
	}
	tmp = getSn_SR(sn);
	if(tmp != SOCK_MACRAW && tmp != SOCK_UDP && tmp != SOCK_IPRAW && tmp != SOCK_IPRAW6) return SOCKERR_SOCKSTATUS;
	if (addrlen == 16)
	{
		setSn_DIP6R(sn,addr);
	}
	else if (addrlen == 4)
	{
		setSn_DIPR(sn,addr);
	}
	else return SOCKERR_IPINVALID;
	setSn_DPORT(sn,port);      
	freesize = getSn_TxMAX(sn)*1024;
	if (len > freesize) len = freesize; // check size not to exceed MAX size.
	while(1)
	{
		freesize = getSn_TX_FSR(sn);
		if(getSn_SR(sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
		if( (sock_io_mode & (1<<sn)) && (len > freesize) ) return SOCK_BUSY;
		if(len <= freesize) break;
	};
	wiz_send_data(sn, buf, len);
	if (addrlen == 16 && (getSn_MR(sn) & 0x0F) == Sn_MR_UDP)
	{
		return SOCKERR_IPINVALID;
	}
	else if (addrlen == 16)
	{
		setSn_CR(sn,Sn_CR_SEND6);
	}
	else if (addrlen == 4)
	{
		setSn_CR(sn,Sn_CR_SEND);
	}
	else return SOCKERR_IPINVALID;
	while(getSn_CR(sn));
	while(1)
	{
		tmp = getSn_IR(sn);
		if(tmp & Sn_IR_SENDOK)
		{
			setSn_IRCLR(sn, Sn_IR_SENDOK);
			break;
		}      
		else if(tmp & Sn_IR_TIMEOUT)
		{
			setSn_IRCLR(sn, Sn_IR_TIMEOUT);         
			//log_info("time out\r\n");
			return SOCKERR_TIMEOUT;
		}
	}	   
	return (int32_t)len;
}



int32_t recvfrom(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port, uint8_t *addrlen)
{	
   uint8_t  mr;	
   uint8_t  head[8];
	 uint8_t  ver;
	 uint8_t  val[2];
	 uint16_t pack_len=0;

   CHECK_SOCKNUM();

   switch((mr=getSn_MR(sn)) & 0x0F)
   {
      case Sn_MR_UDP:
			case Sn_MR_UDP6:
			case Sn_MR_DUALU:
			case Sn_MR_IPRAW:
			case Sn_MR_IPRAW6:
      case Sn_MR_MACRAW:
         break;   
      default:
         return SOCKERR_SOCKMODE;
   }
   CHECK_SOCKDATA();
   if(sock_remained_size[sn] == 0)
   {
      while(1)
      {
				 pack_len = len;
         if(getSn_SR(sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
         if( (sock_io_mode & (1<<sn)) && (pack_len == 0) ) return SOCK_BUSY;
         if(pack_len != 0) break;
      };
   }

	switch (mr & 0x0F)
	{
	   case Sn_MR_UDP :
		 case Sn_MR_UDP6:
		 case Sn_MR_DUALU:
	      if(sock_remained_size[sn] == 0)
	      {
					wiz_recv_data(sn, head, 2);					
					// read peer's ver, data length					
					ver = (head[0]>>7);
					if(ver == 1) 
            *addrlen = 16;
					else
            *addrlen = 4;
					head[0] &= 0x07;
					pack_len = (uint16_t)(head[0]<<8)+head[1];        
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
      		// read peer's IP address, port number					
					if(ver == 1)
					{
						wiz_recv_data(sn, addr, 16);						
					}
					else
					{
						wiz_recv_data(sn, addr, 4);						
					}
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
					
					wiz_recv_data(sn, val, 2);
					*port = (uint16_t)(val[0]<<8)+val[1];
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));         
					sock_remained_size[sn] = pack_len;
					sock_pack_info[sn] = PACK_FIRST;
   	    }
				if(len < sock_remained_size[sn]) pack_len = len;
				else pack_len = sock_remained_size[sn];				
				wiz_recv_data(sn, buf, pack_len);
				break;
	   case Sn_MR_MACRAW :
	      if(sock_remained_size[sn] == 0)
	      {
   			wiz_recv_data(sn, head, 2);
   			setSn_CR(sn,Sn_CR_RECV);
   			while(getSn_CR(sn));
   			// read peer's IP address, port number & packet length
    			sock_remained_size[sn] = head[0];
   			sock_remained_size[sn] = (sock_remained_size[sn] <<8) + head[1] -2;   			
   			if(sock_remained_size[sn] > 1514) 
   			{
   			   close(sn);
   			   return SOCKFATAL_PACKLEN;
   			}
   			sock_pack_info[sn] = PACK_FIRST;
   	   }
			if(len < sock_remained_size[sn]) pack_len = len;
			else pack_len = sock_remained_size[sn];
			wiz_recv_data(sn,buf,pack_len);
		   break;   
		 case Sn_MR_IPRAW:
		 case Sn_MR_IPRAW6:
		   if(sock_remained_size[sn] == 0)
		   {
				if (mr == Sn_MR_IPRAW)	
				{
					wiz_recv_data(sn, head, 2);
					pack_len = (uint16_t)(head[0]<<8) + head[1];
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
					wiz_recv_data(sn, addr, 4);					
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
				}
				else if(mr == Sn_MR_IPRAW6)	
				{					
					wiz_recv_data(sn, head, 2);					
					// read peer's ver, data length					
					head[0] &= 0x07;
					pack_len = (uint16_t)(head[0]<<8)+head[1];        
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
      		// read peer's IP address, port number										
					wiz_recv_data(sn, addr, 16);					
					setSn_CR(sn,Sn_CR_RECV);
					while(getSn_CR(sn));
				}
				else	return SOCKERR_SOCKSTATUS;   			
   			
   			sock_remained_size[sn] = pack_len;
   			sock_pack_info[sn] = PACK_FIRST;
      }
			
			if(len < sock_remained_size[sn]) pack_len = len;
			else pack_len = sock_remained_size[sn];
   		wiz_recv_data(sn, buf, pack_len);
			break;
      default:
         wiz_recv_ignore(sn, len);
         sock_remained_size[sn] = len;
         break;
   }
	setSn_CR(sn,Sn_CR_RECV);	 
	/* wait to process the command... */
	while(getSn_CR(sn)) ;
	sock_remained_size[sn] -= pack_len;	
	if(sock_remained_size[sn] != 0)
	{
	   sock_pack_info[sn] |= PACK_REMAINED;   
	}
	else sock_pack_info[sn] = PACK_COMPLETED;
	return (int32_t)pack_len;
}

int8_t  ctlsocket(uint8_t sn, ctlsock_type cstype, void* arg)
{
   uint8_t tmp = 0;
   CHECK_SOCKNUM();
   switch(cstype)
   {
      case CS_SET_IOMODE:
         tmp = *((uint8_t*)arg);
         if(tmp == SOCK_IO_NONBLOCK)  sock_io_mode |= (1<<sn);
         else if(tmp == SOCK_IO_BLOCK) sock_io_mode &= ~(1<<sn);
         else return SOCKERR_ARG;
         break;
      case CS_GET_IOMODE:   
         //M20140501 : implict type casting -> explict type casting
         //*((uint8_t*)arg) = (sock_io_mode >> sn) & 0x0001;
         *((uint8_t*)arg) = (uint8_t)((sock_io_mode >> sn) & 0x0001);
         //
         break;
      case CS_GET_MAXTXBUF:
         *((uint16_t*)arg) = getSn_TxMAX(sn);
         break;
      case CS_GET_MAXRXBUF:    
         *((uint16_t*)arg) = getSn_RxMAX(sn);
         break;
      case CS_CLR_INTERRUPT:
         if( (*(uint8_t*)arg) > SIK_ALL) return SOCKERR_ARG;
         setSn_IRCLR(sn,*(uint8_t*)arg);
         break;
      case CS_GET_INTERRUPT:
         *((uint8_t*)arg) = getSn_IR(sn);
         break;   
      default:
         return SOCKERR_ARG;
   }
   return SOCK_OK;
}

int8_t  setsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
 // M20131220 : Remove warning
 //uint8_t tmp;
   CHECK_SOCKNUM();
   switch(sotype)
   {
      case SO_TTL:
         setSn_TTLR(sn,*(uint8_t*)arg);
         break;
      case SO_TOS:
         setSn_TOSR(sn,*(uint8_t*)arg);
         break;
      case SO_MSS:
         setSn_MSSR(sn,*(uint16_t*)arg);
         break;
      case SO_DESTIP:
         setSn_DIPR(sn, (uint8_t*)arg);
         break;
      case SO_DESTPORT:
         setSn_DPORT(sn, *(uint16_t*)arg);
         break;

      case SO_KEEPALIVESEND:
         CHECK_SOCKMODE_TCP();         
            if(getSn_KPALVTR(sn) != 0) return SOCKERR_SOCKOPT;
         
            setSn_CR(sn,Sn_CR_SEND_KEEP);
            while(getSn_CR(sn) != 0)
            {               
              if (getSn_IR(sn) & Sn_IR_TIMEOUT)
							{
								setSn_IRCLR(sn, Sn_IR_TIMEOUT);
                return SOCKERR_TIMEOUT;
							}
            }
         break;
   
      case SO_KEEPALIVEAUTO:
         CHECK_SOCKMODE_TCP();
         setSn_KPALVTR(sn,*(uint8_t*)arg);
         break;         

      default:
         return SOCKERR_ARG;
   }   
   return SOCK_OK;
}

int8_t  getsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
   CHECK_SOCKNUM();
   switch(sotype)
   {
      case SO_FLAG:
         *(uint8_t*)arg = getSn_MR(sn) & 0xF0;
         break;
      case SO_TTL:
         *(uint8_t*) arg = getSn_TTLR(sn);
         break;
      case SO_TOS:
         *(uint8_t*) arg = getSn_TOSR(sn);
         break;
      case SO_MSS:   
         *(uint16_t*) arg = getSn_MSSR(sn);
         break;
      case SO_DESTIP:
         getSn_DIPR(sn, (uint8_t*)arg);
         break;
      case SO_DESTPORT:  
         *(uint16_t*) arg = getSn_DPORT(sn);
         break;   
      case SO_KEEPALIVEAUTO:
         CHECK_SOCKMODE_TCP();
         *(uint16_t*) arg = getSn_KPALVTR(sn);
         break;
      case SO_SENDBUF:
         *(uint16_t*) arg = getSn_TX_FSR(sn);
         break;
      case SO_RECVBUF:
         *(uint16_t*) arg = getSn_RX_RSR(sn);
         break;
      case SO_STATUS:
         *(uint8_t*) arg = getSn_SR(sn);
         break;
      case SO_REMAINSIZE:
         if(getSn_MR(sn) == Sn_MR_TCP)
            *(uint16_t*)arg = getSn_RX_RSR(sn);
         else
            *(uint16_t*)arg = sock_remained_size[sn];
         break;
      case SO_PACKINFO:
         if((getSn_MR(sn) == Sn_MR_TCP))
             return SOCKERR_SOCKMODE;
         *(uint8_t*)arg = sock_pack_info[sn];
         break;
      default:
         return SOCKERR_SOCKOPT;
   }
   return SOCK_OK;
}
