/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_


/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
//#define BSP_Printf		printf
#define BSP_Printf(...)

//#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* 按需增加头文件区域 START OF AREA*/

#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_uart.h"
#include "bsp_iwdg.h"
#include "bsp_wwdg.h"
#include "bsp_tim_pwm.h"
#include "bsp_tpad.h"
#include "bsp_adc.h"
#include "bsp_uart4.h"
#include "bsp_co2.h"
#include "bsp_iosensor.h"
#include "bsp_nt35310_lcd.h"
#include "bsp_i2c_gpio.h"
#include "bsp_24c02.h"
#include "bsp_sht30.h"
#include "sensor_Data_Proc.h"
#include "bsp_relay.h"
#include "bsp_fan.h"
#include "bsp_opt3001.h"
#include "bsp_uart5.h"
#include "bsp_pm2.5.h"
#include "bsp_uart2.h"
/* 按需增加头文件区域 END OF AREA */


/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);

#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
