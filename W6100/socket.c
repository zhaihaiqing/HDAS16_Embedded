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


#include <stdio.h>
#include "socket.h"
#include "w6100.h"
#include "main.h"

#define SOCK_ANY_PORT_NUM  0x0400

static uint16_t sock_any_port = SOCK_ANY_PORT_NUM;
static uint16_t sock_io_mode = 0;
static uint16_t sock_is_sending = 0;
static datasize_t sock_remained_size[_WIZCHIP_SOCK_NUM_] = {0,0,};
static uint8_t  sock_pack_info[_WIZCHIP_SOCK_NUM_] = {0,};


#define CHECK_SOCKNUM()                                    \
   do{                                                     \
      if(sn >= _WIZCHIP_SOCK_NUM_) return SOCKERR_SOCKNUM; \
   }while(0);

#define CHECK_SOCKMODE(mode)                                      \
   do{                                                            \
      if((getSn_MR(sn) & 0x0F) != mode) return SOCKERR_SOCKMODE;  \
   }while(0);

#define CHECK_TCPMODE()                                           \
   do{                                                            \
      if((getSn_MR(sn) & 0x03) != 0x01) return SOCKERR_SOCKMODE;  \
   }while(0);

#define CHECK_UDPMODE()                                           \
   do{                                                            \
      if((getSn_MR(sn) & 0x03) != 0x02) return SOCKERR_SOCKMODE;  \
   }while(0);

#define CHECK_IPMODE()                                            \
   do{                                                            \
      if((getSn_MR(sn) & 0x07) != 0x03) return SOCKERR_SOCKMODE;  \
   }while(0);

#define CHECK_DGRAMMODE()                                         \
   do{                                                            \
      if(getSn_MR(sn) == Sn_MR_CLOSED) return SOCKERR_SOCKMODE;   \
      if((getSn_MR(sn) & 0x03) == 0x01) return SOCKERR_SOCKMODE;  \
   }while(0);


#define CHECK_SOCKINIT()                                       \
   do{                                                         \
      if((getSn_SR(sn) != SOCK_INIT)) return SOCKERR_SOCKINIT; \
   }while(0);                

#define CHECK_SOCKDATA()                   \
   do{                                     \
      if(len == 0) return SOCKERR_DATALEN; \
   }while(0);     
 
#define CHECK_IPZERO(addr, addrlen)                                  \
   do{                                                               \
      uint16_t ipzero= 0; uint8_t i;                                           \
      for( i=0; i<addrlen; i++)  ipzero += (uint16_t)addr[i]; \
      if (ipzero == 0) return SOCKERR_IPINVALID;                     \
   }while(0);



int8_t socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag)
{ 
   uint8_t taddr[16];
//   uint16_t local_port=0;
   CHECK_SOCKNUM(); 
   switch (protocol & 0x0F)
   {
      case Sn_MR_TCP4 :           // 2020-03-11   IPV4模式下的TCP 
         getSIPR(taddr);
         CHECK_IPZERO(taddr, 4);
			   setSn_KPALVTR(sn,1);     // 2020-03-11   TCP模式设置心跳
         break;
      case Sn_MR_TCP6 :           // 2020-03-11   IPV6模式下的TCP  
         getLLAR(taddr);
         CHECK_IPZERO(taddr, 16); 
			   setSn_KPALVTR(sn,1);     // 2020-03-11   TCP模式设置心跳
         //getGUAR(taddr);
         //CHECK_IPZERO(taddr, 16);
         break;
      case Sn_MR_TCPD :           // 2020-03-11   IPV4、IPV6模式下的TCP 
         getSIPR(taddr);
         CHECK_IPZERO(taddr, 4);
         getLLAR(taddr);
         CHECK_IPZERO(taddr, 16);  
			   setSn_KPALVTR(sn,1);     // 2020-03-11   TCP模式设置心跳
         //getGUAR(taddr);
         //CHECK_IPZERO(taddr, 16);
         break;
      case Sn_MR_UDP :
      case Sn_MR_UDP6 :
      case Sn_MR_UDPD :
      case Sn_MR_MACRAW :
      case Sn_MR_IPRAW4 :
      case Sn_MR_IPRAW6 :
         break; 
      default :
        return SOCKERR_SOCKMODE;
   } 

   if((flag & 0x04)) return SOCKERR_SOCKFLAG;
   if(flag != 0)
   {
      switch(protocol)
      {
         case Sn_MR_MACRAW:
            if((flag & (SF_DHA_MANUAL | SF_FORCE_ARP)) != 0)
            	return SOCKERR_SOCKFLAG;
            break;
         case Sn_MR_TCP4:
         case Sn_MR_TCP6:
         case Sn_MR_TCPD:     
            if((flag & (SF_MULTI_ENABLE | SF_UNI_BLOCK)) !=0)
            	return SOCKERR_SOCKFLAG;
            break;
         case Sn_MR_IPRAW4:
         case Sn_MR_IPRAW6:
            if(flag !=0)
            	return SOCKERR_SOCKFLAG;
            break;
         default:
            break;
      }
   }
   close(sn);
	
   setSn_MR(sn,(protocol | (flag & 0xF0))); 
   setSn_MR2(sn, flag & 0x03);  
   if(!port)
   {
      port = sock_any_port++;
      if(sock_any_port == 0xFFF0) sock_any_port = SOCK_ANY_PORT_NUM;
   }
   setSn_PORTR(sn,port);
   setSn_CR(sn,Sn_CR_OPEN);

   while(getSn_CR(sn));

   sock_io_mode &= ~(1 <<sn);
   sock_io_mode |= ((flag & (SF_IO_NONBLOCK>>3)) << sn);
   sock_is_sending &= ~(1<<sn);
   sock_remained_size[sn] = 0;
   sock_pack_info[sn] = PACK_NONE;

   while(getSn_SR(sn) == SOCK_CLOSED) ;
   return sn;
}  


