
#ifndef  __LOOPBACK_H_
#define  __LOOPBACK_H_

#include "stm32f4xx.h"	

#define DATA_BUF_SIZE  		 2048//w5500共有16k的接收缓存，假设这里每个socket分得2k，即2048byte

#define RECV_MAX_DATALEN   128//每个socket平均分配2K缓存的情况下

#define CLIENT_MAXNUM      4//定义客户端数量

int32_t loopback_tcps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_udps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_server(uint8_t, uint8_t*, uint16_t);
int32_t loopback_client(uint8_t sn, uint8_t* buf,uint8_t *ip, uint16_t port);

#endif   // _WIZCHIP_LOOPBACK_H_
