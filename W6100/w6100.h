//* ****************************************************************************
//! \file w5100.h
//! \brief W5100 HAL Header File.
//! \version 1.0.0
//! \date 2013/10/21
//! \par  Revision history
//!       <2013/10/21> 1st Release
//! \author MidnightCow
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


#ifndef	_W6100_H_
#define	_W6100_H_

#include <stdint.h>
#include "wizchip_conf.h"


#if   (_WIZCHIP_ == W6100)

//#define _W6100_IO_BASE_             0x00000000

#define _W6100_SPI_READ_			(0x00 << 2) //< SPI interface Read operation in Control Phase
#define _W6100_SPI_WRITE_			(0x01 << 2) //< SPI interface Write operation in Control Phase

#define WIZCHIP_CREG_BLOCK          0x00 	//< Common register block
#define WIZCHIP_SREG_BLOCK(N)       (1+4*N) //< Socket N register block
#define WIZCHIP_TXBUF_BLOCK(N)      (2+4*N) //< Socket N Tx buffer address block
#define WIZCHIP_RXBUF_BLOCK(N)      (3+4*N) //< Socket N Rx buffer address block

#define WIZCHIP_OFFSET_INC(ADDR, N) (ADDR + (N<<8)) //< Increase offset address

#if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)
   #define _W6100_IO_BASE_     _WIZCHIP_IO_BASE_
#elif (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)	
	#define IDM_AR0            ((_WIZCHIP_IO_BASE_ + 0x0000))
	#define IDM_AR1            ((_WIZCHIP_IO_BASE_ + 0x0001))
	#define IDM_BSB            ((_WIZCHIP_IO_BASE_ + 0x0002))
	#define IDM_DR             ((_WIZCHIP_IO_BASE_ + 0x0003))
	#define _W6100_IO_BASE_    0x0000
#elif (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_)
   #define _W6100_IO_BASE_    0x00000000
#endif

///////////////////////////////////////
// Definition For Legacy Chip Driver //
///////////////////////////////////////
#define IINCHIP_READ(ADDR)                //WIZCHIP_READ(ADDR)               ///< The defined for legacy chip driver
#define IINCHIP_WRITE(ADDR,VAL)           //WIZCHIP_WRITE(ADDR,VAL)          ///< The defined for legacy chip driver
#define IINCHIP_READ_BUF(ADDR,BUF,LEN)    //WIZCHIP_READ_BUF(ADDR,BUF,LEN)   ///< The defined for legacy chip driver
#define IINCHIP_WRITE_BUF(ADDR,BUF,LEN)   //WIZCHIP_WRITE(ADDR,BUF,LEN)      ///< The defined for legacy chip driver


//-----------    defgroup --------------------------------

/**
 * @defgroup W5100 W5100
 * @brief WHIZCHIP register defines and I/O functions of @b W5100.
 *
 * - @ref WIZCHIP_register_W5100 : @ref Common_register_group_W5100S and @ref Socket_register_group_W5100S
 * - @ref WIZCHIP_IO_Functions_W5100 : @ref Basic_IO_function_W5100S, @ref Common_register_access_function_W5100S and @ref Socket_register_group_W5100S
 */

 /**
 * @defgroup WIZCHIP_register_W5100 WIZCHIP register
 * @ingroup W5100
 * @brief WIZCHIP register defines register group of <b> W5100 </b>.
 *
 * - \ref Common_register_group_W5100S : Common register group W5100
 * - \ref Socket_register_group_W5100S : \c SOCKET n register group W5100
 */


/**
 * @defgroup WIZCHIP_IO_Functions_W5100 WIZCHIP I/O functions
 * @ingroup W5100
 * @brief This supports the basic I/O functions for \ref WIZCHIP_register_W5100.
 *
 * - <b> Basic I/O function </b> \n
 *   WIZCHIP_READ(), WIZCHIP_WRITE(), WIZCHIP_READ_BUF(), WIZCHIP_WRITE_BUF() \n\n
 *
 * - \ref Common_register_group_W5100S <b>access functions</b> \n
 * 	-# @b Mode \n
 *    getMR(), setMR()
 * 	-# @b Interrupt \n
 *    getIR(), setIR(), getIMR(), setIMR(),
 * 	-# <b> Network Information </b> \n
 *    getSHAR(), setSHAR(), getGAR(), setGAR(), getSUBR(), setSUBR(), getSIPR(), setSIPR()
 * 	-# @b Retransmission \n
 *    getRCR(), setRCR(), getRTR(), setRTR()
 * 	-# @b PPPoE \n
 *    getPTIMER(), setPTIMER(), getPMAGIC(), getPMAGIC()
 *
 * - \ref Socket_register_group_W5100S <b>access functions</b> \n
 *   -# <b> SOCKET control</b> \n
 *      getSn_MR(), setSn_MR(), getSn_CR(), setSn_CR(), getSn_IR(), setSn_IR()
 *   -# <b> SOCKET information</b> \n
 *      getSn_SR(), getSn_DHAR(), setSn_DHAR(), getSn_PORT(), setSn_PORT(), getSn_DIPR(), setSn_DIPR(), getSn_DPORT(), setSn_DPORT()
 *      getSn_MSSR(), setSn_MSSR()
 *   -# <b> SOCKET communication </b> \n
 *      getSn_RXMEM_SIZE(), setSn_RXMEM_SIZE(), getSn_TXMEM_SIZE(), setSn_TXMEM_SIZE() \n
 *      getSn_TX_RD(), getSn_TX_WR(), setSn_TX_WR() \n
 *      getSn_RX_RD(), setSn_RX_RD(), getSn_RX_WR() \n
 *      getSn_TX_FSR(), getSn_RX_RSR()
 *   -# <b> IP header field </b> \n
 *      getSn_FRAG(), setSn_FRAG(),  getSn_TOS(), setSn_TOS() \n
 *      getSn_TTL(), setSn_TTL()
 */

/**
 * @defgroup Common_register_group_W5100S Common register
 * @ingroup WIZCHIP_register_W5100
 * @brief Common register group\n
 * It set the basic for the networking\n
 * It set the configuration such as interrupt, network information, ICMP, etc.
 * @details
 * @sa MR : Mode register.
 * @sa GAR, SUBR, SHAR, SIPR
 * @sa IR, Sn_IR, _IMR_  : Interrupt.
 * @sa _RTR_, _RCR_ : Data retransmission.
 * @sa PTIMER, PMAGIC : PPPoE.
 */


 /**
 * @defgroup Socket_register_group_W5100S Socket register
 * @ingroup WIZCHIP_register_W5100
 * @brief Socket register group\n
 * Socket register configures and control SOCKETn which is necessary to data communication.
 * @details
 * @sa Sn_MR, Sn_CR, Sn_IR : SOCKETn Control
 * @sa Sn_SR, Sn_PORT, Sn_DHAR, Sn_DIPR, Sn_DPORT : SOCKETn Information
 * @sa Sn_MSSR, Sn_TOS, Sn_TTL, Sn_FRAGR : Internet protocol.
 * @sa Sn_RXMEM_SIZE, Sn_TXMEM_SIZE, Sn_TX_FSR, Sn_TX_RD, Sn_TX_WR, Sn_RX_RSR, Sn_RX_RD, Sn_RX_WR : Data communication
 */

 /**
 * @defgroup Basic_IO_function_W5100S Basic I/O function
 * @ingroup WIZCHIP_IO_Functions_W5100
 * @brief These are basic input/output functions to read values from register or write values to register.
 */

/**
 * @defgroup Common_register_access_function_W5100S Common register access functions
 * @ingroup WIZCHIP_IO_Functions_W5100
 * @brief These are functions to access <b>common registers</b>.
 */

/**
 * @defgroup Socket_register_access_function_W5100S Socket register access functions
 * @ingroup WIZCHIP_IO_Functions_W5100
 * @brief These are functions to access <b>socket registers</b>.
 */

 //-----------------------------------------------------------------------------------

//----------------------------- W6100 Common Registers IOMAP -----------------------------

/**
 * @ingroup Common_register_group
 * @brief Chip Identification Register address(RO)
 * @details @ref
 */
#define CIDR 				(_W6100_IO_BASE_ + (0x0000 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Chip Version Register address(RO)
 * @details @ref
 */
#define VER				    (_W6100_IO_BASE_ + (0x0002 << 8) + (WIZCHIP_CREG_BLOCK << 3))
#define VER1				    (_W6100_IO_BASE_ + (0x0003 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief System Status Register address(R/W)
 * @details @ref
 */
#define SYSR                (_W6100_IO_BASE_ + (0x2000 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief System Command Register address(R/W)
 * @details @ref
 */
#define SYCR0               (_W6100_IO_BASE_ + (0x2004 << 8) + (WIZCHIP_CREG_BLOCK << 3))
#define SYCR1               (_W6100_IO_BASE_ + (0x2005 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Debug Register address(R/W)
 * @details @ref
 */
#define DBGR	            (_W6100_IO_BASE_ + (0x2010 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Ticker Counter Register address(R/W)
 * @details @ref
 */
#define TCNTR               (_W6100_IO_BASE_ + (0x2016 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Ticker Counter Clear Register address(R/W)
 * @details @ref
 */
#define TCNTCLR             (_W6100_IO_BASE_ + (0x2020 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt Register address(RO)
 * @details @ref
 */
#define IR                  (_W6100_IO_BASE_ + (0x2100 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET Interrupt Register address(RO)
 * @details @ref
 */
#define SIR                 (_W6100_IO_BASE_ + (0x2101 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Interrupt Register address(RO)
 * @details @ref
 */
#define SLIR                (_W6100_IO_BASE_ + (0x2102 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt Mask Register address(R/W)
 * @details @ref
 */
#define _IMR_               (_W6100_IO_BASE_ + (0x2104 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt Clear Register address(WO)
 * @details @ref
 */
#define IRCLR               (_W6100_IO_BASE_ + (0x2108 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET Interrupt Mask Register address(R/W)
 * @details @ref
 */
#define SIMR                (_W6100_IO_BASE_ + (0x2114 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Interrupt Mask Register address(R/W)
 * @details @ref
 */
#define SLIMR               (_W6100_IO_BASE_ + (0x2124 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Interrupt Clear Register address(WO)
 * @details @ref
 */
#define SLIRCLR             (_W6100_IO_BASE_ + (0x2128 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Prefer Register address(R/W)
 * @details @ref
 */
#define SLPR              (_W6100_IO_BASE_ + (0x212C << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Command Register address(R/W)
 * @details @ref
 */
#define SLCR                (_W6100_IO_BASE_ + (0x2130 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Statuss Register address(RO)
 * @details @ref
 */
#define PHYSR              (_W6100_IO_BASE_ + (0x3000 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY MII Register Address Register address(R/W)
 * @details @ref
 */
#define PHYRAR               (_W6100_IO_BASE_ + (0x3008 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Data Input Register address(R/W)
 * @details @ref
 */
#define PHYDIR0              (_W6100_IO_BASE_ + (0x300C << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Data Input Register address(R/W)
 * @details @ref
 */
#define PHYDIR1              (_W6100_IO_BASE_ + (0x300D << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Data Output Register address(R/W)
 * @details @ref
 */
#define PHYDOR0              (_W6100_IO_BASE_ + (0x3010 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Data Output Register address(R/W)
 * @details @ref
 */
