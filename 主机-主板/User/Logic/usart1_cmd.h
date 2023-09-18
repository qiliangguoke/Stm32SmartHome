/*
*********************************************************************************************************
*
*	模块名称 : usart1_cmd模块
*	文件名称 : usart1_cmd.h
*	说    明 : usart1_cmd协议控制
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef __USART1_CMD_H_
#define __USART1_CMD_H_

//////////////////////////////////////
//头文件区

#include "bsp.h"

//////////////////////////////////
//变量区

//帧头字节长度
#define FRAME_HEADER_LEN		3
//帧数据字节长度
#define FRAME_DATA_LEN			1
//帧校验字节长度
#define FRAME_CHECKSUM_LEN		1

//帧固定的字节长度
#define FRAME_CONST_BYTE_LEN	( FRAME_HEADER_LEN + FRAME_DATA_LEN + FRAME_CHECKSUM_LEN )

//帧的数据区起始地址
#define FRAME_DATA_FIELD_OFFSET	(FRAME_HEADER_LEN + FRAME_DATA_LEN )

/**********************************************************/

#define FRAME_HEADER_BYTE_1st	0x44
#define FRAME_HEADER_BYTE_2nd	0x5A
#define FRAME_HEADER_BYTE_3rd	0x4C


#define FUN_CODE_CMD		0x01
#define FUN_CODE_RSP		0x02


//#define FUNC_CODE_LEN				1
//#define FUNC_CODE_DEVICE_LEN		1
//#define FUNC_CODE_DEVICE_NUM_LEN	1

//#define FUNC_CODE_OFFSET			FRAME_DATA_FIELD_OFFSET
//#define FUNC_CODE_DEVICE_OFFSET		( FRAME_DATA_FIELD_OFFSET + FUNC_CODE_LEN )


//设备为继电器
#define FUN_CODE_DEVICE_RELAY		0x01
//继电器闭合命令
#define FUN_CODE_CMD_RELAY_ON		0x01
//继电器断开命令
#define FUN_CODE_CMD_RELAY_OFF		0x02
//继电器状态翻转命令
#define FUN_CODE_CMD_RELAY_TOGGLE	0x03
//继电器状态查询命令
#define FUN_CODE_CMD_RELAY_QUERY	0x04
/**********************************************************/
//继电器状态闭合反馈
#define FUN_CODE_RSP_RELAY_ON		0xFF
//继电器状态断开反馈
#define FUN_CODE_RSP_RELAY_OFF		0x00


//////////////////////////////////
//函数申明区
void usart1_cmd(void);
uint8_t CheckSum(uint8_t *buf,uint16_t len);

#endif


/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
