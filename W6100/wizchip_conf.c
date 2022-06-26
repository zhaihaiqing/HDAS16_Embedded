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


#include <stddef.h> // for use the type - ptrdiff_t


#include "wizchip_conf.h"
#include "w6100.h"
/**
 * @brief Default function to enter the critical section for @ref _WIZCHIP_.
 * @details @ref wizchip_cris_enter() provides the default protection while @ref _WIZCHIP_ is accessed. \n
 *          but it is null function.
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_cris_cbfunc().
 * @sa wizchip_cris_exit()
 */
void wizchip_cris_enter(void)   {}

/**
 * @brief Default function to exit the critical section for @ref _WIZCHIP_.
 * @details @ref wizchip_cris_exit provides the default protection while @ref _WIZCHIP_ is accessed\n
 *          but it is null function.
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_cris_cbfunc().
 * @sa wizchip_cris_enter()
 */
void wizchip_cris_exit(void)   {}

/**
 * @brief Default function to select @ref _WIZCHIP_.
 * @details @ref wizchip_cs_select() provides the default selection @ref _WIZCHIP_,\n
 *          but it is null function.
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_cs_cbfunc().
 * @sa wizchip_cs_deselect()
 */
void wizchip_cs_select(void)   {}

/**
 * @brief Default function to de-select @ref _WIZCHIP_.
 * @details @ref wizchip_cs_deselect() provides the default de-selection @ref _WIZCHIP_, \n
 *          but it is null function.
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_cs_cbfunc().
 * @sa wizchip_cs_select()
 */
void wizchip_cs_deselect(void)   {}


/// @cond DOXY_APPLY_CODE
#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_)
/// @endcond

/**
 * @brief Default function to read one @ref iodata_t data by using BUS interface
 * @details @ref wizchip_bus_read() provides the default read one @ref iodata_t data from BUS of @ref _WIZCHIP_.
 * @param AddrSel It specifies the address of register to be read.
 * @return @ref iodata_t  
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_bus_cbfunc().
 * @sa wizchip_bus_write()
 */
iodata_t wizchip_bus_read(uint32_t AddrSel) { return * ((volatile iodata_t *)((ptrdiff_t) AddrSel)); }

/**
 * @brief Default function to write one @ref iodata_t data by using BUS interface.
 * @details @ref wizchip_bus_write() provides the default write one @ref iodata_t data to BUS of @ref _WIZCHIP_.
 * @param AddrSel It specifies the address of register to be written.
 * @param wb @ref iodata_t data to be written
 * @return void
 * @note It can be overwritten with your function or register your functions by calling @ref reg_wizchip_bus_cbfunc().
 * @sa wizchip_bus_read()
 */
void wizchip_bus_write(uint32_t AddrSel, iodata_t wb)  { *((volatile iodata_t*)((ptrdiff_t)AddrSel)) = wb; }





/// @cond DOXY_APPLY_CODE
#endif
/// @endcond


/// @cond DOXY_APPLY_CODE
#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_)
/// @endcond

/**
 * @brief Default function to read one byte by using SPI interface.
 * @details @ref wizchip_spi_read() provides the default read one byte data from SPI of @ref _WIZCHIP_,\n
 *          but it is null function.
 * @return uint8_t 
 * @todo It should be overwritten with your function or register your functions by calling @ref reg_wizchip_spi_cbfunc().
 */
uint8_t wizchip_spi_read(void)        {return 0;}

/**
 * @brief Default function to write one byte by using SPI interface.
 * @details @ref wizchip_spi_write() provides the default write one byte data to SPI of @ref _WIZCHIP_,\n
 *          but it is null function.
 * @param wb uint8_t data to be written
 * @return void 
 * @todo It should be overwritten with your function or register your functions by calling @ref reg_wizchip_spi_cbfunc().
 */
void wizchip_spi_write(uint8_t wb) {}




/// @cond DOXY_APPLY_CODE
#endif
/// @endcond





static uint8_t      _DNS_[4];      ///< DNS server IPv4 address
static uint8_t      _DNS6_[16];    ///< DSN server IPv6 address
static ipconf_mode  _IPMODE_;      ///< IP configuration mode



