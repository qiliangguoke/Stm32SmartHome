#ifndef __BSP_UART4_H
#define __BSP_UART4_H
#include "sys.h"

#define UART4_BUF_LEN 64 //定义 UART4 缓冲长度为 64 个字节

extern uint8_t UART4_RX_BUF[UART4_BUF_LEN]; //接收缓冲,最大 UART4_BUF_LEN 个字节
extern uint8_t UART4_RX_CNT; //接收到的数据长度

//如果想串口中断接收，请不要注释以下宏定义
#define EN_UART4_RX 1 //0,不接收;1,接收.

void bsp_InitUart4(uint32_t bound);
void UART4_Send_Data(uint8_t *buf,uint8_t len);
void UART4_Receive_Data(uint8_t *buf,uint8_t *len);
#endif