#define PHYDOR1              (_W6100_IO_BASE_ + (0x3011 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Active Register address(R/W)
 * @details @ref
 */
#define PHYACR              (_W6100_IO_BASE_ + (0x3014 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Division Register address(R/W)
 * @details @ref
 */
#define PHYDIVR             (_W6100_IO_BASE_ + (0x3018 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Command Register address(R/W)
 * @details @ref
 */
#define PHYCR0              (_W6100_IO_BASE_ + (0x301C << 8) + (WIZCHIP_CREG_BLOCK << 3))
#define PHYCR1              (_W6100_IO_BASE_ + (0x301D << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Network IPv4 Mode Register address(R/W)
 * @details @ref
 */
#define NET4MR              (_W6100_IO_BASE_ + (0x4000 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Network IPv6 Mode Register address(R/W)
 * @details @ref
 */
#define NET6MR              (_W6100_IO_BASE_ + (0x4004 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Network Mode Register address(R/W)
 * @details @ref
 */
#define NETMR               (_W6100_IO_BASE_ + (0x4008 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Network Mode Register 2 address(R/W)
 * @details @ref
 */
#define NETMR2              (_W6100_IO_BASE_ + (0x4009 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP LCP request Timer Register address(R/W)
 * @details @ref
 */
#define PTIMER              (_W6100_IO_BASE_ + (0x4100 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP LCP Magic Number Register address(R/W)
 * @details @ref
 */
#define PMAGIC              (_W6100_IO_BASE_ + (0x4104 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Destination Hardware Address Register address(R/W)
 * @details @ref
 */
#define PHAR                (_W6100_IO_BASE_ + (0x4108 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Session ID Register address(R/W)
 * @details @ref
 */
#define PSIDR               (_W6100_IO_BASE_ + (0x4110 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Maximum Receive Unit Register address(R/W)
 * @details @ref
 */
#define PMRUR               (_W6100_IO_BASE_ + (0x4114 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Source Hardware Address Register address(R/W)
 * @details @ref SHAR configures the source hardware address.
 */
#define SHAR                (_W6100_IO_BASE_ + (0x4120 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv4 Gateway Address Register address(R/W)
 * @details @ref GA4R configures the default gateway address.
 */
#define GAR                 (_W6100_IO_BASE_ + (0x4130 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv4 Subnet Mask Register address(R/W)
 * @details @ref SUB4R configures the default subnet mask address.
 */
#define SUBR                (_W6100_IO_BASE_ + (0x4134 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv4 Source IP Register address(R/W)
 * @details @ref SIPR configures the source IP address.
 */
#define SIPR                (_W6100_IO_BASE_ + (0x4138 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 LLA(Link Local Address) Register address(R/W)
 * @details @ref LLAR configures the lla address.
 */
#define LLAR                (_W6100_IO_BASE_ + (0x4140 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 GUA(Global Unicast Address) Register address(R/W)
 * @details @ref GUAR configures the gua address.
 */
#define GUAR                (_W6100_IO_BASE_ + (0x4150 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 Subnet Mask Register address(R/W)
 * @details @ref SUB6R configures the default subnet mask address.
 */
#define SUB6R               (_W6100_IO_BASE_ + (0x4160 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 Gateway Address Register address(R/W)
 * @details @ref GA4R configures the default gateway address.
 */
#define GA6R                (_W6100_IO_BASE_ + (0x4170 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Peer IP Register address(R/W)
 * @details @ref
 */
#define SLPIPR              (_W6100_IO_BASE_ + (0x4180 << 8) + (WIZCHIP_CREG_BLOCK << 3))
/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Peer IP Register address(R/W)
 * @details @ref
 */
#define SLPIPR00              (_W6100_IO_BASE_ + (0x4180 << 8) + (WIZCHIP_CREG_BLOCK << 3))
/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Peer IP Register address(R/W)
 * @details @ref
 */
#define SLPIPR12              (_W6100_IO_BASE_ + (0x418C << 8) + (WIZCHIP_CREG_BLOCK << 3))


/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Peer Hardware Address Register address(R/W)
 * @details @ref
 */
#define SLPHAR              (_W6100_IO_BASE_ + (0x4190 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Ping ID Register address(R/W)
 * @details @ref
 */
#define PINGIDR             (_W6100_IO_BASE_ + (0x4198 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less ping Sequence number Register address(R/W)
 * @details @ref
 */
#define PINGSEQR            (_W6100_IO_BASE_ + (0x419C << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv4 Unreachable IP Address Register address(R/W)
 * @details @ref
 */
#define UIPR                (_W6100_IO_BASE_ + (0x41A0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv4 Unreachable Port number Register address(R/W)
 * @details @ref
 */
#define UPORTR              (_W6100_IO_BASE_ + (0x41A4 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 Unreachable IP Address Register address(R/W)
 * @details @ref
 */
#define UIP6R               (_W6100_IO_BASE_ + (0x41B0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief IPv6 Unreachable Port number Register address(R/W)
 * @details @ref
 */
#define UPORT6R             (_W6100_IO_BASE_ + (0x41C0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt Pending Time Register address(R/W)
 * @details @ref
 */
#define INTPTMR             (_W6100_IO_BASE_ + (0x41C5 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RA Prefix Length Register address(R/W)
 * @details @ref
 */
#define PLR           (_W6100_IO_BASE_ + (0x41D0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RA Flag Register address(R/W)
 * @details @ref
 */
#define _PFR_               (_W6100_IO_BASE_ + (0x41D4 << 8) + (WIZCHIP_CREG_BLOCK << 3))
//#define RAFLGR PFR

/**
 * @ingroup Common_register_group
 * @brief RA Valid Life Time Register address(R/W)
 * @details @ref
 */
#define VLTR            (_W6100_IO_BASE_ + (0x41D8 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RA Prefix Life Time Register address(R/W)
 * @details @ref
 */
#define PLTR            (_W6100_IO_BASE_ + (0x41DC << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RA Prefix Address Register address(R/W)
 * @details @ref
 */
#define _PAR_               (_W6100_IO_BASE_ + (0x41E0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief ICMPv6 Block Register address(R/W)
 * @details @ref
 */
#define ICMP6BLKR             (_W6100_IO_BASE_ + (0x41F0 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Chip configuration Lock Register address(R/W)
 * @details @ref
 */
#define CHPLCKR            (_W6100_IO_BASE_ + (0x41F4 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Network configuration Lock Register address(R/W)
 * @details @ref
 */
#define NETLCKR             (_W6100_IO_BASE_ + (0x41F5 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY configuration Lock Register address(R/W)
 * @details @ref
 */
#define PHYLCKR             (_W6100_IO_BASE_ + (0x41F6 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Retransmission Time Register address(R/W)
 * @details @ref
 */
#define _RTR_                 (_W6100_IO_BASE_ + (0x4200 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Retransmission Count Register address(R/W)
 * @details @ref
 */
#define _RCR_ 				(_W6100_IO_BASE_ + (0x4204 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Retransmission Time Register address(R/W)
 * @details @ref
 */
#define SLRTR               (_W6100_IO_BASE_ + (0x4208 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief SOCKET-less Retransmission Count Register address(R/W)
 * @details @ref
 */
#define SLRCR               (_W6100_IO_BASE_ + (0x420C << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Hop Limit Register address(R/W)
 * @details @ref
 */
#define HOPR             	(_W6100_IO_BASE_ + (0x420F << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Start Register address(R/W)
 * @details @ref
 */
#define RNGSTR				(_W6100_IO_BASE_ + (0x4300 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Control Register address(R/W)
 * @details @ref
 */
#define RNGCR				(_W6100_IO_BASE_ + (0x4302 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Poly Register address(R/W)
 * @details @ref
 */
#define RNGPLR				(_W6100_IO_BASE_ + (0x4303 << 8) + (WIZCHIP_CREG_BLOCK << 3))


/**
 * @ingroup Common_register_group
 * @brief RNG Seed Register address(R/W)
 * @details @ref
 */
#define RNGSDR				(_W6100_IO_BASE_ + (0x4307 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Check Register address(R/W)
 * @details @ref
 */
#define RNGCHKR				(_W6100_IO_BASE_ + (0x4311 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Value Register address(R/W)
 * @details @ref
 */
#define RNGVR				(_W6100_IO_BASE_ + (0x4312 << 8) + (WIZCHIP_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief RNG Crypted Data Register address(R/W)
 * @details @ref
 */
#define RNGCDR				(_W6100_IO_BASE_ + (0x4317 << 8) + (WIZCHIP_CREG_BLOCK << 3))

//----------------------------- W6100 Socket Registers -----------------------------
//--------------------------- For Backward Compatibility ---------------------------

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket Mode register(R/W)
 * @details \ref Sn_MR configures the option or protocol type of Socket n.\n\n
 * Each bit of \ref Sn_MR defined as the following.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>MULTI/MFEN</td> <td>BCASTBL/FPSH</td> <td>NDACK/SMBL/IGMP/MCASTBL</td> <td>UNIBL/MCAST6BL</td> <td>Protocol[3]</td> <td>Protocol[2]</td> <td>Protocol[1]</td> <td>Protocol[0]</td> </tr>
 * </table>
 * - \ref Sn_MR_MULTI	: Support UDP Multicasting
 * - \ref Sn_MR_MFEN    : Support MAC Filter Enable
 * - \ref Sn_MR_BCASTBL	: Broadcast Block
 * - \ref Sn_MR_FPSH   	: Force PSH flag
 * - \ref Sn_MR_NDACK   : No Delay ACK flag
 * - \ref Sn_MR_SMBL    : Solicited Multicast Block
 * - \ref Sn_MR_IGMP    : IGMP ver2, ver1
 * - \ref Sn_MR_MCASTBL : IPv4 UDP Multicast Block
 * - \ref Sn_MR_UNIBL   : Unicast Block
 * - \ref Sn_MR_MCAST6BL: IPv6 UDP Multicast Block </b>
 * - <b>Protocol</b>
 * <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>0</td> <td>Socket Closed</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>1</td> <td>TCP</td>             </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>0</td> <td>UDP</td>             </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>1</td> <td>IPRAW</td>           </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>1</td> <td>1</td> <td>MACRAW</td>          </tr>
 * 		<tr>   <td>1</td> <td>0</td> <td>0</td> <td>1</td> <td>IPv6 TCP</td>        </tr>
 * 		<tr>   <td>1</td> <td>0</td> <td>1</td> <td>0</td> <td>IPv6 UDP</td>        </tr>
 * 		<tr>   <td>1</td> <td>0</td> <td>1</td> <td>1</td> <td>IPv6 IPRAW</td>      </tr>
 * 		<tr>   <td>1</td> <td>1</td> <td>0</td> <td>0</td> <td>DUAL TCP</td>        </tr>
 * 		<tr>   <td>1</td> <td>1</td> <td>1</td> <td>0</td> <td>DUAL UDP</td>        </tr>
 * </table>
 * - <b>In case of Socket 0</b>
 *  <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>0</td> <td>0</td> <td>MACRAW</td>   </tr>
 * </table>
 *  - \ref Sn_MR_CLOSE   : Socket Closed
 *  - \ref Sn_MR_TCP     : TCP
 *  - \ref Sn_MR_UDP     : UDP
 *  - \ref Sn_MR_IPRAW   : IP LAYER RAW SOCK
 *  - \ref Sn_MR_PPPOE   : PPPoE
 *  - \ref Sn_MR_MACRAW  : MAC LAYER RAW SOCK
 *  - \ref Sn_MR_TCP6    : IPv6 TCP
 *  - \ref Sn_MR_UDP6    : IPv6 UDP
 *  - \ref Sn_MR_IPRAW6  : IPv6 IP LAYER RAW SOCK
 *  - \ref Sn_MR_PPPOE6  : IPv6 PPPoE
 *  - \ref Sn_MR_TCPD    : Dual TCP
 *  - \ref Sn_MR_UDPD    : Dual UDP
 *  @note MACRAW mode should be only used in Socket 0.
 */
