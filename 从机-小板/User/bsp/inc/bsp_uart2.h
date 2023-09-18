/*
*********************************************************************************************************
*
*	模块名称 : xxx指示灯驱动模块
*	文件名称 : bsp_xxx.h
*	版    本 : V1.0
*	说    明 : 头文件，配合新建工程使用，非完整文件
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef __BSP_USART2_H
#define __BSP_USART2_H

#include "sys.h"

#define USART2_BUF_LEN	64	//定义UART4缓冲长度为64个字节

extern uint8_t USART2_RX_BUF[USART2_BUF_LEN];//接收缓冲，最大UART4_BUF_LEN个字节
extern uint8_t USART2_RX_CNT;//接收到的数据长度

///////////////////////////////////////////////////////////
//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTAG引脚，需要打开AFIO时钟，并失能JTAG)

#define EN_USART2_RX	1	//0，不接收，1接收

void bsp_InitUart2(uint32_t bound);
void USART2_Send_Data(uint8_t *buf,uint8_t len);
void USART2_Receive_Data(uint8_t *buf,uint8_t *len);
	

/////////////////////////////////////////////////////////////////////////////////////////////////




/* 供外部调用的函数声明 */


#endif