int8_t close(uint8_t sn)
{
   CHECK_SOCKNUM();
   setSn_CR(sn,Sn_CR_CLOSE);
   /* wait to process the command... */
   while( getSn_CR(sn) );
   /* clear all interrupt of SOCKETn. */
   setSn_IRCLR(sn, 0xFF);
   /* Release the sock_io_mode of SOCKETn. */
   sock_io_mode &= ~(1<<sn); 
   sock_remained_size[sn] = 0;
   sock_is_sending &= ~(1<<sn);
   sock_pack_info[sn] = PACK_NONE;
   while(getSn_SR(sn) != SOCK_CLOSED);
   return SOCK_OK;
}


int8_t listen(uint8_t sn)
{
   CHECK_SOCKNUM();
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


int8_t connect(uint8_t sn, uint8_t * addr, uint16_t port, uint8_t addrlen)  //2020-03-12
{ 

   CHECK_SOCKNUM();
   CHECK_TCPMODE();
   CHECK_SOCKINIT();
  
   CHECK_IPZERO(addr, addrlen);
   if(port == 0)
	   return SOCKERR_PORTZERO;

   setSn_DPORTR(sn, port);
  
   if (addrlen == 16)     // addrlen=16, Sn_MR_TCP6(1001), Sn_MR_TCPD(1101))
   {
      if( getSn_MR(sn) & 0x08)  
      {
         setSn_DIP6R(sn,addr);
         setSn_CR(sn,Sn_CR_CONNECT6);
      }
      else return SOCKERR_SOCKMODE;
   } 
   else           // addrlen=4, Sn_MR_TCP4(0001), Sn_MR_TCPD(1101)
   {
      if(getSn_MR(sn) == Sn_MR_TCP6) return SOCKERR_SOCKMODE;
      setSn_DIPR(sn,addr);
      setSn_CR(sn,Sn_CR_CONNECT);
   }
   while(getSn_CR(sn));
   return SOCK_OK;
}

int8_t disconnect(uint8_t sn)
{
   CHECK_SOCKNUM();
   CHECK_TCPMODE();
   if(getSn_SR(sn) != SOCK_CLOSED)
   {
      setSn_CR(sn,Sn_CR_DISCON);
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
   }
   return SOCK_OK;
}


datasize_t send(uint8_t sn, uint8_t * buf, datasize_t len)
{
   uint8_t tmp=0;
   datasize_t freesize=0;
   /* 
    * The below codes can be omitted for optmization of speed
    */
   //CHECK_SOCKNUM();
   //CHECK_TCPMODE(Sn_MR_TCP4);
   /************/

   freesize = getSn_TxMAX(sn);
   if (len > freesize) len = freesize; // check size not to exceed MAX size.
   while(1)
   {
      freesize = (datasize_t)getSn_TX_FSR(sn);
      tmp = getSn_SR(sn);
      if ((tmp != SOCK_ESTABLISHED) && (tmp != SOCK_CLOSE_WAIT))
      {
         if(tmp == SOCK_CLOSED) close(sn);
         return SOCKERR_SOCKSTATUS;
      }
      if(len <= freesize) break;
      if( sock_io_mode & (1<<sn) ) return SOCK_BUSY;  
   }
   wiz_send_data(sn, buf, len);
   if(sock_is_sending & (1<<sn))
   {
      while ( !(getSn_IR(sn) & Sn_IR_SENDOK) )
      {    
         tmp = getSn_SR(sn);
         if ((tmp != SOCK_ESTABLISHED) && (tmp != SOCK_CLOSE_WAIT) )
         {
            if( (tmp == SOCK_CLOSED) || (getSn_IR(sn) & Sn_IR_TIMEOUT) ) close(sn);
            return SOCKERR_SOCKSTATUS;
         }
         if(sock_io_mode & (1<<sn)) return SOCK_BUSY;
      } 
      setSn_IRCLR(sn, Sn_IR_SENDOK);
   }
   setSn_CR(sn,Sn_CR_SEND);
 
   while(getSn_CR(sn));   // wait to process the command...
   sock_is_sending |= (1<<sn);
 
   return len;
}


datasize_t recv(uint8_t sn, uint8_t * buf, datasize_t len)
{
   uint8_t  tmp = 0;
   datasize_t recvsize = 0;
   /* 
    * The below codes can be omitted for optmization of speed
    */
   //CHECK_SOCKNUM();
   //CHECK_TCPMODE();
   //CHECK_SOCKDATA();
   /************/
 
   recvsize = getSn_RxMAX(sn); 
   if(recvsize < len) len = recvsize;
   while(1)
   {
      recvsize = (datasize_t)getSn_RX_RSR(sn);
      tmp = getSn_SR(sn);
      if (tmp != SOCK_ESTABLISHED && tmp != SOCK_CLOSE_WAIT)
      {
         if(tmp == SOCK_CLOSED) close(sn);
         return SOCKERR_SOCKSTATUS;
      }
      if(recvsize) break;
      if(sock_io_mode & (1<<sn)) return SOCK_BUSY;
   }
   if(recvsize < len) len = recvsize;
   wiz_recv_data(sn, buf, len); 
   setSn_CR(sn,Sn_CR_RECV); 
   while(getSn_CR(sn));  
   return len;
}


datasize_t sendto(uint8_t sn, uint8_t * buf, datasize_t len, uint8_t * addr, uint16_t port, uint8_t addrlen)
{
   uint8_t tmp = 0;
   uint8_t tcmd = 0;
   uint16_t freesize = 0;
   /* 
    * The below codes can be omitted for optmization of speed
    */
   //CHECK_SOCKNUM();
   //CHECK_DGRAMMODE();
   /************/
   tmp = getSn_MR(sn);
   if(tmp != Sn_MR_MACRAW)
   {
       if (addrlen == 16)      // addrlen=16, Sn_MR_UDP6(1010), Sn_MR_UDPD(1110)), IPRAW6(1011)
      {
         if( tmp & 0x08)  
         {
            setSn_DIP6R(sn,addr);
            tcmd = Sn_CR_SEND6;
         }
         else return SOCKERR_SOCKMODE;
      } 
      else if(addrlen == 4)      // addrlen=4, Sn_MR_UDP4(0010), Sn_MR_UDPD(1110), IPRAW4(0011)
      {
         if(tmp == Sn_MR_UDP6 || tmp == Sn_MR_IPRAW6) return SOCKERR_SOCKMODE;
         setSn_DIPR(sn,addr);
         tcmd = Sn_CR_SEND;
      }
      else return SOCKERR_IPINVALID;
   }
   if(tmp & 0x02)     // Sn_MR_UPD4(0010), Sn_MR_UDP6(1010), Sn_MR_UDPD(1110)
   {
      if(port){ setSn_DPORTR(sn, port);}
      else   return SOCKERR_PORTZERO;
   }
  
   freesize = getSn_TxMAX(sn);
   if (len > freesize) len = freesize; // check size not to exceed MAX size.
  
   while(1)
   {
      freesize = getSn_TX_FSR(sn);
      if(getSn_SR(sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
      if(len <= freesize) break;
      if( sock_io_mode & (1<<sn) ) return SOCK_BUSY;  
   };
   wiz_send_data(sn, buf, len);
   setSn_CR(sn,tcmd);
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
				printf("timeout");
         return SOCKERR_TIMEOUT;
      }
   }  
   return (int32_t)len;
}


datasize_t recvfrom(uint8_t sn, uint8_t * buf, datasize_t len, uint8_t * addr, uint16_t *port, uint8_t *addrlen)
{ 
   uint8_t  head[2];
   datasize_t pack_len=0;
  
   /* 
    * The below codes can be omitted for optmization of speed
    */
   //CHECK_SOCKNUM();
   //CHECK_DGRAMMODE();
   //CHECK_SOCKDATA();
   /************/
  
   if(sock_remained_size[sn] == 0)
   {
      while(1)
      {
         pack_len = getSn_RX_RSR(sn);
         if(getSn_SR(sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
         if(pack_len != 0)
         {
            sock_pack_info[sn] = PACK_NONE;
            break;
         } 
         if( sock_io_mode & (1<<sn) ) return SOCK_BUSY;
      };
      /* First read 2 bytes of PACKET INFO in SOCKETn RX buffer*/
      wiz_recv_data(sn, head, 2);  
      setSn_CR(sn,Sn_CR_RECV);
      while(getSn_CR(sn));
      pack_len = head[0] & 0x07;
      pack_len = (pack_len << 8) + head[1];
    
      switch (getSn_MR(sn) & 0x0F)
      {
         case Sn_MR_UDP4 :
         case Sn_MR_UDP6:
         case Sn_MR_UDPD:
         case Sn_MR_IPRAW6:
         case Sn_MR_IPRAW4 : 
            if(addr == 0) return SOCKERR_ARG;
            sock_pack_info[sn] = head[0] & 0xF8;
            if(sock_pack_info[sn] & PACK_IPv6) *addrlen = 16;
            else *addrlen = 4;
            wiz_recv_data(sn, addr, *addrlen);
            setSn_CR(sn,Sn_CR_RECV);
            while(getSn_CR(sn));
            break;
         case Sn_MR_MACRAW :
            if(pack_len > 1514) 
            {
               close(sn);
               return SOCKFATAL_PACKLEN;
            }
            break; 
         default:
            return SOCKERR_SOCKMODE;
            //break;
      }
      sock_remained_size[sn] = pack_len;
      sock_pack_info[sn] |= PACK_FIRST;
      if((getSn_MR(sn) & 0x03) == 0x02)  // Sn_MR_UDP4(0010), Sn_MR_UDP6(1010), Sn_MR_UDPD(1110)
      {
         /* Read port number of PACKET INFO in SOCKETn RX buffer */
         if(port==0) return SOCKERR_ARG;
         wiz_recv_data(sn, head, 2);
         *port = ( ((((uint16_t)head[0])) << 8) + head[1] );
         setSn_CR(sn,Sn_CR_RECV);
         while(getSn_CR(sn));   
      }
   }   
   
   if   (len < sock_remained_size[sn]) pack_len = len;
   else pack_len = sock_remained_size[sn];    
   wiz_recv_data(sn, buf, pack_len);
   setSn_CR(sn,Sn_CR_RECV);  
   /* wait to process the command... */
   while(getSn_CR(sn)) ;
 
   sock_remained_size[sn] -= pack_len; 
   if(sock_remained_size[sn] != 0) sock_pack_info[sn] |= PACK_REMAINED; 
   else sock_pack_info[sn] |= PACK_COMPLETED; 
 
   return pack_len;
}



int8_t ctlsocket(uint8_t sn, ctlsock_type cstype, void* arg)
{
   uint8_t tmp = 0;
   CHECK_SOCKNUM();
   tmp = *((uint8_t*)arg); 
   switch(cstype)
   {
      case CS_SET_IOMODE:
         if(tmp == SOCK_IO_NONBLOCK)  sock_io_mode |= (1<<sn);
         else if(tmp == SOCK_IO_BLOCK) sock_io_mode &= ~(1<<sn);
         else return SOCKERR_ARG;
         break;
      case CS_GET_IOMODE: 
         *((uint8_t*)arg) = (uint8_t)((sock_io_mode >> sn) & 0x0001);
         break;
      case CS_GET_MAXTXBUF:
         *((datasize_t*)arg) = getSn_TxMAX(sn);
         break;
      case CS_GET_MAXRXBUF:  
         *((datasize_t*)arg) = getSn_RxMAX(sn);
         break;
      case CS_CLR_INTERRUPT:
         if( tmp > SIK_ALL) return SOCKERR_ARG;
         setSn_IRCLR(sn,tmp);
         break;
      case CS_GET_INTERRUPT:
         *((uint8_t*)arg) = getSn_IR(sn);
         break;
      case CS_SET_INTMASK:
         if( tmp > SIK_ALL) return SOCKERR_ARG;
         setSn_IMR(sn,tmp);
         break;
      case CS_GET_INTMASK:
         *((uint8_t*)arg) = getSn_IMR(sn);
         break;
      case CS_SET_PREFER:
    	  if((tmp & 0x03) == 0x01) return SOCKERR_ARG;
    	  setSn_PSR(sn, tmp);
    	  break;
      case CS_GET_PREFER:
    	  *(uint8_t*) arg = getSn_PSR(sn);
    	  break;
      default:
         return SOCKERR_ARG;
   }
   return SOCK_OK;
}

int8_t setsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
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
         if(((wiz_IPAddress*)arg)->len == 16) setSn_DIP6R(sn, ((wiz_IPAddress*)arg)->ip);
         else           setSn_DIPR(sn, ((wiz_IPAddress*)arg)->ip);
         break;
      case SO_DESTPORT:
         setSn_DPORTR(sn, *(uint16_t*)arg);
         break;
      case SO_KEEPALIVESEND:
         CHECK_TCPMODE();   
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
         CHECK_TCPMODE();
         setSn_KPALVTR(sn,*(uint8_t*)arg);
         break;   
      default:
         return SOCKERR_ARG;
   } 
   return SOCK_OK;
}

int8_t getsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
   CHECK_SOCKNUM();
   switch(sotype)
   {
      case SO_FLAG:
         *(uint8_t*)arg = (getSn_MR(sn) & 0xF0) | (getSn_MR2(sn)) | ((uint8_t)(((sock_io_mode >> sn) & 0x0001) << 3));
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
         CHECK_TCPMODE();
         if(getSn_ESR(sn) & TCPSOCK_MODE) //IPv6 ?
         {
            getSn_DIP6R(sn, ((wiz_IPAddress*)arg)->ip);
            ((wiz_IPAddress*)arg)->len = 16;
         } 
         else
         {
            getSn_DIPR(sn, ((wiz_IPAddress*)arg)->ip);
            ((wiz_IPAddress*)arg)->len = 4;
         } 
         break;
      case SO_DESTPORT:  
         *(uint16_t*) arg = getSn_DPORTR(sn);
         break; 
      case SO_KEEPALIVEAUTO:
         CHECK_TCPMODE();
         *(uint16_t*) arg = getSn_KPALVTR(sn);
         break;
      case SO_SENDBUF:
         *(datasize_t*) arg = getSn_TX_FSR(sn);
         break;
      case SO_RECVBUF:
         *(datasize_t*) arg = getSn_RX_RSR(sn);
         break;
      case SO_STATUS:
         *(uint8_t*) arg = getSn_SR(sn);
         break;
      case SO_EXTSTATUS:
         CHECK_TCPMODE();
         *(uint8_t*) arg = getSn_ESR(sn) & 0x07;
         break;
      case SO_REMAINSIZE:
         if(getSn_MR(sn)==SOCK_CLOSED) return SOCKERR_SOCKSTATUS;
         if(getSn_MR(sn) & 0x01)   *(uint16_t*)arg = getSn_RX_RSR(sn);
         else                      *(uint16_t*)arg = sock_remained_size[sn];
         break;
      case SO_PACKINFO:
         if(getSn_MR(sn)==SOCK_CLOSED) return SOCKERR_SOCKSTATUS;
         if(getSn_MR(sn) & 0x01)       return SOCKERR_SOCKMODE;
         else *(uint8_t*)arg = sock_pack_info[sn];
         break;
      case SO_MODE:
         *(uint8_t*) arg = 0x0F & getSn_MR(sn);
         break;
      default:
         return SOCKERR_SOCKOPT;
   }
   return SOCK_OK;
}

