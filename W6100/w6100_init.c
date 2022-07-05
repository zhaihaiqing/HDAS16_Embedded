/***************************************************************************************
成都浩然电子有限公司
WIZnet官方代理商，09年起一直蝉联WIZnet销量第一名，为客户提供技术、产品、售后等全方位服务
公司网址：http://www.hschip.com

浩然成都
地址：四川省成都市高新区天和路69号皇城花卉8栋2503室
热线电话:028-86127089,86120475
商务QQ : 402856564,577863143
传真:028-86127039
邮政编码:610041

浩然深圳
地址: 深圳市福田区皇岗路高科利大厦A座28B 
热线电话:0755-86066647, 18575510340
商务QQ  :2728050853,2541214655,2716561615
邮政编码:518000

官方淘宝   
电脑端:http://shop325030069.taobao.com/index.htm
手机端:https://shop325030069.m.taobao.com

技术支持
e-mail:support@hschip.com 
技术QQ:2232725509 
技术交流QQ群：722479032

硬件平台：浩然电子评估板  HS-EVBW6100/STM32    (HS-EVBW5100S/STM32底板全兼容)

SPI模式或间接总线模式通过 wizchip_conf.h 文件的
//   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_BUS_INDIR_     //  间接总线
   #define _WIZCHIP_IO_MODE_         _WIZCHIP_IO_MODE_SPI_VDM_           //  SPI模式
来进行选择
***************************************************************************************/

#include "w6100_init.h"
#include "main.h"

uint16 ATOI(char* str,uint16 base	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}

uint32 ATOI32(char* str,uint16 base	)
{
  uint32 num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}


void itoa(uint16 n,uint8 str[5], uint8 len)
{
  
  uint8 i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}

int ValidATOI(char* str, int base,int* ret)
{
  int c;
  char* tstr = str;
  if(str == 0 || *str == '\0') return 0;
  while(*tstr != '\0')
  {
    c = C2D(*tstr);
    if( c >= 0 && c < base) tstr++;
    else    return 0;
  }
  
  *ret = ATOI(str,base);
  return 1;
}
 
void replacetochar(char * str,	char oldchar,char newchar	)
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar) str[x] = newchar;	
}

char C2D(uint8 c	)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}

uint16 swaps(uint16 i)
{
  uint16 ret=0;
  ret = (i & 0xFF) << 8;
  ret |= ((i >> 8)& 0xFF);
  return ret;	
}

uint32 swapl(uint32 l)
{
  uint32 ret=0;
  ret = (l & 0xFF) << 24;
  ret |= ((l >> 8) & 0xFF) << 16;
  ret |= ((l >> 16) & 0xFF) << 8;
  ret |= ((l >> 24) & 0xFF);
  return ret;
}

//get mid str
void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
	int8* sub1;
	int8* sub2;
	uint16 n;

  sub1=strstr(src,s1);
  sub1+=strlen(s1);
  sub2=strstr(sub1,s2);
  n=sub2-sub1;
  strncpy(sub,sub1,n);
  sub[n]=0;
}
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
	int i;
//	u_long inetaddr = 0;
	char taddr[30];
	char * nexttok;
	char num;
	strcpy(taddr,(char *)addr);
	
	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = strtok(nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = ATOI(nexttok+2,0x10);
		else num = ATOI(nexttok,10);
		
		ip[i] = num;
		nexttok = NULL;
	}
}	
/**
@brief	Convert 32bit Address(Host Ordering) into Dotted Decimal Format
@return 	a char pointer to a static buffer containing the text address in standard ".'' notation. Otherwise, it returns NULL. 
*/  
char* inet_ntoa(
	unsigned long addr	/**< Pointer variable to store converted value(INPUT) */
	)
{
	static char addr_str[32];
	memset(addr_str,0,32);
	sprintf(addr_str,"%d.%d.%d.%d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
	return addr_str;
}

char* inet_ntoa_pad(unsigned long addr)
{
	static char addr_str[16];
	memset(addr_str,0,16);
	printf(addr_str,"%03d.%03d.%03d.%03d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
	return addr_str;
}


/**
@brief	Verify dotted notation IP address string
@return 	success - 1, fail - 0
*/ 
char VerifyIPAddress_orig(
	char* src	/**< pointer to IP address string */
	)
{
	int i;
	int tnum;
	char tsrc[50];
	char* tok = tsrc;
	
	strcpy(tsrc,src);
	
	for(i = 0; i < 4; i++)
	{
		tok = strtok(tok,".");
		if ( !tok ) return 0;
		if(tok[0] == '0' && tok[1] == 'x')
		{
			if(!ValidATOI(tok+2,0x10,&tnum)) return 0;
		}
		else if(!ValidATOI(tok,10,&tnum)) return 0;
		
		if(tnum < 0 || tnum > 255) return 0;
		tok = NULL;
	}
	return 1;	
}

char VerifyIPAddress(char* src, uint8 * ip)
{
	int i;
	int tnum;
	char tsrc[50];
	char* tok = tsrc;
	
	strcpy(tsrc,src);
	
	for(i = 0; i < 4; i++)
	{
		tok = strtok(tok,".");
		if ( !tok ) return 0;
		if(tok[0] == '0' && tok[1] == 'x')
		{
			if(!ValidATOI(tok+2,0x10,&tnum)) return 0;
		}
		else if(!ValidATOI(tok,10,&tnum)) return 0;

		ip[i] = tnum;
		
		if(tnum < 0 || tnum > 255) return 0;
		tok = NULL;
	}
	return 1;	
}



/**
@brief	htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/ 
uint16 htons( 
	uint16 hostshort	/**< A 16-bit number in host byte order.  */
	)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swaps(hostshort);
#else
	return hostshort;
#endif		
}