#define Sn_MR(N)            (_W6100_IO_BASE_ + (0x0000 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Prefer Source IPv6 Address Register(R/W)
 * @details \ref Sn_PRFR configures the Source IPv6 Address of Socket n.
 * - \ref "00"   : AUTO
 * - \ref "10"   : LLA
 * - \ref "11"   : GUA
 * It is set before OPEN command.
 */
#define Sn_PRFR(N)          (_W6100_IO_BASE_ + (0x0004 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket command register(R/W)
 * @details This is used to set the command for Socket n such as OPEN, CLOSE, CONNECT, LISTEN, SEND, and RECEIVE.\n
 * After W6100 accepts the command, the \ref Sn_CR register is automatically cleared to 0x00.
 * Even though \ref Sn_CR is cleared to 0x00, the command is still being processed.\n
 * To check whether the command is completed or not, please check the \ref Sn_IR or \ref Sn_SR.
 * - \ref Sn_CR_OPEN 		: Initialize or open socket.
 * - \ref Sn_CR_LISTEN 		: Wait connection request on TCP/TCP6/TCPD mode(<b>Server mode</b>)
 * - \ref Sn_CR_CONNECT 	: Send connection request on TCP/TCPD mode(<b>Client mode</b>)
 * - \ref Sn_CR_CONNECT6	: Send connection request on TCP6/TCPD mode(<b>Client mode</b>):nohl
 *
 * - \ref Sn_CR_DISCON 		: Send closing request on TCP/TCP6/TCPD mode.
 * - \ref Sn_CR_CLOSE   	: Close socket.
 * - \ref Sn_CR_SEND    	: Update TX buffer pointer and send data in IPv4 socket.
 * - \ref Sn_CR_SEND6    	: Update TX buffer pointer and send data in IPv6 socket.
 * - \ref Sn_CR_SEND_KEEP 	: Send keep alive message.
 * - \ref Sn_CR_RECV		: Update RX buffer pointer and receive data.
 */
#define Sn_CR(N)            (_W6100_IO_BASE_ + (0x0010 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket interrupt register(R)
 * @details \ref Sn_IR indicates the status of Socket Interrupt such as establishment, termination, receiving data, timeout).\n
 * When an interrupt occurs and the corresponding bit \ref IR_SOCK(N) in \ref _IMR_ are set, \ref IR_SOCK(N) in \ref IR becomes '1'.\n
 * In order to clear the \ref Sn_IR bit, the host should write the bit to \n
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td></td> <td></td> <td></td> <td>SEND_OK</td> <td>TIMEOUT</td> <td>RECV</td> <td>DISCON</td> <td>CON</td> </tr>
 * </table>
 * - \ref Sn_IR_SENDOK : <b>SEND_OK Interrupt</b>
 * - \ref Sn_IR_TIMEOUT : <b>TIMEOUT Interrupt</b>
 * - \ref Sn_IR_RECV : <b>RECV Interrupt</b>
 * - \ref Sn_IR_DISCON : <b>DISCON Interrupt</b>
 * - \ref Sn_IR_CON : <b>CON Interrupt</b>
 */
#define Sn_IR(N)            (_W6100_IO_BASE_ + (0x0020 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket interrupt mask register
 * @details Register address to configure the interrupt mask of the socket
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b> expect <b>bit 4</b>.
 *
 */
#define Sn_IMR(N)           (_W6100_IO_BASE_ + (0x0024 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket interrupt clear register
 * @details Register address to clear the interrupt of the socket
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b> expect <b>bit 4</b>.
 *
 */
#define Sn_IRCLR(N)         (_W6100_IO_BASE_ + (0x0028 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket status register(R)
 * @details \ref Sn_SR indicates the status of Socket n.\n
 * The status of Socket n is changed by \ref Sn_CR or some special control packet as SYN, FIN packet in TCP.
 * @par Normal status
 * - \ref SOCK_CLOSED 		: Closed
 * - \ref SOCK_INIT   		: Initiate state
 * - \ref SOCK_LISTEN    	: Listen state
 * - \ref SOCK_ESTABLISHED 	: Success to connect
 * - \ref SOCK_CLOSE_WAIT   : Closing state
 * - \ref SOCK_UDP   		: UDP socket
 * - \ref SOCK_IPRAW   		: IPRAW socket
 * - \ref SOCK_IPRAW6   	: IPv6 IPRAW socket
 * - \ref SOCK_MACRAW  		: MAC raw mode socket
 *@par Temporary status during changing the status of Socket n.
 * - \ref SOCK_SYNSENT   	: This indicates Socket n sent the connect-request packet (SYN packet) to a peer.
 * - \ref SOCK_SYNRECV    	: It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.
 * - \ref SOCK_FIN_WAIT		: Connection state
 * - \ref SOCK_CLOSING		: Closing state
 * - \ref SOCK_TIME_WAIT	: Closing state
 * - \ref SOCK_LAST_ACK 	: Closing state
 */
#define Sn_SR(N)            (_W6100_IO_BASE_ + (0x0030 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Socket extension status register
 * @details Register address to describe IP version, Server/Client mode and Source IPv6 address of the socket
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b> expect <b>bit 4</b>.
 *
 */
#define Sn_ESR(N)           (_W6100_IO_BASE_ + (0x0031 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief IP Protocol Number(PN) Register(R/W)
 * @details \ref Sn_PNR that sets the protocol number field of the IPv4/IPv6 header at the IP layer. It is
 * valid only in IPRAW/IPRAW6 mode, and ignored in other modes.
 */
#define Sn_PROTOR(N)        (_W6100_IO_BASE_ + (0x0100 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief IP Type of Service(TOS) Register(R/W)
 * @details \ref Sn_TOS configures the TOS(Type Of Service field in IP Header) of Socket n.
 * It is set before OPEN command.
 */
#define Sn_TOSR(N)          (_W6100_IO_BASE_ + (0x0104 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief IP Time to live(TTL) Register(R/W)
 * @details \ref Sn_TTL configures the TTL(Time To Live field in IP header) of Socket n.
 * It is set before OPEN command.
 */
#define Sn_TTLR(N)          (_W6100_IO_BASE_ + (0x0108 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Fragment Register(R/W)
 * @details \ref Sn_FRGR configures the Fragment Offset in IP header of Socket n.
 * It is set before OPEN command.
 */
#define Sn_FRGR(N)          (_W6100_IO_BASE_ + (0x010C << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Maximum Segment Size(Sn_MSSR0) register address(R/W)
 * @details \ref Sn_MSSR configures or indicates the MTU(Maximum Transfer Unit) of Socket n.
 */
#define Sn_MSSR(N)          (_W6100_IO_BASE_ + (0x0110 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief source port register(R/W)
 * @details \ref Sn_PORT configures the source port number of Socket n.
 * It is valid when Socket n is used in TCP/UDP mode. It should be set before OPEN command is ordered.
*/
#define Sn_PORTR(N)         (_W6100_IO_BASE_ + (0x0114 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Peer MAC register address(R/W)
 * @details \ref Sn_DHAR configures the destination hardware address of Socket n when using SEND_MAC command in UDP mode or
 * it indicates that it is acquired in ARP-process by CONNECT/SEND command.
 */
#define Sn_DHAR(N)          (_W6100_IO_BASE_ + (0x0118 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Peer IP register address(R/W)
 * @details \ref Sn_DIPR configures or indicates the destination IP address of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP client mode, it configures an IP address of TCP server before CONNECT command.
 * In TCP server mode, it indicates an IP address of TCP client after successfully establishing connection.
 * In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command.
 */
#define Sn_DIPR(N)          (_W6100_IO_BASE_ + (0x0120 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief Peer IP register address(R/W)
 * @details \ref Sn_DIPR configures or indicates the destination IP address of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP client mode, it configures an IP address of TCP server before CONNECT command.
 * In TCP server mode, it indicates an IP address of TCP client after successfully establishing connection.
 * In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command.
 */
#define Sn_DIP6R(N)         (_W6100_IO_BASE_ + (0x0130 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group_W6100
 * @brief source port register(R/W)
 * @details \ref Sn_PORT configures the source port number of Socket n.
 * It is valid when Socket n is used in TCP/UDP mode. It should be set before OPEN command is ordered.
*/
#define Sn_DPORTR(N)        (_W6100_IO_BASE_ + (0x0140 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_MR2(N)           (_W6100_IO_BASE_ + (0x0144 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_RTR(N)           (_W6100_IO_BASE_ + (0x0180 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_RCR(N)           (_W6100_IO_BASE_ + (0x0184 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_KPALVTR(N)       (_W6100_IO_BASE_ + (0x0188 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_CSUMSKPR(N)      (_W6100_IO_BASE_ + (0x0198 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))


/**
 * @ingroup Socket_register_group
 * @brief Transmit memory size register(R/W)
 * @details @ref Sn_TXBUF_SIZE configures the TX buffer block size of Socket n. Socket n TX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes.
 * If a different size is configured, the data can�좎럩伊싩뵳占폹e normally transmitted to a peer.
 * Although Socket n TX Buffer Block size is initially configured to 2Kbytes,
 * user can be re-configure its size using @ref Sn_TXBUF_SIZE. The total sum of @ref Sn_TXBUF_SIZE can not be exceed 16Kbytes.
 * When exceeded, the data transmission error is occurred.
 */
#define Sn_TX_MSR(N)        (_W6100_IO_BASE_ + (0x0200 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit free memory size register(R)
 * @details @ref Sn_TX_FSR indicates the free size of Socket n TX Buffer Block. It is initialized to the configured size by @ref Sn_TXBUF_SIZE.
 * Data bigger than @ref Sn_TX_FSR should not be saved in the Socket n TX Buffer because the bigger data overwrites the previous saved data not yet sent.
 * Therefore, check before saving the data to the Socket n TX Buffer, and if data is equal or smaller than its checked size,
 * transmit the data with SEND/SEND_MAC command after saving the data in Socket n TX buffer. But, if data is bigger than its checked size,
 * transmit the data after dividing into the checked size and saving in the Socket n TX buffer.
 */
#define Sn_TX_FSR(N)         (_W6100_IO_BASE_ + (0x0204 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit memory read poi#define Sn_KPALVTR(N)       (_W6100_IO_BASE_ + (0x0188 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))
nter register address(R)
 * @details @ref Sn_TX_RD is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001, it is re-initialized while connecting with TCP.
 * After its initialization, it is auto-increased by SEND command.
 * SEND command transmits the saved data from the current @ref Sn_TX_RD to the @ref Sn_TX_WR in the Socket n TX Buffer.
 * After transmitting the saved data, the SEND command increases the @ref Sn_TX_RD as same as the @ref Sn_TX_WR.
 * If its increment value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.
 */
