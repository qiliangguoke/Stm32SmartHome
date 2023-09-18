/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : .h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/
#ifndef __BSP_UART5_H
#define __BSP_UART5_H

#include "sys.h"

#define UART5_BUF_LEN 64 					//定义 UART4 缓冲长度为 64 个字节

extern uint8_t UART5_RX_BUF[UART5_BUF_LEN]; //接收缓冲,最大 UART4_BUF_LEN 个字节
extern uint8_t UART5_RX_CNT;				//接收到的数据长度

//如果想串口中断接收，请不要注释以下宏定义
#define EN_UART5_RX 1 						//0,不接收;1,接收.

void bsp_InitUART5(uint32_t bound);
void UART5_Send_Data(uint8_t *buf,uint8_t len);
void UART5_Receive_Data(uint8_t *buf,uint8_t *len);


#endif