/**
@brief	htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/ 
unsigned long htonl(
	unsigned long hostlong		/**< hostshort  - A 32-bit number in host byte order.  */
	)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swapl(hostlong);
#else
	return hostlong;
#endif	
}
/**
@brief	ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/ 
unsigned long ntohs(
	unsigned short netshort	/**< netshort - network odering 16bit value */
	)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )	
	return htons(netshort);
#else
	return netshort;
#endif		
}


/**
@brief	converts a unsigned long from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/ 
unsigned long ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return htonl(netlong);
#else
	return netlong;
#endif		
}

// destip : BigEndian
u_char CheckDestInLocal(u_long destip)
{
	int i = 0;
	u_char * pdestip = (u_char*)&destip;
	for(i =0; i < 4; i++)
	{
		if((pdestip[i] & WIZCHIP_READ(W6100_SUBR+i)) != (WIZCHIP_READ(W6100_SIPR+i) & WIZCHIP_READ(W6100_SUBR+i)))
			return 1;	// Remote
	}
	return 0;
}


/**
@brief	Get handle of socket which status is same to 'status'
@return 	socket number
*/ 
SOCKET getSocket(
	unsigned char status, 	/**< socket's status to be found */
	SOCKET start			/**< base of socket to be found */
	)
{
	SOCKET i;
	if(start > 3) start = 0;

	for(i = start; i < MAX_SOCK_NUM ; i++) if( getSn_SR(i)==status ) return i;
	return MAX_SOCK_NUM;	
}


/**
@brief	Calculate checksum of a stream
@return 	checksum
*/ 
unsigned short checksum(
	unsigned char * src, 	/**< pointer to stream  */
	unsigned int len		/**< size of stream */
	)
{
	u_int sum, tsum, i, j;
	u_long lsum;

	j = len >> 1;

	lsum = 0;

	for (i = 0; i < j; i++) 
	{
		tsum = src[i * 2];
		tsum = tsum << 8;
		tsum += src[i * 2 + 1];
		lsum += tsum;
	}

	if (len % 2) 
	{
		tsum = src[i * 2];
		lsum += (tsum << 8);
	}


	sum = lsum;
	sum = ~(sum + (lsum >> 16));
	return (u_short) sum;	
}




void Reset_W6100(void) // 2018-07-17
{
 // int i;
		HAL_GPIO_WritePin(W_RSTN_GPIO_Port,W_RSTN_Pin,GPIO_PIN_RESET);
		rt_thread_mdelay(50);
		HAL_GPIO_WritePin(W_RSTN_GPIO_Port,W_RSTN_Pin,GPIO_PIN_SET);
		rt_thread_mdelay(1000);	
}





void Set_Network(void)  // 2020-03-11
{
	uint8_t tmp_array[16];
	uint8_t mac[6]={0x48,0x53,0x0b,0x00,0x11,0x22};   // MAC地址
  uint8_t lip[4]={192,168,1,10};                    // 本机IPV4地址  
  uint8_t sub[4]={255,255,255,0};                   // 子网掩码
  uint8_t gw[4]={192,168,1,1};                      // 网关
  //uint8 dns[4]={8,8,8,8};
	uint8_t txsize[8] ={2,2,2,2,2,2,2,2};   // 每个socket的发送缓慢配置为2K.
	uint8_t rxsize[8] ={2,2,2,2,2,2,2,2};   // 每个socket的接收缓存配置为2K 
	uint8_t ip[6]={0,0,0,0};
	
	
	
  uint8_t lla[16]={0xfe, 0x80, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00,
							0x75, 0x03, 0x1b, 0x81,
							0xf3, 0xee, 0x0c, 0x4e};   ///< 本机IPV6 地址
	uint8_t gua[16]={0x20,0x01,0x02,0xb8,
									0x00,0x10,0x00,0x01,
									0x02,0x08,0xdc,0xff,
									0xfe,0x57,0x57,0x25};   ///< 全局单播地址
	uint8_t sn6[16]={0xff,0xff,0xff,0xff,
									0xff,0xff,0xff,0xff,
									0x00,0x00,0x00, 0x00,
									0x00,0x00,0x00,0x00 };   ///< IPv6 子网前缀
	uint8_t gw6[16]={0xfe, 0x80, 0x00,0x00,
									0x00,0x00,0x00,0x00,
									0x02,0x00, 0x87,0xff,
									0xfe,0x08, 0x4c,0x81};   ///< 网关IPV6 地址

									
	setNETLCKR(0X3A);//Network Unlock before set Network Information 
	
	/*set ip mac sub gateway */
	setSHAR(mac);       // MAC地址
  setSUBR(sub);       // 子网掩码
  setGAR(gw);         // 网关   
  setSIPR(lip);       // 本机IPV4地址 
									
  setGA6R(gw6);       //< 网关IPV6 地址设置 
  setSUB6R(sn6);     // IPv6子网前缀设置
  setLLAR(lla);      // Link本地地址设置
  setGUAR(gua);      // 全局单播地址设置

  wizchip_init( txsize, rxsize);   //  初始发送和接收缓存
  setRTR(4000);			// 设置超时为400ms																
  setRCR(3);				// 设置重传次数3次																
  
}