#define Sn_TX_RD(N)         (_W6100_IO_BASE_ + (0x0208 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit memory write pointer register address(R/W)
 * @details @ref Sn_TX_WR is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001, it is re-initialized while connecting with TCP.\n
 * It should be read or be updated like as follows.\n
 * 1. Read the starting address for saving the transmitting data.\n
 * 2. Save the transmitting data from the starting address of Socket n TX buffer.\n
 * 3. After saving the transmitting data, update @ref Sn_TX_WR to the increased value as many as transmitting data size.
 * If the increment value exceeds the maximum value 0xFFFF(greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.\n
 * 4. Transmit the saved data in Socket n TX Buffer by using SEND/SEND command
 */
#define Sn_TX_WR(N)          (_W6100_IO_BASE_ + (0x020C << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Receive memory size register(R/W)
 * @details @ref Sn_RXBUF_SIZE configures the RX buffer block size of Socket n.
 * Socket n RX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes.
 * If a different size is configured, the data cannot be normally received from a peer.
 * Although Socket n RX Buffer Block size is initially configured to 2Kbytes,
 * user can re-configure its size using @ref Sn_RXBUF_SIZE. The total sum of @ref Sn_RXBUF_SIZE can not be exceed 16Kbytes.
 * When exceeded, the data reception error is occurred.
 */
#define Sn_RX_MSR(N)        (_W6100_IO_BASE_ + (0x0220 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_RX_RSR(N)        (_W6100_IO_BASE_ + (0x0224 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_RX_RD(N)         (_W6100_IO_BASE_ + (0x0228 << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))

#define Sn_RX_WR(N)         (_W6100_IO_BASE_ + (0x022C << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))




/*----------------------------- W6100 Register values  -----------------------------*/

/* System Status Register Bit Definition */
/**
 * @brief CHIP Lock
 * @details 1 : Lock
 *          0 : unlock
 */
#define SYSR_CHPL               (1<<7)

/**
 * @brief NET Lock
 * @details 1 : Lock
 *          0 : unlock
 */
#define SYSR_NETL               (1<<6)

/**
 * @brief PHY Lock
 * @details 1 : Lock
 *          0 : unlock
 */
#define SYSR_PHYL               (1<<5)

/**
 * @brief Indirect Bus I/F Mode
 * @details 1 : Enable
 *          0 : Disable
 */
#define SYSR_IND                (1<<1)

/**
 * @brief SPI BUS I/F Mode
 * @details 1 : Enable
 *          0 : Disable
 */
#define SYSR_SPI                (1<<0)


/* System Command Register Bit Definition */
/**
 * @brief Reset
 * @details 0 : reset
 */
#define SYCR0_RST				(0<<7) ///< s/w reset

/**
 * @brief Global Interrupt Enable
 * @details 1 : enable
 */
#define SYCR1_IEN				(1<<7) ///< global interrupt enable

/**
 * @brief System Clock Select
 * @details 1 : 25MHz
 */
#define SYCR1_CLK25M			(1<<0) ///< System Operation Clock select 25MHz

/**
 * @brief System Clock Select
 * @details 0 : 100MHz
 */
#define SYCR1_CLK100M			(0<<0) ///< System Operation Clock select 100MHz


/* Interrupt Register Bit Definition */
/**
 * @brief WOL Magic Packet Interrupt
 * @details
 */
#define IR_WOL				    (1<<7) ///< receive magic packet on WOL mode

/**
 * @brief IPv6 Unreachable Packet Interrupt
 * @details
 */
#define IR_UNR6		        (1<<4) ///< check IPv6 Port Unreachable

/**
 * @brief IP Conflict Packet Interrupt
 * @details
 */
#define IR_IPCONF             (1<<2) ///< check IPv4 conflict

/**
 * @brief IPv4 Unreachable Packet Interrupt
 * @details
 */
#define IR_UNR4		        (1<<1) ///< check IPv4 Port Unreachable

/**
 * @brief PPP Termination Packet Interrupt
 * @details
 */
#define IR_PTERM		        (1<<0) ///< check IPv4 PPPoE Close message


/* SOCKET Interrupt Register Bit Definition */
/**
 * @brief SOCKET Interrupt bit
 * @details Indicates whether each socket interrupt has occured.
 */
#define SIR_INT(sn)		        (1<<sn) ///< check socket interrupt


/* SOCKET-less Interrupt Register Bit Definition */
/**
 * @brief Timeout Interrupt
 * @details
 */
#define SLIR_TIOUT	        (1<<7)

/**
 * @brief ARP Success Interrupt
 * @details
 */
#define SLIR_ARP4	            (1<<6) ///<

/**
 * @brief ICMPv4 PING Success Interrupt
 * @details
 */
#define SLIR_PING4	            (1<<5) ///<

/**
 * @brief ICMPv6 NS ARP Success Interrupt
 * @details
 */
#define SLIR_ARP6	            (1<<4) ///<

/**
 * @brief ICMPv6 Echo PING Success Interrupt
 * @details
 */
#define SLIR_PING6	            (1<<3) ///<

/**
 * @brief ICMPv6 NS DAD Fail Interrupt
 * @details
 */
#define SLIR_NS	            (1<<2) ///<

/**
 * @brief ICMPv6 Auto Configuration RS Success 
 * Interrupt
 * @details
 */
#define SLIR_RS                 (1<<1) ///<

/**
 * @brief ICMPv6 RA Received Interrupt
 * @details
 */
#define SLIR_RA                 (1<<0) ///<


/* Interrupt Mask Register Bit Definition */
/**
 * @brief WOL Magic Packet Interrupt Mask
 * @details
 */
#define IMR_WOL				    (1<<7) ///< receive magic packet on WOL mode

/**
 * @brief IPv6 Unreachable Packet Interrupt Mask
 * @details
 */
#define IMR_UNR6		        (1<<4) ///< check IPv6 Port Unreachable

/**
 * @brief IP Conflict Packet Interrupt Mask
 * @details
 */
#define IMR_IPCONF             (1<<2) ///< check IPv4 conflict

/**
 * @brief IPv4 Unreachable Packet Interrupt Mask
 * @details
 */
#define IMR_UNR4		        (1<<1) ///< check IPv4 Port Unreachable

/**
 * @brief PPP Termination Packet Interrupt Mask
 * @details
 */
#define IMR_PTERM		        (1<<0) ///< check IPv4 PPPoE Close message

/* Interrupt Clear Register Bit Definition */
/**
 * @brief WOL Magic Packet Interrupt Mask
 * @details
 */
#define IRCLR_WOL				    (1<<7) ///< receive magic packet on WOL mode

/**
 * @brief IPv6 Unreachable Packet Interrupt Mask
 * @details
 */
#define IRCLR_UNR6		        (1<<4) ///< check IPv6 Port Unreachable

/**
 * @brief IP Conflict Packet Interrupt Mask
 * @details
 */
#define IRCLR_IPCONF             (1<<2) ///< check IPv4 conflict

/**
 * @brief IPv4 Unreachable Packet Interrupt Mask
 * @details
 */
#define IRCLR_UNR4		        (1<<1) ///< check IPv4 Port Unreachable

/**
 * @brief PPP Termination Packet Interrupt Mask
 * @details
 */
#define IRCLR_PTERM		        (1<<0) ///< check IPv4 PPPoE Close message


/* SOCKET Interrupt Mask Register Bit Definition */
/**
 * @brief SOCKET Interrupt Mask
 * @details Indicates whether each socket interrupt has occured.
 */
#define SIMR_INT(sn)		    (1<<sn) ///< check socket interrupt Mask


/* SOCKET-less Interrupt Mask Register Bit Definition */
/**
 * @brief Timeout Interrupt Mask
 * @details
 */
#define SLIMR_TIOUT	        (1<<7)

/**
 * @brief ARP Success Interrupt Mask
 * @details
 */
#define SLIMR_ARP4	            (1<<6) ///<

/**
 * @brief ICMPv4 PING Success Interrupt Mask
 * @details
 */
#define SLIMR_PING4	            (1<<5) ///<

/**
 * @brief ICMPv6 NS ARP Success Interrupt Mask
 * @details
 */
#define SLIMR_ARP6	            (1<<4) ///<

/**
 * @brief ICMPv6 Echo PING Success Interrupt Mask
 * @details
 */
#define SLIMR_PING6	            (1<<3) ///<

/**
 * @brief ICMPv6 NS DAD Fail Interrupt Mask
 * @details
 */
#define SLIMR_NS               (1<<2) ///<

/**
 * @brief ICMPv6 Auto Configuration RS Success Interrupt Mask
 * @details
 */
#define SLIMR_RS                (1<<1) ///<

/**
 * @brief ICMPv6 RA Received Interrupt Mask
 * @details
 */
#define SLIMR_RA                (1<<0) ///<

/* SOCKET-less Interrupt Clear Register Bit Definition */
/**
 * @brief Timeout Interrupt Mask
 * @details
 */
#define SLIRCLR_TIOUT	        (1<<7)

/**
 * @brief ARP Success Interrupt Mask
 * @details
 */
#define SLIRCLR_ARP4	            (1<<6) ///<

/**
 * @brief ICMPv4 PING Success Interrupt Mask
 * @details
 */
#define SLIRCLR_PING4	            (1<<5) ///<

/**
 * @brief ICMPv6 NS ARP Success Interrupt Mask
 * @details
 */
#define SLIRCLR_ARP6	            (1<<4) ///<

/**
 * @brief ICMPv6 Echo PING Success Interrupt Mask
 * @details
 */
#define SLIRCLR_PING6	            (1<<3) ///<

/**
 * @brief ICMPv6 NS DAD Fail Interrupt Mask
 * @details
 */
#define SLIRCLR_NS               (1<<2) ///<

/**
 * @brief ICMPv6 Auto Configuration RS Success Interrupt Mask
 * @details
 */
#define SLIRCLR_RS                (1<<1) ///<

/**
 * @brief ICMPv6 RA Received Interrupt Mask
 * @details
 */
#define SLIRCLR_RA                (1<<0) ///<

/* SOCKET-less Prefer Register Bit Definition */
/**
 * @brief select IPv6 Source IP AUTO
 * @details
 */
#define SLPR_AUTO              (0<<1)

/**
 * @brief Select IPv6 Source IP LLA
 * @details
 */
#define SLPR_LLA               (1<<1)|(0<<0)

/**
 * @brief Select IPv6 Source IP GUA
 * @details
 */
#define SLPR_GUA               (1<<1)|(1<<0)


/* SOCKET-less Command Register Bit Definition */
/**
 * @brief IPv4 ARP Command
 * @details
 */
#define SLCR_ARP4              (1<<6) ///<

/**
 * @brief IPv4 PING Command
 * @details
 */
#define SLCR_PING4             (1<<5) ///<

/**
 * @brief IPv6 ARP Command
 * @details
 */
#define SLCR_ARP6              (1<<4) ///<

/**
 * @brief IPv6 PING Command
 * @details
 */
#define SLCR_PING6             (1<<3) ///<

/**
 * @brief IPv6 DAD NS Command
 * @details
 */
#define SLCR_NS               (1<<2) ///<

/**
 * @brief IPv6 Autoconfiguration RS Command
 * @details
 */
#define SLCR_RS                (1<<1) ///<

/**
 * @brief IPv6 Unsolicited NA Command
 * @details
 */
#define SLCR_NA                (1<<0) ///<


/* PHY Status Register Bit Definition */
/**
 * @brief Cable Unplugged
 * @details
 */
#define PHYSR_CABOFF            (1<<7) ///<

/**
 * @brief Auto Negotiation
 * details
 */
#define PHYSR_AUTO              (0<<5)

/**
 * @brief 100BASE-TX FDX
 * @details
 */
#define PHYSR_100F              (1<<5)|(0<<4)|(0<<3)

/**
 * @brief 100BASE-TX HDX
 * @details
 */
#define PHYSR_100H              (1<<5)|(0<<4)|(1<<3)

/**
 * @brief 10BASE-TX FDX
 * @details
 */
#define PHYSR_10F               (1<<5)|(1<<4)|(0<<3)

/**
 * @brief 10BASE-TX HDX
 * @details
 */
#define PHYSR_10H               (1<<5)|(1<<4)|(1<<3)

/**
 * @brief Half Duplex
 * @details
 */
#define PHYSR_HDX               (1<<2) ///<

/**
 * @brief Full Duplex
 * @details
 */
#define PHYSR_FDX               (0<<2) ///<

/**
 * @brief 10MHz Speed
 * @details
 */
#define PHYSR_10M               (1<<1) ///<

/**
 * @brief 100MHz Speed
 * @details
 */
#define PHYSR_100M              (0<<1) ///<

/**
 * @brief Link Up
 * @details
 */
#define PHYSR_LNK               (1<<0) ///<

/* PHY Division Register Bit Definition */
/**
 * @brief 1/32
 * @details
 */
#define PHYDIVR_32               0x00
#define PHYDIVR_64               0x01
#define PHYDIVR_128              0xff


/* PHY Command Register Bit Definition */
/**
 * @brief Auto Negotiation
 * @details
 */
#define PHYCR_AUTO              (0<<2)

/**
 * @brief 100BASE-TX FDX
 * @details
 */
#define PHYCR_100F              (1<<2)|(0<<1)|(0<<0)

/**
 * @brief 100BASE-TX HDX
 * @details
 */
#define PHYCR_100H              (1<<2)|(0<<1)|(1<<0)

/**
 * @brief 10BASE-TX FDX
 * @details
 */
#define PHYCR_10F               (1<<2)|(1<<1)|(0<<0)

/**
 * @brief 10BASE-TX HDX
 * @details
 */
#define PHYCR_10H               (1<<2)|(1<<1)|(1<<0)

/**
 * @brief Wake On LAN
 * @details
 */
//#define PHYCR1_WOL              (1<<7) ///<

/**
 * @brief PHY Power Down
 * @details
 */
#define PHYCR1_PWDN             (1<<5) ///<

/**
 * @brief PHY TE Mode
 * @details
 */
#define PHYCR1_TE               (1<<3) ///<

/**
 * @brief PHY Software Reset
 * @details
 */
#define PHYCR1_RST              (1<<0) ///<


/* IPv4 Network Mode Register Bit Definition */
/**
 * @brief UDP Unreachable Packet Block
 * @details
 */
#define NET4MR_UNRB           (1<<3) ///<

/**
 * @brief Force ARP for PING Reply
 * @details
 */
#define NET4MR_PARP             (1<<2) ///<

/**
 * @brief TCP Reset Packet Block
 * @details
 */
#define NET4MR_RSTB         (1<<1) ///<

/**
 * @brief PING Reply Block
 * @details
 */
#define NET4MR_PB               (1<<0) ///<


/* IPv6 Network Mode Register Bit Definition */
/**
 * @brief UDP Unreachable Packet Block
 * @details
 */
#define NET6MR_UNRB           (1<<3) ///<

/**
 * @brief Force ARP for Echo Reply
 * @details
 */
#define NET6MR_PARP             (1<<2) ///<

/**
 * @brief TCP Reset Packet Block
 * @details
 */
#define NET6MR_RSTB         (1<<1) ///<

/**
 * @brief Echo Reply Block
 * @details
 */
#define NET6MR_PB               (1<<0) ///<


/* Network Mode Register Bit Definition */
/**
 * @brief IPv6 All Node Block
 * @details
 */
#define NETMR_ANB               (1<<5) ///<

/**
 * @brief IPv6 Multicast Block
 * @details
 */
#define NETMR_M6B               (1<<4) ///<

/**
 * @brief No Size Check on MACRAW Mode
 * @details
 */
//#define NETMR_NSC               (1<<3) ///<

/**
 * @brief Wake On LAN
 * @details
 */
#define NETMR_WOL               (1<<2) ///<

/**
 * @brief IPv6 Packet Block
 * @details
 */
#define NETMR_IP6B              (1<<1) ///<

/**
 * @brief IPv4 Packet Block
 * @details
 */
#define NETMR_IP4B              (1<<0) ///<

/**
 * @brief PPPoE Mode
 * @details
 */
#define NETMR2_DHAS            (1<<7) ///<

/**
 * @brief PPPoE Mode
 * @details
 */
#define NETMR2_PPPoE            (1<<0) ///<


/* ICMPv6 Block Register Bit Definition */
/**
 * @brief PING Block
 * @details
 */
#define ICMP6BLK_PING6           (1<<4) ///<

/**
 * @brief MLD Block
 * @details
 */
#define ICMP6BLK_MLD             (1<<3) ///<

/**
 * @brief RA Block
 * @details
 */
#define ICMP6BLK_RA              (1<<2) ///<

/**
 * @brief NA Block
 * @details
 */
#define ICMP6BLK_NA              (1<<1) ///<

/**
 * @brief NS Block
 * @details
 */
#define ICMP6BLK_NS              (1<<0) ///<




/* Sn_MR values */
/**
 * @brief 
 * @details 
 */
#define Sn_MR_MULTI             (1<<7)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_MFEN              (1<<7)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_BRDB            (1<<6)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_FPSH              (1<<6)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_ND                (1<<5)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_MB6               (1<<5)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_MC                (1<<5)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_MMB               (1<<5)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_UBLK            (1<<4)

/**
 * @brief 
 * @details 
 */
#define Sn_MR_M6BLK             (1<<4)

/**
 * @brief Socket Close
 * @details 
 */
#define Sn_MR_CLOSE	            0x00 ///< unused socket

/**
 * @brief IPv4 TCP
 * @details 
 */
#define Sn_MR_TCP               0x01
#define Sn_MR_STREAM            0x01
#define SOCK_STREAM             0x01
#define Sn_MR_TCP4			    0x01 ///< TCP

/**
 * @brief IPv4 TCP
 * @details 
 */
#define Sn_MR_UDP               0x02
#define Sn_MR_DGRAM             0x02
#define SOCK_DGRAM							0x02
#define Sn_MR_UDP4              0x02 ///< UDP

/**
 * @brief IPv4 IPRAW
 * @details 
 */
#define Sn_MR_IPRAW             0x03
#define Sn_MR_IPRAW4            0x03

/**
 * @brief MACRAW
 * @details 
 */
#define Sn_MR_MACRAW            0x07

/**
 * @brief IPv6 TCP
 * @details 
 */
#define Sn_MR_TCP6              0x09

/**
 * @brief IPv6 UDP
 * @details 
 */
#define Sn_MR_UDP6              0x0A

/**
 * @brief IPv6 IPRAW
 * @details 
 */
#define Sn_MR_IPRAW6            0x0B

/**
 * @brief DUAL TCP
 * @details 
 */
#define Sn_MR_DUALT             0x0D
#define Sn_MR_STREAMD           0x0D

/**
 * @brief DUAL UDP
 * @details 
 */
#define Sn_MR_DUALU             0x0E
#define Sn_MR_DGRAMD            0x0E

/**
 * @brief Addfess Family v4
 * @details 
 */
#define AF_INET 2

/**
 * @brief Addfess Family v6
 * @details 
 */
#define AF_INET6 23

/**
 * @brief Addfess Family DUAL
 * @details 
 */
#define AF_INET_DUAL 11


/* Socket n Prefer Source IPv6 Address Register BIt Definition */
/**
 * @brief AUTO Prefer
 * details
 */
#define Sn_PRFR_AUTO            0x00
/**
 * @brief Link Local Address Prefer
 * details
 */
#define Sn_PRFR_LLA             0x02
/**
 * @brief Global Unicast Address Prefer
 * details
 */
#define Sn_PRFR_GUA             0x03


/* Socket n Command Register BIt Definition */
/**
 * @brief Open
 * details
 */
#define Sn_CR_OPEN              0x01

/**
 * @brief Listen
 * details
 */
#define Sn_CR_LISTEN            0x02

/**
 * @brief Connect
 * @details
 */
#define Sn_CR_CONNECT           0x04

/**
 * @brief IPv6 Connect
 * @details
 */
#define Sn_CR_CONNECT6          0x84

/**
 * @brief Disconnect
 * @details
 */
#define Sn_CR_DISCONNECT        0x08

/**
 * @brief Close
 * @details
 */
#define Sn_CR_CLOSE             0x10

/**
 * @brief Send
 * @details
 */
#define Sn_CR_SEND              0x20

/**
 * @brief IPv6 Send
 * @details
 */
#define Sn_CR_SEND6             0xA0

/**
 * @brief Send Keep Alive
 * @details
 */
#define Sn_CR_SEND_KEEP         0x22

/**
 * @brief Receive
 * @details
 */
#define Sn_CR_RECV              0x40



/* Sn_IR values */
/**
 * @brief SEND_OK Interrupt
 * @details This is issued when SEND command is completed.
 */
#define Sn_IR_SENDOK		0x10 ///< complete sending

/**
 * @brief TIMEOUT Interrupt
 * @details This is issued when ARPTO or TCPTO occurs.
 */
#define Sn_IR_TIMEOUT		0x08 ///< assert timeout

/**
 * @brief RECV Interrupt
 * @details This is issued whenever data is received from a peer.
 */
#define Sn_IR_RECV          0x04

/**
 * @brief DISCON Interrupt
 * @details This is issued when FIN or FIN/ACK packet is received from a peer.
 */
#define Sn_IR_DISCON        0x02

/**
 * @brief CON Interrupt
 * @details This is issued one time when the connection with peer is successful and then \ref Sn_SR is changed to \ref SOCK_ESTABLISHED.
 */
#define Sn_IR_CON           0x01


/* Sn_IMR values */

/**
 * @brief SEND_OK Interrupt mask bit
 * @details
 */
#define Sn_IMR_SENDOK		0x10 ///< complete sending

/**
 * @brief TIMEOUT Interrupt mask bit
 * @details
 */
#define Sn_IMR_TIMEOUT		0x08 ///< assert timeout

/**
 * @brief RECV Interrupt mask bit
 * @details
 */
#define Sn_IMR_RECV          0x04

/**
 * @brief DISCON Interrupt mask bit
 * @details
 */
#define Sn_IMR_DISCON        0x02

/**
 * @brief CON Interrupt mask bit
 * @details
 */
#define Sn_IMR_CON           0x01


/* Sn_IR_CLR values */

/**
 * @brief SEND_OK Interrupt mask bit
 * @details
 */
#define Sn_IRCLR_SENDOK		0x10 ///< complete sending

/**
 * @brief TIMEOUT Interrupt mask bit
 * @details
 */
#define Sn_IRCLR_TIMEOUT		0x08 ///< assert timeout

/**
 * @brief RECV Interrupt mask bit
 * @details
 */
#define Sn_IRCLR_RECV           0x04

/**
 * @brief DISCON Interrupt mask bit
 * @details
 */
#define Sn_IRCLR_DISCON         0x02

/**
 * @brief CON Interrupt mask bit
 * @details
 */
#define Sn_IRCLR_CON            0x01




/* Sn_SR values */
/**
 * @brief Closed
 * @details This indicates that Socket n is released.\n
 * When DICON, CLOSE command is ordered, or when a timeout occurs, it is changed to \ref SOCK_CLOSED regardless of previous status.
 */
#define SOCK_CLOSED			0x00 ///< closed

/**
 * @brief Initiate state
 * @details This indicates Socket n is opened with TCP mode.\n
 * It is changed to \ref SOCK_INIT when Sn_MR(P[3:0]) = 001)and OPEN command is ordered.\n
 * After \ref SOCK_INIT, user can use LISTEN /CONNECT command.
 */
#define SOCK_INIT 			0x13 ///< init state

/**
 * @brief Listen state
 * @details This indicates Socket n is operating as <b>TCP server</b>mode and waiting for connection-request (SYN packet) from a peer (<b>TCP client</b>).\n
 * It will change to \ref SOCK_ESTABLISHED when the connection-request is successfully accepted.\n
 * Otherwise it will change to \ref SOCK_CLOSED after TCPTO occurred (Sn_IR(TIMEOUT) = '1').
 */
#define SOCK_LISTEN         0x14

/**
 * @brief Connection state
 * @details This indicates Socket n sent the connect-request packet (SYN packet) to a peer.\n
 * It is temporarily shown when \ref Sn_SR is changed from \ref SOCK_INIT to \ref SOCK_ESTABLISHED by CONNECT command.\n
 * If connect-accept(SYN/ACK packet) is received from the peer at SOCK_SYNSENT, it changes to \ref SOCK_ESTABLISHED.\n
 * Otherwise, it changes to \ref SOCK_CLOSED after TCPTO (\ref Sn_IR[TIMEOUT] = '1') is occurred.
 */
#define SOCK_SYNSENT        0x15

/**
 * @brief Connection state
 * @details It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.\n
 * If socket n sends the response (SYN/ACK  packet) to the peer successfully,  it changes to \ref SOCK_ESTABLISHED. \n
 * If not, it changes to \ref SOCK_CLOSED after timeout occurs (\ref Sn_IR[TIMEOUT] = '1').
 */
#define SOCK_SYNRECV        0x16

/**
 * @brief Success to connect
 * @details This indicates the status of the connection of Socket n.\n
 * It changes to \ref SOCK_ESTABLISHED when the <b>TCP SERVER</b>processed the SYN packet from the <b>TCP CLIENT</b>during \ref SOCK_LISTEN, or
 * when the CONNECT command is successful.\n
 * During \ref SOCK_ESTABLISHED, DATA packet can be transferred using SEND or RECV command.
 */
#define SOCK_ESTABLISHED    0x17

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define SOCK_FIN_WAIT       0x18

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define SOCK_CLOSING        0x1A

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define SOCK_TIME_WAIT      0x1B

/**
 * @brief Closing state
 * @details This indicates Socket n received the disconnect-request (FIN packet) from the connected peer.\n
 * This is half-closing status, and data can be transferred.\n
 * For full-closing, DISCON command is used. But For just-closing, @ref Sn_CR_CLOSE command is used.
 */
#define SOCK_CLOSE_WAIT     0x1C

/**
 * @brief Closing state
 * @details This indicates Socket n is waiting for the response (FIN/ACK packet) to the disconnect-request (FIN packet) by passive-close.\n
 * It changes to \ref SOCK_CLOSED when Socket n received the response successfully, or when timeout occurs  (\ref Sn_IR[TIMEOUT] = '1').
 */
#define SOCK_LAST_ACK       0x1D

/**
 * @brief UDP socket
 * @details This indicates Socket n is opened in UDP mode(Sn_MR(P[3:0]) = 010).\n
 * It changes to SOCK_UDP when Sn_MR(P[3:0]) = 010 and @ref Sn_CR_OPEN command is ordered.\n
 * Unlike TCP mode, data can be transfered without the connection-process.
 */
#define SOCK_UDP			0x22 ///< udp socket

/**
 * @brief IP raw mode socket
 * @details TThe socket is opened in IPRAW mode. The SOCKET status is change to SOCK_IPRAW when @ref Sn_MR (P3:P0) is
 * Sn_MR_IPRAW and @ref Sn_CR_OPEN command is used.\n
 * IP Packet can be transferred without a connection similar to the UDP mode.
*/
#define SOCK_IPRAW			0x32 ///< ip raw mode socket

/**
 * @brief IPv6 raw mode socket
 * @details TThe socket is opened in IPRAW mode. The SOCKET status is change to SOCK_IPRAW when @ref Sn_MR (P3:P0) is
 * Sn_MR_IPRAW and @ref Sn_CR_OPEN command is used.\n
 * IP Packet can be transferred without a connection similar to the UDP mode.
*/
#define SOCK_IPRAW6			0x33 ///< ip raw mode socket

/**
 * @brief MAC raw mode socket
 * @details This indicates Socket 0 is opened in MACRAW mode (@ref Sn_MR(P[3:0]) = '100' and n=0) and is valid only in Socket 0.\n
 * It changes to SOCK_MACRAW when @ref Sn_MR(P[3:0]) = '100' and @ref Sn_CR_OPEN command is ordered.\n
 * Like UDP mode socket, MACRAW mode Socket 0 can transfer a MAC packet (Ethernet frame) without the connection-process.
 */
#define SOCK_MACRAW			0x42 ///< mac raw mode socket

/* Sn_ESR values */
/**
 * @brief Socket IP version IPv4
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_IPv4			(0<<2)

/**
 * @brief Socket IP version IPv6
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_IPv6			(1<<2)

/**
 * @brief Socket IP version DUAL
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_DUAL           (1<<2)

/**
 * @brief Socket mode Client
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_CLT			(0<<1)

/**
 * @brief Socket mode Server
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_SVR			(1<<1)

/**
 * @brief Socket Source IP Address LLA
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_LLA			(0<<0)

/**
 * @brief Socket Source IP Address GUA
 * @details valid state is SOCK_ESTABLISHED
 */
#define SOCK_GUA			(1<<0)


/* Sn_MR2 values */
/**
 * @brief Skip ARP
 * @details
 */
#define Sn_MR2_DHAM         (1<<1)

/**
 * @brief Force ARP
 * @details
 */
#define Sn_MR2_FARP         (0<<1)|(1<<0)


/* Sn_CSUMSKPR values */
/**
 * @brief Socket TCP Checksum Skip
 * @details
 */
#define Sn_CSUMSKPR_TCP     (1<<1)

/**
 * @brief Socket UDP Checksum Skip
 * @details
 */
#define Sn_CSUMSKPR_UDP     (1<<0)


/*----------------------------For PHY Control-------------------------------*/

/********************/
/* Register Address */
/********************/

//Basic mode control register, basic register
#define PHYMDIO_BMCR				0x00

//Basic mode status register, basic register
#define PHYMDIO_BMSR				0x01


/********************/
/* Bit definitions  */
/********************/

//For BMCR register
#define BMCR_RESET				(1<<15)
#define BMCR_MLOOPBACK			(1<<14)
#define BMCR_SPEED				(1<<13)
#define BMCR_AUTONEGO			(1<<12)
#define BMCR_PWDN				(1<<11)
#define BMCR_ISOLATE			(1<<10)
#define BMCR_RSTNEGO			(1<<9)
#define BMCR_DUP				(1<<8)
#define BMCR_COLTEST			(1<<7)

//For BMSR register
#define BMSR_AUTONEGO_COMPL		(1<<5)
#define BMSR_REMOTE_FAULT		(1<<4)
#define BMSR_LINK_STATUS		(1<<2)
#define BMSR_JAB_DETECT			(1<<1)
#define EXTENDED_CAPA			(1<<0)


/********************/
/*Functions for PHY */
/********************/
//todo move this definition to bit area
#define PHYACR_READ			0x02
#define PHYACR_WRITE		0x01




/**
 * @brief Enter a critical section
 *
 * @details It is provided to protect your shared code which are executed without distribution. \n \n
 *
 * In non-OS environment, It can be just implemented by disabling whole interrupt.\n
 * In OS environment, You can replace it to critical section api supported by OS.
 *
 * \sa WIZCHIP_READ(), WIZCHIP_WRITE(), WIZCHIP_READ_BUF(), WIZCHIP_WRITE_BUF()
 * \sa WIZCHIP_CRITICAL_EXIT()
 */
#define WIZCHIP_CRITICAL_ENTER()    WIZCHIP.CRIS._enter()

#ifdef _exit
#undef _exit
#endif

/**
 * @brief Exit a critical section
 *
 * @details It is provided to protect your shared code which are executed without distribution. \n\n
 *
 * In non-OS environment, It can be just implemented by disabling whole interrupt. \n
 * In OS environment, You can replace it to critical section api supported by OS.
 *
 * @sa WIZCHIP_READ(), WIZCHIP_WRITE(), WIZCHIP_READ_BUF(), WIZCHIP_WRITE_BUF()
 * @sa WIZCHIP_CRITICAL_ENTER()
 */
#define WIZCHIP_CRITICAL_EXIT()     WIZCHIP.CRIS._exit()



////////////////////////
// Basic I/O Function //
////////////////////////
//
//M20150601 :  uint16_t AddrSel --> uint32_t AddrSel
//
/**
 * @ingroup Basic_IO_function_W6100
 * @brief It reads 1 byte value from a register.
 * @param AddrSel Register address
 * @return The value of register
 */
uint8_t  WIZCHIP_READ (uint32_t AddrSel);

/**
 * @ingroup Basic_IO_function_W6100
 * @brief It writes 1 byte value to a register.
 * @param AddrSel Register address
 * @param wb Write data
 * @return void
 */
void     WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb );

/**
 * @ingroup Basic_IO_function_W6100
 * @brief It reads sequence data from registers.
 * @param AddrSel Register address
 * @param pBuf Pointer buffer to read data
 * @param len Data length
 */
void     WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, uint16_t len);

/**
 * @ingroup Basic_IO_function_W6100
 * @brief It writes sequence data to registers.
 * @param AddrSel Register address
 * @param pBuf Pointer buffer to write data
 * @param len Data length
 */
void     WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, uint16_t len);



/////////////////////////////////
// Common Register IO function //
/////////////////////////////////
#define getCIDR() \
	    (((uint16_t)WIZCHIP_READ(CIDR) << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(CIDR,1)))

#define getVER() \
	    (((uint16_t)WIZCHIP_READ(VER) << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(VER,1)))

#define getSYSR() \
	    WIZCHIP_READ(SYSR)

#define getSYCR0() \
	    WIZCHIP_READ(SYCR0)

#define setSYCR0(sycr0) \
	    WIZCHIP_WRITE(SYCR0, sycr0)

#define getSYCR1() \
	    WIZCHIP_READ(SYCR1)
			
#define setSYCR1(sycr1) \
	    WIZCHIP_WRITE(SYCR1, sycr1)

#define getTCNTR() \
	    (((uint16_t)(WIZCHIP_READ(TCNTR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(TCNTR,1)))

#define setTCNTCLR() \
	    WIZCHIP_WRITE(TCNTCLR,0xff)

#define getIR() \
	    WIZCHIP_READ(IR)

#define getSIR() \
	    WIZCHIP_READ(SIR)

#define getSLIR() \
	    WIZCHIP_READ(SLIR)

#define setIMR(imr) \
	    WIZCHIP_WRITE(_IMR_,imr)

#define getIMR() \
	    WIZCHIP_READ(_IMR_)

#define setIRCLR(irclr) \
	    WIZCHIP_WRITE(IRCLR,irclr)

#define setSIMR(simr) \
	    WIZCHIP_WRITE(SIMR,simr)

#define getSIMR() \
	    WIZCHIP_READ(SIMR)

#define setSLIMR(slimr) \
	    WIZCHIP_WRITE(SLIMR,slimr)

#define getSLIMR() \
	    WIZCHIP_READ(SLIMR)

#define setSLIRCLR(slirclr) \
	    WIZCHIP_WRITE(SLIRCLR,slirclr)

#define setSLPR(slpr) \
	    WIZCHIP_WRITE(SLPR,slpr)
#define getSLPR() \
	    WIZCHIP_WRITE(SLPR)

#define setSLCR(slcr) \
	    WIZCHIP_WRITE(SLCR,slcr)
			
#define getSLCR()	\
			WIZCHIP_READ(SLCR)

#define getPHYSR() \
	    WIZCHIP_READ(PHYSR)


#define setPHYRAR(phyrar) \
        WIZCHIP_WRITE(PHYRAR,phyrar)				
#define getPHYRAR() \
        WIZCHIP_READ(PHYRAR)

#define setPHYDIR(phydir) \
	    WIZCHIP_WRITE_BUF(PHYDIR,phydir,2)

#define getPHYDOR(phydor) \
	    WIZCHIP_READ_BUF(PHYDOR,phydor,2)

#define setPHYACR(phyacr) \
	    WIZCHIP_WRITE(PHYACR,phyacr)
#define getPHYACR() \
	    WIZCHIP_READ(PHYACR)

#define setPHYDIVR(phydivr) \
  	    WIZCHIP_WRITE(PHYDIVR,phydivr)

#define getPHYDIVR() \
	    WIZCHIP_READ(PHYDIVR)

#define setPHYCR0(phycr0) \
	    WIZCHIP_WRITE(PHYCR0,phycr0)

#define setPHYCR1(phycr1) \
	    WIZCHIP_WRITE(PHYCR1,phycr1)

#define setNET4MR(net4mr) \
	    WIZCHIP_WRITE(NET4MR,net4mr)

#define setNET6MR(net6mr) \
	    WIZCHIP_WRITE(NET6MR,net6mr)

#define setNETMR(netmr) \
	    WIZCHIP_WRITE(NETMR,netmr)

#define setNETMR2(netmr2) \
	    WIZCHIP_WRITE(NETMR2,netmr2)

#define getNET4MR() \
		WIZCHIP_READ(NET4MR)

#define getNET6MR() \
		WIZCHIP_READ(NET6MR)

#define getNETMR() \
		WIZCHIP_READ(NETMR)

#define getNETMR2() \
		WIZCHIP_READ(NETMR2)

#define setPTIMER(ptimer) \
        WIZCHIP_WRITE(PTIMER, ptimer)

#define getPTIMER() \
        WIZCHIP_READ(PTIMER)

#define setPMAGIC(pmagic) \
        WIZCHIP_WRITE(PMAGIC, pmagic)

#define getPMAGIC() \
        WIZCHIP_READ(PMAGIC)

#define setPHAR(phar) \
	    WIZCHIP_WRITE_BUF(PHAR,phar,6)

#define getPHAR(phar) \
	    WIZCHIP_READ_BUF(PHAR,phar,6)

#define setPSIDR(psidr) \
	    WIZCHIP_WRITE(PSIDR,(uint8_t)(psidr >> 8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PSIDR,1),(uint8_t)psidr);      

#define getPSIDR() \
	    (((uint16_t)(WIZCHIP_READ(PSIDR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PSIDR,1)))

#define setPMRUR(pmrur) \
	    WIZCHIP_WRITE(PMRUR,(uint8_t)(pmrur >> 8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PMRUR,1),(uint8_t)pmrur);      \

#define getPMRUR() \
        (((uint16_t)(WIZCHIP_READ(PMRUR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PMRUR,1)))

#define setSHAR(shar) \
	    WIZCHIP_WRITE_BUF(SHAR,shar,6)

#define getSHAR(shar) \
	    WIZCHIP_READ_BUF(SHAR,shar,6)

#define setGAR(gar) \
	    WIZCHIP_WRITE_BUF(GAR,gar,4)

#define getGAR(gar) \
	    WIZCHIP_READ_BUF(GAR,gar,4)

#define setSUBR(subr) \
	    WIZCHIP_WRITE_BUF(SUBR,subr,4)

#define getSUBR(subr) \
	    WIZCHIP_READ_BUF(SUBR,subr,4)

#define setSIPR(sipr) \
	    WIZCHIP_WRITE_BUF(SIPR,sipr,4)

#define getSIPR(sipr) \
	    WIZCHIP_READ_BUF(SIPR,sipr,4)

#define setLLAR(llar) \
	    WIZCHIP_WRITE_BUF(LLAR,llar,16)

#define getLLAR(llar) \
	    WIZCHIP_READ_BUF(LLAR,llar,16)

#define setGUAR(guar) \
	    WIZCHIP_WRITE_BUF(GUAR,guar,16)

#define getGUAR(guar) \
	    WIZCHIP_READ_BUF(GUAR,guar,16)

#define setSUB6R(sub6r) \
	    WIZCHIP_WRITE_BUF(SUB6R,sub6r,16)

#define getSUB6R(sub6r) \
	    WIZCHIP_READ_BUF(SUB6R,sub6r,16)

#define setGA6R(ga6r) \
	    WIZCHIP_WRITE_BUF(GA6R,ga6r,16)

#define getGA6R(ga6r) \
	    WIZCHIP_READ_BUF(GA6R,ga6r,16)

void setSLPIPR(uint8_t * addr);

void getSLPIPR(uint8_t * addr);

void setSLPIP6R(uint8_t * addr);

void getSLPIP6R(uint8_t * addr);

#define setSLPHAR(slphar) \
	    WIZCHIP_WRITE_BUF(SLPHAR,slphar,6)

#define getSLPHAR(slphar) \
	    WIZCHIP_READ_BUF(SLPHAR,slphar,6)

#define setPINGIDR(pingidr) \
	    WIZCHIP_WRITE(PINGIDR,(uint8_t)(pingidr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PINGIDR,1),(uint8_t)pingidr);    

#define getPINGIDR(pingidr) \
	    (((int16_t)(WIZCHIP_READ(PINGIDR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PINGIDR,1)))

#define setPINGSEQR(pingseqr) \
		WIZCHIP_WRITE(PINGSEQR,(uint8_t)(pingseqr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PINGSEQR,1),(uint8_t)pingseqr);    \

#define getPINGSEQR(pingseqr) \
		(((int16_t)(WIZCHIP_READ(PINGSEQR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PINGSEQR,1)))

#define getUIPR(uipr) \
	    WIZCHIP_READ_BUF(UIPR, uipr, 4)

#define getUPORTR(uportr) \
	    WIZCHIP_READ_BUF(UPORTR,uportr,2)

#define getUIP6R(uip6r) \
	    WIZCHIP_READ_BUF(UIP6R,uip6r,16)

#define getUPORT6R(uport6r) \
	    WIZCHIP_READ_BUF(UPORT6R,uport6r,2)

#define setINTPTMR(intptmr) \
	    WIZCHIP_WRITE(INTPTMR,(uint8_t)(intptmr >> 8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(INTPTMR,1),(uint8_t)intptmr);      \

#define getINTPTMR() \
	    (((uint16_t)(WIZCHIP_READ(INTPTMR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(INTPTMR,1)))

#define getPLR() \
        WIZCHIP_READ(PLR)

#define getPFR() \
        WIZCHIP_READ(_PFR_)

#define getVLTR(vltr) \
        WIZCHIP_READ_BUF(VLTR, vltr, 4)

#define getPLTR(pltr) \
        WIZCHIP_READ_BUF(PLTR, pltr, 4)

#define getPAR(par) \
        WIZCHIP_READ_BUF(_PAR_, par, 16)

#define setICMP6BLKR(icmp6blkr) \
	    WIZCHIP_WRITE(ICMP6BLKR,icmp6blkr)

#define getICMP6BLKR() \
	    WIZCHIP_READ(ICMP6BLKR)

#define CHIPLOCK() \
        WIZCHIP_WRITE(CHPLCKR,0xff)

#define CHIPUNLOCK() \
	    WIZCHIP_WRITE(CHPLCKR,0xce)

#define NETLOCK() \
	    WIZCHIP_WRITE(NETLCKR,0xc5)

#define NETUNLOCK() \
	    WIZCHIP_WRITE(NETLCKR,0x3a)

#define PHYLOCK() \
	    WIZCHIP_WRITE(PHYLCKR,0xff)

#define PHYUNLOCK() \
	    WIZCHIP_WRITE(PHYLCKR,0x53)

#define setRTR(rtr) \
	    WIZCHIP_WRITE(_RTR_,(uint8_t)(rtr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(_RTR_,1),(uint8_t)rtr);      \

#define getRTR() \
	    (((uint16_t)(WIZCHIP_READ(_RTR_) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(_RTR_,1)))

#define setRCR(rcr) \
	    WIZCHIP_WRITE(_RCR_,rcr)

#define getRCR() \
	    WIZCHIP_READ(_RCR_)

#define setSLRTR(slrtr) \
			WIZCHIP_WRITE(SLRTR,(uint8_t)(slrtr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(SLRTR,1),(uint8_t)slrtr);

#define getSLRTR() \
			(((uint16_t)(WIZCHIP_READ(SLRTR) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(SLRTR,1)))

#define setSLRCR(slrcr) \
	    WIZCHIP_WRITE(SLRCR,slrcr)

#define getSLRCR() \
	    WIZCHIP_REDA(SLRCR)

#define setHOPR(hopr) \
	    WIZCHIP_WRITE(HOPR,hopr)

#define getHOPR(hopr) \
	    WIZCHIP_READ(HOPR)

//#define RNGSTR
//#define RNGCR
//#define RNGPLR
//#define RNGSDR
//#define RNGCHKR
//#define RNGVR

///////////////////////////////////
// Socket N register I/O function //
///////////////////////////////////
#define setSn_MR(sn,mr) \
	    WIZCHIP_WRITE(Sn_MR(sn),mr)
#define getSn_MR(sn) \
	    WIZCHIP_READ(Sn_MR(sn))

#define setSn_PRFR(sn,prfr) \
	    WIZCHIP_WRITE(Sn_PRFR(sn),prfr)
#define getSn_PRFR(sn) \
	    WIZCHIP_READ(Sn_PRFR(sn))

#define setSn_CR(sn,cr) \
	    WIZCHIP_WRITE(Sn_CR(sn),cr)
#define getSn_CR(sn) \
	    WIZCHIP_READ(Sn_CR(sn))

#define getSn_IR(sn) \
	    WIZCHIP_READ(Sn_IR(sn))

#define setSn_IMR(sn,imr) \
	    WIZCHIP_WIRTE(Sn_IMR(sn),imr)
#define getSn_IMR(sn) \
	    WIZCHIP_READ(Sn_IMR(sn))

#define setSn_IR(sn,ir) setSn_IRCLR(sn,ir)
#define setSn_IRCLR(sn,irclr) \
	    WIZCHIP_WRITE(Sn_IRCLR(sn),irclr)

#define getSn_SR(sn) \
	    WIZCHIP_READ(Sn_SR(sn))

#define getSn_ESR(sn) \
	    WIZCHIP_READ(Sn_ESR(sn))

#define setSn_PROTOR(sn,protor) \
	    WIZCHIP_WRITE(Sn_PROTOR(sn),protor)
#define getSn_PROTOR(sn) \
	    WIZCHIP_READ(Sn_PROTOR(sn))

#define setSn_TOSR(sn,tosr) \
	    WIZCHIP_WRITE(Sn_TOSR(sn),tosr)
#define getSn_TOSR(sn) \
	    WIZCHIP_READ(Sn_TOSR(sn))

#define setSn_TTLR(sn,ttlr) \
	    WIZCHIP_WRITE(Sn_TTLR(sn),ttlr)
#define getSn_TTLR(sn) \
	    WIZCHIP_READ(Sn_TTLR(sn))

#define setSn_FRGR(sn,frgr) \
	    WIZCHIP_WRITE_BUF(Sn_FRGR(sn),frgr,2)
#define getSn_FRGR(sn,frgr) \
	    WIZCHIP_READ_BUF(Sn_FRGR(sn),frgr,2)

#define setSn_MSSR(sn,mssr) \
	    WIZCHIP_WRITE(Sn_MSSR(sn),(uint8_t)(mssr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_MSSR(sn),1),(uint8_t)mssr);    \

#define getSn_MSSR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_MSSR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_MSSR(sn),1)))

#define setSn_PORT(sn,port) setSn_PORTR(sn,port)
#define setSn_PORTR(sn,portr) \
	    WIZCHIP_WRITE(Sn_PORTR(sn),(uint8_t)(portr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_PORTR(sn),1),(uint8_t)portr); \

#define getSn_PORT(sn) getSn_PORTR(sn)
#define getSn_PORTR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_PORTR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_PORTR(sn),1)))

#define setSn_DHAR(sn,dhar) \
	    WIZCHIP_WRITE_BUF(Sn_DHAR(sn),dhar,6)
#define getSn_DHAR(sn,dhar) \
	    WIZCHIP_READ_BUF(Sn_DHAR(sn),dhar,6)

#define setSn_DIPR(sn,dipr) \
	    WIZCHIP_WRITE_BUF(Sn_DIPR(sn),dipr,4)
#define getSn_DIPR(sn,dipr) \
	    WIZCHIP_READ_BUF(Sn_DIPR(sn),dipr,4)

#define setSn_DIP6R(sn,dip6r) \
	    WIZCHIP_WRITE_BUF(Sn_DIP6R(sn),dip6r,16)
#define getSn_DIP6R(sn,dip6r) \
	    WIZCHIP_READ_BUF(Sn_DIP6R(sn),dip6r,16)

#define setSn_DPORT(sn,dport) setSn_DPORTR(sn,dport)
#define setSn_DPORTR(sn,dportr) \
	    WIZCHIP_WRITE(Sn_DPORTR(sn),(uint8_t)(dportr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_DPORTR(sn),1),(uint8_t)dportr); \

#define getSn_DPORT(sn) getSn_DPORTR(sn)
#define getSn_DPORTR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_DPORTR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_DPORTR(sn),1)))

#define setSn_MR2(sn,mr2) \
        WIZCHIP_WRITE(Sn_MR2(sn),mr2)
#define getSn_MR2(sn) \
	    WIZCHIP_READ(Sn_MR2(sn))

#define setSn_RTR(sn,rtr) \
	    WIZCHIP_WRITE(Sn_RTR(sn),(uint8_t)(rtr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_RTR(sn),1),(uint8_t)rtr); \

#define getSn_RTR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_RTR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RTR(sn),1)))

#define setSn_RCR(sn,rcr) \
	    WIZCHIP_WRITE(Sn_RCR(sn),rcr)
#define getSn_RCR(sn) \
	    WIZCHIP_READ(Sn_RCR(sn))

#define setSn_KPALVTR(sn,katmr) \
	    WIZCHIP_WRITE(Sn_KPALVTR(sn),(uint8_t)(katmr>>8)); \
	    WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_KPALVTR(sn),1),(uint8_t)katmr); \

#define getSn_KPALVTR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_KPALVTR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_KPALVTR(sn),1)))

#define setSn_CSUMSKPR(sn, csumskpr) \
	    WIZCHIP_WRITE(Sn_CSUMSKPR(sn), csumskpr)
#define getSn_Sn_CSUMSKPR(sn) \
	    WIZCHIP_READ(Sn_CSUMSKPR(sn))


#define setSn_TxMAX(sn, tmsr) setSn_TX_MSR(sn, tmsr)
#define setSn_TX_MSR(sn,tmsr) \
	    WIZCHIP_WRITE(Sn_TX_MSR(sn),tmsr)
#define  setSn_TXBUF_SIZE(sn, tmsr) setSn_TX_MSR(sn,tmsr)

#define getSn_TxMAX(sn) getSn_TX_MSR(sn)
#define getSn_TX_MSR(sn) \
	    WIZCHIP_READ(Sn_TX_MSR(sn))
#define  getSn_TXBUF_SIZE(sn) getSn_TX_MSR(sn)

#define getSn_TX_FSR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_TX_FSR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1)))

#define getSn_TX_RD(sn) \
			(((uint16_t)WIZCHIP_READ(Sn_TX_RD(sn)) << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_RD(sn),1)))

#define setSn_TX_WR(sn,txwr) \
			WIZCHIP_WRITE(Sn_TX_WR(sn),   (uint8_t)(txwr>>8)); \
			WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_TX_WR(sn),1), (uint8_t) txwr);
#define getSn_TX_WR(sn) \
			(((uint16_t)WIZCHIP_READ(Sn_TX_WR(sn)) << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_WR(sn),1)))

#define setSn_RxMAX(sn, rmsr) setSn_RX_MSR(sn, rmsr)
#define setSn_RX_MSR(sn,rmsr) \
	    WIZCHIP_WRITE(Sn_RX_MSR(sn),rmsr)
#define  setSn_RXBUF_SIZE(sn,rmsr) setSn_RX_MSR(sn,rmsr)

#define getSn_RxMAX(sn) getSn_RX_MSR(sn)
#define getSn_RX_MSR(sn) \
	    WIZCHIP_READ(Sn_RX_MSR(sn))
#define  getSn_RXBUF_SIZE(sn) getSn_RX_MSR(sn)

//#define getSn_RX_RSR(sn) \
	    (((uint16_t)(WIZCHIP_READ(Sn_RX_RSR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1)))
uint16_t getSn_RX_RSR(uint8_t s);

#define setSn_RX_RD(sn,rxrd) \
	    WIZCHIP_WRITE(Sn_RX_RD(sn),   (uint8_t)(rxrd>>8)); \
			WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_RX_RD(sn),1), (uint8_t) rxrd) ;

#define getSn_RX_RD(sn) \
	    (((uint16_t)WIZCHIP_READ(Sn_RX_RD(sn)) << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RD(sn),1)))

#define getSn_RX_WR(sn) \
			(((uint16_t)(WIZCHIP_READ(Sn_RX_WR(sn)) << 8)) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_WR(sn),1)))
			
#define	NETCFG_LOCK()	\
			WIZCHIP_WRITE(NETLCKR, 0x00); // NETCFG lock!!
#define	NETCFG_UNLOCK()	\
			WIZCHIP_WRITE(NETLCKR, 0x3A); // NETCFG Unlock!!


#define	clearIR(val)	\
			WIZCHIP_WRITE(IRCLR, val)
#define	clearSn_IR( s, val )	\
			WIZCHIP_WRITE(Sn_IRCLR(s), val)


// for others
#define MR_RST 0x00
//#define IPPROTO_ICMP 0x00
//#define Sn_MR2_UUBLK 0x00
//#define MR2_CLKSEL 0x00
//#define MR2_G_IEN 0x00
#define Sn_CR_DISCON 0x00
///#define MR2_WOL 0x00
//#define Sn_CR_SEND_MAC 0x00
//#define Sn_MR_MC 0x00
//#define Sn_MR_SEND_KEEP 0x00

/////////////////////////////////////
// Sn_TXBUF & Sn_RXBUF IO function //
/////////////////////////////////////
/**
 * @ingroup Basic_IO_function_W6100
 * @brief It copies data to internal TX memory
 *
 * @details This function reads the Tx write pointer register and after that,
 * it copies the <i>wizdata(pointer buffer)</i> of the length of <i>len(variable)</i> bytes to internal TX memory
 * and updates the Tx write pointer register.
 * This function is being called by send() and sendto() function also.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param wizdata Pointer buffer to write data
 * @param len Data length
 * @sa wiz_recv_data()
 */
void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len);

/**
 * @ingroup Basic_IO_function_W6100
 * @brief It copies data to your buffer from internal RX memory
 *
 * @details This function read the Rx read pointer register and after that,
 * it copies the received data from internal RX memory
 * to <i>wizdata(pointer variable)</i> of the length of <i>len(variable)</i> bytes.
 * This function is being called by recv() also.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param wizdata Pointer buffer to read data
 * @param len Data length
 * @sa wiz_send_data()
 */
void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len);

/**
 * @ingroup Basic_IO_function_W6100
 * @brief It discard the received data in RX memory.
 * @details It discards the data of the length of <i>len(variable)</i> bytes in internal RX memory.
 * @param (uint8_t)sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param len Data length
 */
void wiz_recv_ignore(uint8_t sn, uint16_t len);

/**
 * @ingroup Special_function_W6100
 * @brief Write data to the PHY via MDC/MDIO interface.
 * @details Write command data to the PHY via MDC/MDIO interface.
 * @param (uint8_t)PHYMDIO_regadr Address of the PHY register. It should be PHYMDIO_BMCR or PHYMDIO_BMSR.
 * @param (uint16_t)var Data to write to the PHY register. Please refer to the bit definitions of the BMCR and BMSR register.
 */
void wiz_mdio_write(uint8_t PHYMDIO_regadr, uint16_t var);

/**
 * @ingroup Special_function_W6100
 * @brief Read data from the PHY via MDC/MDIO interface.
 * @details Read command or status data from the PHY via MDC/MDIO interface.
 * @param (uint8_t)PHYMDIO_regadr Address of the PHY register. It should be PHYMDIO_BMCR or PHYMDIO_BMSR.
 * @return The value of the PHY register
 */
uint16_t wiz_mdio_read(uint8_t PHYMDIO_regadr);

/**
 * @ingroup Special_function_W6100
 * @brief Delay function
 * @details Delay function using internal 100us timer of the W6100
 * @param (uint32_t)ms Time to delay in milliseconds.
 */
void wiz_delay_ms(uint32_t ms);

/// @cond DOXY_APPLY_CODE
#endif
/// @endcond

void Interface_init(void);

/**
 * @ingroup extra_functions
 * @brief Get Destination address
 * @details @ref _RTR_ configures the retransmission timeout period and @ref _RCR_ configures the number of time of retransmission.  
 * @param dest_addr @ref Sn_ESR value and @ref Sn_DIPR value and @ref Sn_DIP6R.
 */
 
int8_t getDestAddr(uint8_t sn, uint8_t * dest_addr);


#endif //_W6100_H_