int8_t ctlwizchip(ctlwizchip_type cwtype, void* arg)
{
   uint8_t tmp = *(uint8_t*) arg;

   uint8_t* ptmp[2] = {0,0};
   switch(cwtype)
   {
      case CW_SYS_LOCK:
         if(tmp & SYS_CHIP_LOCK) CHIPLOCK();
         if(tmp & SYS_NET_LOCK)  NETLOCK();
         if(tmp & SYS_PHY_LOCK)  PHYLOCK();
         break;
      case CW_SYS_UNLOCK:
         if(tmp & SYS_CHIP_LOCK) CHIPUNLOCK();
         if(tmp & SYS_NET_LOCK)  NETUNLOCK();
         if(tmp & SYS_PHY_LOCK)  PHYUNLOCK();
         break;
      case CW_GET_SYSLOCK:
         *(uint8_t*)arg = getSYSR() >> 5;
         break;
      case CW_RESET_WIZCHIP:
         wizchip_sw_reset();
         break;
      case CW_INIT_WIZCHIP:
         if(arg != 0) 
         {
            ptmp[0] = (uint8_t*)arg;
            ptmp[1] = ptmp[0] + _WIZCHIP_SOCK_NUM_;
         }
         return wizchip_init(ptmp[0], ptmp[1]);
      case CW_GET_INTERRUPT:
        *(intr_kind*)arg = wizchip_getinterrupt();
         break;
      case CW_CLR_INTERRUPT:
         wizchip_clrinterrupt(*(intr_kind*)arg);
         break;
      case CW_SET_INTRMASK:
         wizchip_setinterruptmask(*(intr_kind*)arg);
         break;         
      case CW_GET_INTRMASK:
         *(intr_kind*)arg = wizchip_getinterruptmask();
         break;   
      case CW_SET_INTRTIME:
         setINTPTMR(*(uint16_t*)arg);
         break;
      case CW_GET_INTRTIME:
         *(uint16_t*)arg = getINTPTMR();
         break;   
      case CW_SET_IEN:
         tmp = getSYCR1();
         if(*(uint8_t*)arg == 1) setSYCR1(tmp | SYCR1_IEN);
         else                      setSYCR1(tmp & ~SYCR1_IEN);
         break;
      case CW_GET_IEN:
         *((uint8_t*)arg) = getSYCR1() >> 7;
         break;
      case CW_GET_ID:
    //     ((uint8_t*)arg)[0] = WIZCHIP.id[0];
    //     ((uint8_t*)arg)[1] = WIZCHIP.id[1];
    //     ((uint8_t*)arg)[2] = WIZCHIP.id[2];
    //     ((uint8_t*)arg)[3] = WIZCHIP.id[3];
    //     ((uint8_t*)arg)[4] = WIZCHIP.id[4];
   //      ((uint8_t*)arg)[5] = 0;
         break;   
      case CW_GET_VER:
         *(uint16_t*)arg = getVER();
         break;
      case CW_SET_SYSCLK:
         tmp = getSYCR1();
         if(*(uint8_t*)arg == SYSCLK_100MHZ)     setSYCR1(tmp & ~SYCR1_CLKSEL);
         else if(*(uint8_t*)arg == SYSCLK_25MHZ) setSYCR1(tmp |  SYCR1_CLKSEL);
         break;
      case CW_GET_SYSCLK:
         *(uint8_t*)arg = (getSYCR1() & SYCR1_CLKSEL);
         break;
      case CW_RESET_PHY:
         wizphy_reset();
         break;
      case CW_SET_PHYCONF:
         wizphy_setphyconf((wiz_PhyConf*)arg);
         break;
      case CW_GET_PHYCONF:
         wizphy_getphyconf((wiz_PhyConf*)arg);
         break;
      case CW_GET_PHYSTATUS:
         wizphy_getphystatus((wiz_PhyConf*)arg);
         break;
      case CW_SET_PHYPOWMODE:
         wizphy_setphypmode(tmp);
         break;
      case CW_GET_PHYPOWMODE:
         *(uint8_t*)arg = wizphy_getphypmode();
         break;
      case CW_GET_PHYLINK:
         *(uint8_t*)arg = wizphy_getphylink();
         break;
      default:
         return -1;
   }
   return 0;
}