int16_t peeksockmsg(uint8_t sn, uint8_t* submsg, uint16_t subsize)
{
   uint32_t rx_ptr = 0;
   uint16_t i = 0, sub_idx = 0;

   if( (getSn_RX_RSR(sn) > 0) && (subsize > 0) )
   {
       rx_ptr = ((uint32_t)getSn_RX_RD(sn) << 8)  + WIZCHIP_RXBUF_BLOCK(sn);
       sub_idx = 0;
       for(i = 0; i < getSn_RX_RSR(sn) ; i++)
       {
          if(WIZCHIP_READ(rx_ptr) == submsg[sub_idx])
          {
              sub_idx++;
              if(sub_idx == subsize) return (i + 1 - sub_idx);
          }
          else sub_idx = 0;
          rx_ptr = WIZCHIP_OFFSET_INC(rx_ptr,1);
       }
   }
   return -1;
}





/*********
*@brief: TCP Server
*
**********************/
int32_t tcps_senddata_ipv4(uint8_t sn, uint16_t port, uint8_t* buf)  //2020-03-11
{
    int32_t ret;
    int8_t status,inter;
    uint8_t tmp = 0;
    datasize_t received_size;
    uint8_t arg_tmp8;
      
		getsockopt(sn, SO_STATUS, &status);  // 获取socket sn的状态  2020-03-11
		switch(status)
    {
					
      case SOCK_ESTABLISHED :   // 处于SOCK_ESTABLISHED状态，可以进行收发  2020-03-11
         ctlsocket(sn,CS_GET_INTERRUPT,&inter);   // 读取中断2020-03-11 inter=getSn_IR(sn);  等效可替换
         if(inter & Sn_IR_CON)       // 连接中断产生表示，TCP连接成功，中断下，其它内容可根据需要添加或删除 2020-03-11 
         {
           arg_tmp8 = Sn_IR_CON;
           ctlsocket(sn,CS_CLR_INTERRUPT,&arg_tmp8); // 回写，清除中断标记，进入连接中断后需要清除。2020-03-11
         }
			
				if(AD_flag==0x01)//判断AD是否采集到数据
				{
					AD_flag=0;
					
					//tick_count=HAL_GetTick();
					
					ret = send(sn, (uint8_t *)AD_buff, ONE_PACKET_DAT_SIZE*ONE_FRAME_DP_NUM+8);
					
					//tick_count=HAL_GetTick()-tick_count;
		
					//log_info("tick:%d\r\n",tick_count);
					
				}
				 
//				 getsockopt(sn, SO_RECVBUF, &received_size); // 判断socket有没有接受到数据：同 received_size=getSn_RX_RSR(sn);等效可替换 2020-03-11
//				 if(received_size>0)
//				 {	 
//           ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11	 
//					 ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的数据 2020-03-11
//				 } 	 
       break;
						
						
						
       case SOCK_CLOSE_WAIT :   // 该状态即将关闭， 但还处于连接状态，先处理完收发后，再回DISCON命令，完成4次挥手。

				 if(AD_flag==0x01)//判断AD是否采集到数据
				{
					AD_flag=0;
					ret = send(sn, (uint8_t *)AD_buff, ONE_PACKET_DAT_SIZE*ONE_FRAME_DP_NUM+8);
				}
				
					
//         ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11
//			   if(ret>0)  //buf中的数据大于0，可以启动发送
//				 {	 
//						ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的数据
//				 } 	
				
          if((ret = disconnect(sn)) != SOCK_OK) return ret;   //  调用DISCON命令，完成4次挥手。2020-03-11

          break;
				 
       case SOCK_INIT :    //SOCK初始完成 ，配置为TCP状态成功 2020-03-11
          if( (ret = listen(sn)) != SOCK_OK)  // 配置为服务器模式 2020-03-11
					return ret;
         break;
					
       case SOCK_CLOSED:     // 处于CLOSE状态，需要配置SOCKET函数  2020-03-11	  
         tmp = socket(sn, Sn_MR_TCP4, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11  
				 printf("\r\nW6100 IPV4 Server port: %d\r\n",port); //打印配置socket 2020-03-11	
         if(tmp != sn)    /* reinitialize the socket */
         {
             #ifdef _LOOPBACK_DEBUG_
                prin24("%d : Fail to create socket.\r\n",sn);
             #endif
           return SOCKERR_SOCKNUM;
         }
         break;
					
        default:
          break;
				
       }
    return 1;
}






/*********
*@brief: TCP Server
*
**********************/
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port, uint8_t loopback_mode)  //2020-03-11
{
    int32_t ret;
    int8_t status,inter;
    uint8_t tmp = 0;
    datasize_t received_size;
    uint8_t arg_tmp8;
      
		getsockopt(sn, SO_STATUS, &status);  // 获取socket sn的状态  2020-03-11
		switch(status)
    {
					
      case SOCK_ESTABLISHED :   // 处于SOCK_ESTABLISHED状态，可以进行收发  2020-03-11
         ctlsocket(sn,CS_GET_INTERRUPT,&inter);   // 读取中断2020-03-11 inter=getSn_IR(sn);  等效可替换
         if(inter & Sn_IR_CON)       // 连接中断产生表示，TCP连接成功，中断下，其它内容可根据需要添加或删除 2020-03-11 
         {
           arg_tmp8 = Sn_IR_CON;
           ctlsocket(sn,CS_CLR_INTERRUPT,&arg_tmp8); // 回写，清除中断标记，进入连接中断后需要清除。2020-03-11
         }
			
       
				 getsockopt(sn, SO_RECVBUF, &received_size); // 判断socket有没有接受到数据：同 received_size=getSn_RX_RSR(sn);等效可替换 2020-03-11
				 if(received_size>0)
				 {	 
           ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11	 
					 ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的数据 2020-03-11
				 } 	 
       break;
						
						
						
       case SOCK_CLOSE_WAIT :   // 该状态即将关闭， 但还处于连接状态，先处理完收发后，再回DISCON命令，完成4次挥手。

         ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11
			   if(ret>0)  //buf中的数据大于0，可以启动发送
				 {	 
						ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的数据
				 } 	 
          if((ret = disconnect(sn)) != SOCK_OK) return ret;   //  调用DISCON命令，完成4次挥手。2020-03-11

          break;
				 
       case SOCK_INIT :    //SOCK初始完成 ，配置为TCP状态成功 2020-03-11
          if( (ret = listen(sn)) != SOCK_OK)  // 配置为服务器模式 2020-03-11
					return ret;
         break;
					
       case SOCK_CLOSED:     // 处于CLOSE状态，需要配置SOCKET函数  2020-03-11	
         switch(loopback_mode)  // 支持IPV4 ,IPV6  IPV4/IPV6 3种模式  
         {
             case AS_IPV4:
                tmp = socket(sn, Sn_MR_TCP4, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11  
						   	printf("\r\nW6100 IPV4 Server port: %d\r\n",port); //打印配置socket 2020-03-11
               break;
             case AS_IPV6:
                tmp = socket(sn, Sn_MR_TCP6, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11
						    printf("\r\nW6100 IPV6 Server port: %d\r\n",port); //打印配置socket 2020-03-1
               break;
             case AS_IPDUAL:
                tmp = socket(sn, Sn_MR_TCPD, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11
                printf("\r\nW6100 IPV4/IPV6 Server port: %d\r\n",port); //打印配置socket 2020-03-11						 
               break;
             default:
               break;
         }				
								
         if(tmp != sn)    /* reinitialize the socket */
         {
             #ifdef _LOOPBACK_DEBUG_
                prin24("%d : Fail to create socket.\r\n",sn);
             #endif
           return SOCKERR_SOCKNUM;
         }
         break;
					
        default:
          break;
				
       }
    return 1;
}



/***********
*@brief:TCP Client
*
*****************/
uint8_t connect_flag[8];  //2020-03-12
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint16_t port,uint8_t* destip, uint16_t destport, uint8_t loopback_mode) //2020-03-12
{

    int32_t ret; // return value for SOCK_ERRORs
    uint8_t status,inter;
    datasize_t received_size;
    uint8_t tmp = 0;
    uint8_t arg_tmp8;
	
	  getsockopt(sn, SO_STATUS, &status);  // 获取socket sn的状态  2020-03-12
    switch(status)
    {
      case SOCK_ESTABLISHED :   // 处于SOCK_ESTABLISHED状态，可以进行收发  2020-03-11
         ctlsocket(sn,CS_GET_INTERRUPT,&inter);   // 读取中断2020-03-11
         if(inter & Sn_IR_CON)       // 连接中断产生表示，TCP连接成功，中断下，其它内容可根据需要添加或删除 2020-03-11 
         {
           arg_tmp8 = Sn_IR_CON;
           ctlsocket(sn,CS_CLR_INTERRUPT,&arg_tmp8); // 回写，清除中断标记，进入连接中断后需要清除。2020-03-11
         }
				 
				    
				 getsockopt(sn, SO_RECVBUF, &received_size);// received_size=getSn_RX_RSR(sn); 等效可替换，获取缓存是否收到数据?020-03-11?
				 if(received_size>0)
				 {	 
           ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11
					 ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的全部数据。
				 } 	 
       break;
								
       case SOCK_CLOSE_WAIT :   // 该状态即将关闭， 但还处于连接状态，先处理完收发后，再回DISCON命令，完成4次挥手。

         ret = recv(sn, buf, received_size);  //将缓存中收到的数据放入buf中received_size是希望收到的数据，返回ret是实际收到的数据 2020-03-11
			   if(ret>0)  //buf中的数据大于0，可以启动发送
				 {	 
						ret = send(sn, buf, ret);   // 发送函数，发送buf中长度为ret的数据
				 } 	 
          if((ret = disconnect(sn)) != SOCK_OK) return ret;   //  调用DISCON命令，完成4次挥手。2020-03-11
        break;

    case SOCK_INIT :
			 if(connect_flag[sn]==0)   //此状态下，如果尚未发起连接，可以发起连接，如已发起连接不用再连，直接等待结果?  //2020-03-12
			 {	 
				 if(loopback_mode == AS_IPV4)// IPV4模式 //2020-03-12
					 ret = connect(sn, destip, destport, 4); /* 连接服务器（s0cket号，服务器IP,服务器端口，地址4位） */  //2020-03-12
				 else if(loopback_mode == AS_IPV6) // IPV6模式?/2020-03-12
					 ret = connect(sn, destip, destport, 16); /*连接服务器（socket号，服务器IP，服务器端口，地址16位） */  //2020-03-12
				 
				 connect_flag[sn]=1;   //已发出连接动作，标记为1.
       }
			  break;
			 
    case SOCK_CLOSED:
			   connect_flag[sn]=0;    // 尚未连接发起连接，将标记清0.
         switch(loopback_mode)  // 支持IPV4 ,IPV6  IPV4/IPV6 3种模式  
         {
             case AS_IPV4:
                tmp = socket(sn, Sn_MR_TCP4, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11 
						    printf("\r\nW6100 IPV4 client port:%d\\r\n Server IP : %d.%d.%d.%d\r\nServer port: %d\r\n",port, destip[0],destip[1],destip[2],destip[3],destport);
						    break;
						 
             case AS_IPV6:
                tmp = socket(sn, Sn_MR_TCP6, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-11
                printf("\r\nW6100 IPV6 client port:%d\r\n",port);	     // 打印目的 相关参数
                printf("Server IP: %04X:%04X", ((uint16_t)destip[0] << 8) | ((uint16_t)destip[1]),
    		        ((uint16_t)destip[2] << 8) | ((uint16_t)destip[3]));
                printf(":%04X:%04X", ((uint16_t)destip[4] << 8) | ((uint16_t)destip[5]),
    		        ((uint16_t)destip[6] << 8) | ((uint16_t)destip[7]));
                printf(":%04X:%04X", ((uint16_t)destip[8] << 8) | ((uint16_t)destip[9]),
    		        ((uint16_t)destip[10] << 8) | ((uint16_t)destip[11]));
                printf(":%04X:%04X\r\n", ((uint16_t)destip[12] << 8) | ((uint16_t)destip[13]),
    		        ((uint16_t)destip[14] << 8) | ((uint16_t)destip[15]));
						    printf("Server IP port:%d\r\n",destport);	     // 打印目的 相关参数		
               break;
						 
             case AS_IPDUAL:  // 
                tmp = socket(sn, Sn_MR_TCPD, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-13 
						    printf("\r\nW6100 IPV4/IPV6 client port:%d\r\n",port);	     // 打印目的 相关参数
						    printf("Please select specific IPV4 or IPV6 mode\r\n");	     // 打印目的 相关参数 2020-03-13 
               break;
             default:
               break;
         }				

        if(tmp != sn)
				{   
            #ifdef _LOOPBACK_DEBUG_
                printf("%d : Fail to create socket.\r\n",sn);
            #endif
            return SOCKERR_SOCKNUM;
        }
				return 0;
        
    default:
        break;
    }
    return 1;
}


/**********
*@brief: UDP
*
*
*****************/

int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port, uint8_t loopback_mode)  //2020-3-13
{
    uint8_t status;
    static uint8_t destip[16] ;
    static uint16_t destport;
    uint8_t addr_len;
    datasize_t ret;
    datasize_t received_size;

    getsockopt(sn, SO_STATUS,&status);  // 获取socket sn的状态  2020-03-13
    switch(status)
    {
    case SOCK_UDP:   //UDP状态，可以进行收发。
        getsockopt(sn, SO_RECVBUF, &received_size);  // received_size=getSn_RX_RSR(sn); 等效可替换，获取缓存是否收到数据?2020-03-13
        if(received_size>0)  //收到数据，可以接受 2020-03-13
        {
					 //ret实际接受到的数据=(socket号，存数据的BUF,缓存总数据长度，目的IP（谁发来的），目的端口，addr_len为4表示IPV4,addr_len为16表示IPV6).2020-03-13   
           ret = recvfrom(sn, buf, received_size, (uint8_t*)&destip, (uint16_t*)&destport, &addr_len);
					 // 用socket sn 将buf中 ret长的数据发向目的IP(destip)目的端口（destport）,IPV4是 addr_len 为4，IPV6时addr_len即地址为16位。2020-03-13 
           ret = sendto(sn, buf, ret, destip, destport, addr_len);
        }
        break;
    case SOCK_CLOSED:

        switch(loopback_mode)
        {
        case AS_IPV4:
           socket(sn,Sn_MR_UDP4, port,SOCK_IO_NONBLOCK);   //配置socket 2020-03-13 
           printf("\r\nW6100 IPV4 UDP port: %d\r\n",port); //打印配置socket 2020-03-13				
           break;
        case AS_IPV6:
           socket(sn,Sn_MR_UDP6, port, SOCK_IO_NONBLOCK);  //配置socket 2020-03-13 
           printf("\r\nW6100 IPV6 UDP port: %d\r\n",port); //打印配置socket 2020-03-13					
           break;
        case AS_IPDUAL:
            socket(sn,Sn_MR_UDPD, port, SOCK_IO_NONBLOCK); //配置socket 2020-03-13
            printf("\r\nW6100 IPV4/IPV6 UDP port: %d\r\n",port); //打印配置socket 2020-03-13					
            break;
        }
    }

    return 0;
}


