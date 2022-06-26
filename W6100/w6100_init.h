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
#ifndef __W6100_INIT_H
#define __W6100_INIT_H
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "types.h"

#include "W6100.h"
#define ON										1
#define OFF										0
#define HIGH									1
#define LOW										0


//  2018-07-17 11:42



void LOG_Net_info(void);
void Set_Network(void);

void Reset_W6100(void);



uint16 ATOI(char* str,uint16 base); 			/* Convert a string to integer number */
uint32 ATOI32(char* str,uint16 base); 			/* Convert a string to integer number */
void itoa(uint16 n,uint8* str, uint8 len);
int ValidATOI(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char C2D(u_char c); 					/* Convert a character to HEX */

uint16 swaps(uint16 i);
uint32 swapl(uint32 l);

void replacetochar(char * str, char oldchar, char newchar);

void mid(int8* src, int8* s1, int8* s2, int8* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);

char* inet_ntoa(unsigned long addr);			/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

//unsigned long inet_addr(unsigned char* addr);		/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */

char VerifyIPAddress_orig(char* src);			/* Verify dotted notation IP address string */
char VerifyIPAddress(char* src, uint8 * ip);
unsigned long GetDestAddr(SOCKET s);			/* Output destination IP address of appropriate channel */
unsigned int GetDestPort(SOCKET s);			/* Output destination port number of appropriate channel */
unsigned short htons( unsigned short hostshort);	/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/
unsigned long htonl(unsigned long hostlong);		/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */
unsigned long ntohs(unsigned short netshort);		/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */
unsigned long ntohl(unsigned long netlong);		/* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */
u_char CheckDestInLocal(u_long destip);			/* Check Destination in local or remote */

SOCKET getSocket(unsigned char status, SOCKET start); 	/* Get handle of socket which status is same to 'status' */
unsigned short checksum(unsigned char * src, unsigned int len);		/* Calculate checksum of a stream */

#endif