int8_t ctlnetwork(ctlnetwork_type cntype, void* arg)
{
   switch(cntype)
   {
      case CN_SET_NETINFO:
         wizchip_setnetinfo((wiz_NetInfo*)arg);
         break;
      case CN_GET_NETINFO:
         wizchip_getnetinfo((wiz_NetInfo*)arg);
         break;
      case CN_SET_NETMODE:
         wizchip_setnetmode(*(netmode_type*)arg);
         break;
      case CN_GET_NETMODE:
         *(netmode_type*)arg = wizchip_getnetmode();
         break;
      case CN_SET_TIMEOUT:
         wizchip_settimeout((wiz_NetTimeout*)arg);
         break;
      case CN_GET_TIMEOUT:
         wizchip_gettimeout((wiz_NetTimeout*)arg);
         break;
      case CN_SET_PREFER:
         setSLPSR(*(uint8_t*)arg);
         break;
      case CN_GET_PREFER:
    	  *(uint8_t*)arg= getSLPSR();
         break;
      default:
         return -1;
   }
   return 0;
}

int8_t ctlnetservice(ctlnetservice_type cnstype, void* arg)
{
   switch(cnstype)
   {
      case CNS_ARP:
         return wizchip_arp((wiz_ARP*)arg);
      case CNS_PING:
         return wizchip_ping((wiz_PING*)arg);
      case CNS_DAD:
         return wizchip_dad((uint8_t*)arg);
      case CNS_SLAAC:
         return wizchip_slaac((wiz_Prefix*)arg);
      case CNS_UNSOL_NA:
         return wizchip_unsolicited();
      case CNS_GET_PREFIX:
         return wizchip_getprefix((wiz_Prefix*)arg);
      default:
          return -1;
          //break;
   }
}

void wizchip_sw_reset(void)
{
   uint8_t gw[4], sn[4], sip[4], gw6[16], sn6[16], lla[16], gua[16];
   uint8_t mac[6];
   uint8_t islock = getSYSR();

   CHIPUNLOCK();

   getSHAR(mac);
   getGAR(gw);  getSUBR(sn);  getSIPR(sip);	getGA6R(gw6); getSUB6R(sn6); getLLAR(lla); getGUAR(gua);
   setSYCR0(SYCR0_RST);
   getSYCR0(); // for delay

   NETUNLOCK();

   setSHAR(mac);
   setGAR(gw);
   setSUBR(sn);
   setSIPR(sip);
   setGA6R(gw6); 
   setSUB6R(sn6); 
   setLLAR(lla);
   setGUAR(gua);
   if(islock & SYSR_CHPL) CHIPLOCK();
   if(islock & SYSR_NETL) NETLOCK();
}

int8_t wizchip_init(uint8_t* txsize, uint8_t* rxsize)
{
   int8_t i;
   int8_t tmp = 0;
   wizchip_sw_reset();
   if(txsize)
   {
      tmp = 0;
			for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
			{
				tmp += txsize[i];		
				if(tmp > 2*_WIZCHIP_SOCK_NUM_) return -1;		
			}
			for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++) setSn_TXBUF_SIZE(i, txsize[i]);			
   }
   if(rxsize)
   {
      tmp = 0;
			for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
			{
				tmp += rxsize[i];		
				if(tmp > 2*_WIZCHIP_SOCK_NUM_) return -1;		
			}
			for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++) setSn_RXBUF_SIZE(i, rxsize[i]);
   }
   return 0;
}

void wizchip_clrinterrupt(intr_kind intr)
{
   int i;
   uint8_t ir   = (uint8_t)intr;
   uint8_t sir  = (uint8_t)((uint32_t)intr >> 8);
   uint8_t slir = (uint8_t)((uint32_t)intr >> 16);
   setIRCLR(ir);
   for(i=0; i<_WIZCHIP_SOCK_NUM_; i++)
   {
   	if(sir&(1<<i))   	setSn_IRCLR(i,0xFF);
   }
   setSLIRCLR(slir);
}

