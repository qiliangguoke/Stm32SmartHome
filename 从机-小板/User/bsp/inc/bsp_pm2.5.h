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

#ifndef __BSP_PM25_H
#define __BSP_PM25_H

#include "sys.h"

#define PM25_TEMP_BUF_LEN	10	//定义CO2_TEMP_BUF_LEN缓冲长度为10个字节

///////////////////////////////////////////////////////////
//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTAG引脚，需要打开AFIO时钟，并失能JTAG)

//CO2读数据
uint16_t PM25_READ(void);



/* 供外部调用的函数声明 */


#endif