void LOG_Net_info(void)
{
	uint8_t tmp_array[16];
	uint8_t ip[6]={0,0,0,0};
	
	log_info("VERSION() = %.2x \r\n",getVER());
	
	//以下将以上设置的参数读出来验证是否正确?
  getSHAR(ip);
	log_info("MAC : %x.%x.%x.%x.%x.%x\r\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);
  getSIPR (ip);
  log_info("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getSUBR(ip);
  log_info("SN : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getGAR(ip);
  log_info("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
	setNETLCKR(0x00);
	getGA6R(tmp_array);
    log_info("GW6 : %04X:%04X", ((uint16_t)tmp_array[0] << 8) | ((uint16_t)tmp_array[1]),
    		((uint16_t)tmp_array[2] << 8) | ((uint16_t)tmp_array[3]));
    log_info(":%04X:%04X", ((uint16_t)tmp_array[4] << 8) | ((uint16_t)tmp_array[5]),
    		((uint16_t)tmp_array[6] << 8) | ((uint16_t)tmp_array[7]));
    log_info(":%04X:%04X", ((uint16_t)tmp_array[8] << 8) | ((uint16_t)tmp_array[9]),
    		((uint16_t)tmp_array[10] << 8) | ((uint16_t)tmp_array[11]));
    log_info(":%04X:%04X\r\n", ((uint16_t)tmp_array[12] << 8) | ((uint16_t)tmp_array[13]),
    		((uint16_t)tmp_array[14] << 8) | ((uint16_t)tmp_array[15]));

	getLLAR(tmp_array);
	log_info("LLA : %04X:%04X", ((uint16_t)tmp_array[0] << 8) | ((uint16_t)tmp_array[1]),
			((uint16_t)tmp_array[2] << 8) | ((uint16_t)tmp_array[3]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[4] << 8) | ((uint16_t)tmp_array[5]),
			((uint16_t)tmp_array[6] << 8) | ((uint16_t)tmp_array[7]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[8] << 8) | ((uint16_t)tmp_array[9]),
			((uint16_t)tmp_array[10] << 8) | ((uint16_t)tmp_array[11]));
	log_info(":%04X:%04X\r\n", ((uint16_t)tmp_array[12] << 8) | ((uint16_t)tmp_array[13]),
			((uint16_t)tmp_array[14] << 8) | ((uint16_t)tmp_array[15]));
	getGUAR(tmp_array);
	log_info("GUA : %04X:%04X", ((uint16_t)tmp_array[0] << 8) | ((uint16_t)tmp_array[1]),
			((uint16_t)tmp_array[2] << 8) | ((uint16_t)tmp_array[3]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[4] << 8) | ((uint16_t)tmp_array[5]),
			((uint16_t)tmp_array[6] << 8) | ((uint16_t)tmp_array[7]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[8] << 8) | ((uint16_t)tmp_array[9]),
			((uint16_t)tmp_array[10] << 8) | ((uint16_t)tmp_array[11]));
	log_info(":%04X:%04X\r\n", ((uint16_t)tmp_array[12] << 8) | ((uint16_t)tmp_array[13]),
			((uint16_t)tmp_array[14] << 8) | ((uint16_t)tmp_array[15]));

	getSUB6R(tmp_array);
	log_info("SUB6 : %04X:%04X", ((uint16_t)tmp_array[0] << 8) | ((uint16_t)tmp_array[1]),
			((uint16_t)tmp_array[2] << 8) | ((uint16_t)tmp_array[3]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[4] << 8) | ((uint16_t)tmp_array[5]),
			((uint16_t)tmp_array[6] << 8) | ((uint16_t)tmp_array[7]));
	log_info(":%04X:%04X", ((uint16_t)tmp_array[8] << 8) | ((uint16_t)tmp_array[9]),
			((uint16_t)tmp_array[10] << 8) | ((uint16_t)tmp_array[11]));
	log_info(":%04X:%04X\r\n", ((uint16_t)tmp_array[12] << 8) | ((uint16_t)tmp_array[13]),
			((uint16_t)tmp_array[14] << 8) | ((uint16_t)tmp_array[15]));
			
}