intr_kind wizchip_getinterrupt(void)
{
   uint32_t ret;
   ret = (((uint32_t)getSLIR())<<16) | (((uint32_t)getSIR())<<8) | (((uint32_t)getIR()));
   return (intr_kind)ret;
}

void wizchip_setinterruptmask(intr_kind intr)
{
   uint8_t imr   = (uint8_t)intr;
   uint8_t simr  = (uint8_t)((uint32_t)intr >> 8);
   uint8_t slimr = (uint8_t)((uint32_t)intr >> 16);
   setIMR(imr);
   setSIMR(simr);
   setSLIMR(slimr);
}

intr_kind wizchip_getinterruptmask(void)
{
   uint32_t ret;
   ret = (((uint32_t)getSLIMR())<<16) | (((uint32_t)getSIMR())<<8) | (((uint32_t)getIMR()));
   return (intr_kind)ret;
}

int8_t wizphy_getphylink(void)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   return (getPHYSR() & PHYSR_LNK);
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
   if(wiz_mdio_read(PHYRAR_BMSR) & BMSR_LINK_STATUS) return PHY_LINK_ON;
   return PHY_LINK_OFF;
#endif  
}

void wizphy_reset(void)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   uint8_t tmp = getPHYCR1() | PHYCR1_RST;
   PHYUNLOCK();
   setPHYCR1(tmp);
   PHYLOCK();
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
	wiz_mdio_write(PHYRAR_BMCR, wiz_mdio_read(PHYRAR_BMCR) | BMCR_RST);
	while(wiz_mdio_read(PHYRAR_BMCR) & BMCR_RST);
#endif
}

void wizphy_setphyconf(wiz_PhyConf* phyconf)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   uint8_t tmp = 0;
   if(phyconf->mode == PHY_MODE_TE)
   {
      setPHYCR1(getPHYCR1() | PHYCR1_TE);
      tmp = PHYCR0_AUTO;
   }
   else
   {
      setPHYCR1(getPHYCR1() & ~PHYCR1_TE);
      if(phyconf->mode == PHY_MODE_AUTONEGO) tmp = PHYCR0_AUTO;
      else
      {
         tmp |= 0x04;
         if(phyconf->speed  == PHY_SPEED_10)    tmp |= 0x02;
         if(phyconf->duplex == PHY_DUPLEX_HALF) tmp |= 0x01;
      }
   }
   setPHYCR0(tmp);
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
   uint16_t tmp = wiz_mdio_read(PHYRAR_BMCR);
   if(phyconf->mode == PHY_MODE_TE)
   {
      setPHYCR1(getPHYC1R() | PHYCR1_TE);
      setPHYCR0(PHYCR0_AUTO);
   }
   else
   {
	  setPHYCR1(getPHYC1R() & ~PHYCR1_TE);
      if(phyconf->mode == PHY_MODE_AUTONEGO) tmp |= BMCR_ANE;
      else
      {
	     tmp &= ~(BMCR_ANE|BMCR_DPX|BMCR_SPD);
         if(phyconf->duplex == PHY_DUPLEX_FULL) tmp |= BMCR_DPX;
         if(phyconf->speed == PHY_SPEED_100)    tmp |= BMCR_SPD;
      }
      wiz_mdio_write(PHYRAR_BMCR, tmp);
   }
#endif   
}

