#ifndef __BSP_UART_H
#define __BSP_UART_H


///////////////////////////////////////
/* 头文件包含区 */
#include"sys.h"
///////////////////////////////////////


/* 宏定义区域 */
#define USART_REC_LEN               1024   //定义最大接受字节数  1024
#define EN_USART1_RX                1      //使能（1）/禁止（0）串口1接收
///////////////////////////////////////


/* 外部变量申明区 */
extern uint8_t  USART_RX_BUF[USART_REC_LEN];       //接收换船，最大USART_REC_LEN个字节，末字节位换行符

extern uint8_t  ReceiveState;
extern uint16_t RxCounter;	
///////////////////////////////////////
/* 函数申明区 */
void bsp_InitUart1(uint32_t baud);

#if EN_USART1_RX   //如果使能了接收
void Uart0_STA_Clr(void);
void USART1_Send_Data(uint8_t *buf,uint8_t len);
#endif
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