void wizphy_getphyconf(wiz_PhyConf* phyconf)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   uint8_t tmp = 0;
   tmp = getPHYSR();
   if(getPHYCR1() & PHYCR1_TE) phyconf->mode = PHY_MODE_TE;
   else                        phyconf->mode = (tmp & (1<<5)) ? PHY_MODE_MANUAL : PHY_MODE_AUTONEGO ;
   phyconf->speed  = (tmp & (1<<4)) ? PHY_SPEED_10    : PHY_SPEED_100;
   phyconf->duplex = (tmp & (1<<3)) ? PHY_DUPLEX_HALF : PHY_DUPLEX_FULL;
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
   uint16_t tmp = 0;
   tmp = wiz_mdio_read(PHYRAR_BMCR);
   if(getPHYCR1() & PHYCR1_TE) phyconf->mode = PHY_MODE_TE;
   else phyconf->mode   = (tmp & BMCR_ANE) ? PHY_MODE_AUTONEGO : PHY_MODE_MANUAL;
   phyconf->duplex = (tmp & BMCR_DPX) ? PHY_DUPLEX_FULL   : PHY_DUPLEX_HALF;
   phyconf->speed  = (tmp & BMCR_SPD) ? PHY_SPEED_100     : PHY_SPEED_10;
#endif
}

void wizphy_getphystatus(wiz_PhyConf* phyconf)
{
   uint8_t tmp = 0;
   tmp = getPHYSR();
   if(getPHYCR1() & PHYCR1_TE) phyconf->mode = PHY_MODE_TE;
   else phyconf->mode   = (tmp & (1<<5))    ? PHY_MODE_MANUAL : PHY_MODE_AUTONEGO ;
   phyconf->speed  = (tmp & PHYSR_SPD) ? PHY_SPEED_10    : PHY_SPEED_100;
   phyconf->duplex = (tmp & PHYSR_DPX) ? PHY_DUPLEX_HALF : PHY_DUPLEX_FULL;
}

void wizphy_setphypmode(uint8_t pmode)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   uint8_t tmp = getPHYCR1();
   if (pmode == PHY_POWER_DOWN)    tmp |= PHYCR1_PWDN;
   else                            tmp &= ~PHYCR1_PWDN;
   setPHYCR1(tmp);
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
   uint16_t tmp = 0;
   tmp = wiz_mdio_read(PHYRAR_BMCR);
   if (pmode == PHY_POWER_DOWN)    tmp |= BMCR_PWDN;
   else                            tmp &= ~BMCR_PWDN;
   wiz_mdio_write(PHYRAR_BMCR, tmp);
#endif   
}

int8_t wizphy_getphypmode(void)
{
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_PHYCR_)
   if (getPHYCR1() & PHYCR1_PWDN)              return PHY_POWER_DOWN;
#elif (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
   if (wiz_mdio_read(PHYRAR_BMCR) & BMCR_PWDN) return PHY_POWER_DOWN;
#endif   
   return PHY_POWER_NORM;
}

void wizchip_setnetinfo(wiz_NetInfo* pnetinfo)
{
   uint8_t i=0;
   setSHAR(pnetinfo->mac);
   setGAR(pnetinfo->gw);
   setSUBR(pnetinfo->sn);
   setSIPR(pnetinfo->ip);   
   setGA6R(pnetinfo->gw6);
   setSUB6R(pnetinfo->sn6);
   setLLAR(pnetinfo->lla);
   setGUAR(pnetinfo->gua);

   for(i=0; i<4; i++)  _DNS_[i]  = pnetinfo->dns[i];
   for(i=0; i<16; i++) _DNS6_[i] = pnetinfo->dns6[i];

   _IPMODE_   = pnetinfo->ipmode;
}

void wizchip_getnetinfo(wiz_NetInfo* pnetinfo)
{
   uint8_t i = 0;
   getSHAR(pnetinfo->mac);

   getGAR(pnetinfo->gw);
   getSUBR(pnetinfo->sn);
   getSIPR(pnetinfo->ip);
   
   getGA6R(pnetinfo->gw6);
   getSUB6R(pnetinfo->sn6);
   getLLAR(pnetinfo->lla);
   getGUAR(pnetinfo->gua);
   for(i=0; i<4; i++)  pnetinfo->dns[i] = _DNS_[i];
   for(i=0; i<16; i++) pnetinfo->dns6[i]  = _DNS6_[i];

   pnetinfo->ipmode = _IPMODE_;
}

void wizchip_setnetmode(netmode_type netmode)
{
   uint32_t tmp = (uint32_t) netmode;
   setNETMR ((uint8_t)tmp);
   setNETMR2((uint8_t)(tmp>>8));
   setNET4MR((uint8_t)(tmp>>16));
   setNET6MR((uint8_t)(tmp>>24));
}

netmode_type wizchip_getnetmode(void)
{
   uint32_t ret = 0;
   ret = getNETMR();
   ret = (ret << 8)  + getNETMR2();
   ret = (ret << 16) + getNET4MR();
   ret = (ret << 24) + getNET6MR();
   return (netmode_type)ret;
}

void wizchip_settimeout(wiz_NetTimeout* nettime)
{
   setRCR(nettime->s_retry_cnt);
   setRTR(nettime->s_time_100us);
   setSLRCR(nettime->sl_retry_cnt);
   setSLRTR(nettime->sl_time_100us);
}

void wizchip_gettimeout(wiz_NetTimeout* nettime)
{
   nettime->s_retry_cnt   = getRCR();
   nettime->s_time_100us  = getRTR();
   nettime->sl_retry_cnt  = getSLRCR();
   nettime->sl_time_100us = getSLRTR();
}

int8_t wizchip_arp(wiz_ARP* arp)
{
   uint8_t tmp;
   if(arp->destinfo.len == 16)
   {
      setSLDIP6R(arp->destinfo.ip);
      setSLCR(SLCR_ARP6);
   }
   else
   {
      setSLDIP4R(arp->destinfo.ip);
      setSLCR(SLCR_ARP4);
   } 
   while(getSLCR());
   while((tmp = getSLIR()) == 0x00);
   setSLIRCLR(~SLIR_RA);
   if(tmp & (SLIR_ARP4 | SLIR_ARP6))
   {
      getSLDHAR(arp->dha);
      return 0;
   }  
   return -1;
}

int8_t wizchip_ping(wiz_PING* ping)
{
   uint8_t tmp;
   setPINGIDR(ping->id);
   setPINGSEQR(ping->seq);
   if(ping->destinfo.len == 16)
   {
      setSLDIP6R(ping->destinfo.ip);
      setSLCR(SLCR_PING6);
   }
   else
   {
      setSLDIP4R(ping->destinfo.ip);
      setSLCR(SLCR_PING4);      
   }  
   while(getSLCR());
   while((tmp = getSLIR()) == 0x00);
   setSLIRCLR(~SLIR_RA);
   if(tmp & (SLIR_PING4 | SLIR_PING6))  return 0;   
   return -1;
}

int8_t wizchip_dad(uint8_t* ipv6)
{
   uint8_t tmp;
   setSLDIP6R(ipv6);
   setSLCR(SLCR_NS);
   while(getSLCR());
   while((tmp = getSLIR()) == 0x00);
   setSLIRCLR(~SLIR_RA);
   if(tmp & SLIR_TOUT) return 0;
   return -1;
}

int8_t wizchip_slaac(wiz_Prefix* prefix)
{
   uint8_t tmp;
   setSLCR(SLCR_RS);
   while(getSLCR());
   while((tmp = getSLIR()) == 0x00);
   setSLIRCLR(~SLIR_RA);
   if(tmp & SLIR_RS)
   {
      prefix->len = getPLR();
      prefix->flag = getPFR();
      prefix->valid_lifetime = getVLTR();
      prefix->preferred_lifetime = getPLTR();
      getPAR(prefix->prefix);
      return 0;
   }
   return -1;
}

int8_t wizchip_unsolicited(void)
{
   uint8_t tmp;
   setSLCR(SLCR_UNA);
   while(getSLCR());
   while((tmp = getSLIR()) == 0x00);
   setSLIRCLR(~SLIR_RA);
   if(tmp & SLIR_TOUT) return 0;
   return -1;
}

int8_t wizchip_getprefix(wiz_Prefix * prefix)
{
   if(getSLIR() & SLIR_RA)
   {
      prefix->len = getPLR();
      prefix->flag = getPFR();
      prefix->valid_lifetime = getVLTR();
      prefix->preferred_lifetime = getPLTR();
      getPAR(prefix->prefix);
      setSLIRCLR(SLIR_RA);
   }
   return -1;
}
