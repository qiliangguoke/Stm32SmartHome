#ifndef __BSP_IOSENSOR_H
#define __BSP_IOSENSOR_H

#include "sys.h"

///////////////////////////////////////////////////////////

//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非 JTAG/SWD 引脚，如果是 JTAG 引脚，需要打开 AFIO 时钟，并失能 JTAG)



///////////////////////
//DO101 震动传感器
#define VIBRATE_OUT_GPIO_PIN 			GPIO_Pin_11 			//VIBRATE_OUT 引脚号
#define VIBRATE_OUT_PIN_ID 				11 						//VIBRATE_OUT 引脚序号
#define VIBRATE_OUT_GPIO_PORT 			GPIOA 					//VIBRATE_OUT 端口号
#define VIBRATE_OUT_GPIO_CLK 			RCC_APB2Periph_GPIOA 	//VIBRATE_OUT 时钟
//#define VIBRATE_OUT_FUN_OUT 		PAout 					//VIBRATE_OUT 输出端口配置函数

#define VIBRATE_OUT_FUN_IN 				PAin 					//VIBRATE_OUT 输入端口配置函数
#define VIBRATE_OUT_GPIO_MODE 			GPIO_Mode_IPU 			//VIBRATE_OUT 外部中断输入模式
#define VIBRATE_OUT_ACTIVE_LEVEL		0 						//VIBRATE_OUT 有效电平为低电平
#define VIBRATE_OUT_PORT_SRC 			GPIO_PortSourceGPIOA	//VIBRATE_OUT 外部中断端口号
#define VIBRATE_OUT_PIN_SRC 			GPIO_PinSource11 		//VIBRATE_OUT 外部中断引脚号
#define VIBRATE_OUT_EXTI_LINE 			EXTI_Line11 			//VIBRATE_OUT 外部中断号
#define VIBRATE_OUT_EXTI_TRIG 			EXTI_Trigger_Falling 	//VIBRATE_OUT 外部中断触发方式
#define VIBRATE_OUT_EXTI_IRQN 			EXTI15_10_IRQn 			//VIBRATE_OUT 外部中断 NVIC 号

///////////////////////
//PIR 人体红外传感器
#define PIR_OUT_GPIO_PIN 				GPIO_Pin_3 			//PIR_OUT 引脚号
#define PIR_OUT_PIN_ID 					3 						//PIR_OUT 引脚序号
#define PIR_OUT_GPIO_PORT 				GPIOB 					//PIR_OUT 端口号
#define PIR_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOB 	//PIR_OUT 时钟
//#define PIR_OUT_FUN_OUT 		PBout //PIR_OUT 输出端口配置函数

#define PIR_OUT_FUN_IN 					PBin 					//PIR_OUT 输入端口配置函数
#define PIR_OUT_GPIO_MODE 				GPIO_Mode_IPD 			//PIR_OUT 外部中断输入模式
#define PIR_OUT_ACTIVE_LEVEL 			1 //PIR_OUT 有效电平为高电平
#define PIR_OUT_PORT_SRC 				GPIO_PortSourceGPIOB 	//PIR_OUT 外部中断端口号
#define PIR_OUT_PIN_SRC 				GPIO_PinSource3 		//PIR_OUT 外部中断引脚号
#define PIR_OUT_EXTI_LINE 				EXTI_Line3 			//PIR_OUT 外部中断号
#define PIR_OUT_EXTI_TRIG 				EXTI_Trigger_Rising 	//PIR_OUT 外部中断触发方式
#define PIR_OUT_EXTI_IRQN 				EXTI3_IRQn 			//PIR_OUT 外部中断 NVIC 号

///////////////////////
//DO101 火焰传感器
#define HY_OUT_GPIO_PIN 				GPIO_Pin_12 			//HY_OUT 引脚号
#define HY_OUT_PIN_ID 					12						//HY_OUT 引脚序号
#define HY_OUT_GPIO_PORT 				GPIOA 					//HY_OUT 端口号
#define HY_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOA 	//HY_OUT 时钟
//#define HY_OUT_FUN_OUT 			PAout 					//HY_OUT 输出端口配置函数

#define HY_OUT_FUN_IN 					PAin 					//HY_OUT 输入端口配置函数
#define HY_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//HY_OUT 外部中断输入模式
#define HY_OUT_ACTIVE_LEVEL				0 						//HY_OUT 有效电平为低电平
#define HY_OUT_PORT_SRC 				GPIO_PortSourceGPIOA	//HY_OUT 外部中断端口号
#define HY_OUT_PIN_SRC 					GPIO_PinSource12 		//HY_OUT 外部中断引脚号
#define HY_OUT_EXTI_LINE 				EXTI_Line12 			//HY_OUT 外部中断号
#define HY_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//HY_OUT 外部中断触发方式
#define HY_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//HY_OUT 外部中断 NVIC 号


//DO211 霍尔传感器输出
//#define HE_OUT_GPIO_PIN 				GPIO_Pin_13			//HE_OUT 引脚号
//#define HE_OUT_PIN_ID 					13						//HE_OUT 引脚序号
//#define HE_OUT_GPIO_PORT 				GPIOC					//HE_OUT 端口号
//#define HE_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOC 	//HE_OUT 时钟
////#define HE_OUT_FUN_OUT 			PCout 					//HE_OUT 输出端口配置函数

//#define HE_OUT_FUN_IN 					PCin 					//HE_OUT 输入端口配置函数
//#define HE_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//HE_OUT 外部中断输入模式
//#define HE_OUT_ACTIVE_LEVEL				0 						//HE_OUT 有效电平为低电平
//#define HE_OUT_PORT_SRC 				GPIO_PortSourceGPIOC	//HE_OUT 外部中断端口号
//#define HE_OUT_PIN_SRC 					GPIO_PinSource13 		//HE_OUT 外部中断引脚号
//#define HE_OUT_EXTI_LINE 				EXTI_Line13			//HE_OUT 外部中断号
//#define HE_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//HE_OUT 外部中断触发方式
//#define HE_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//HE_OUT 外部中断 NVIC 号


//DO101 可燃气体传感器输出
#define KR_OUT_GPIO_PIN 				GPIO_Pin_15			//KR_OUT 引脚号
#define KR_OUT_PIN_ID 					15						//KR_OUT 引脚序号
#define KR_OUT_GPIO_PORT 				GPIOA 					//KR_OUT 端口号
#define KR_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOA 	//KR_OUT 时钟
//#define KR_OUT_FUN_OUT 			PAout 					//KR_OUT 输出端口配置函数

#define KR_OUT_FUN_IN 					PAin 					//KR_OUT 输入端口配置函数
#define KR_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//KR_OUT 外部中断输入模式
#define KR_OUT_ACTIVE_LEVEL				0 						//KR_OUT 有效电平为低电平
#define KR_OUT_PORT_SRC 				GPIO_PortSourceGPIOA	//KR_OUT 外部中断端口号
#define KR_OUT_PIN_SRC 					GPIO_PinSource15 		//KR_OUT 外部中断引脚号
#define KR_OUT_EXTI_LINE 				EXTI_Line15 			//KR_OUT 外部中断号
#define KR_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//KR_OUT 外部中断触发方式
#define KR_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//KR_OUT 外部中断 NVIC 号

/////////////////////////////////////////////////////////////////////////////////////////////////
//IO 操作函数
#define VIBRATE_OUT 					VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID) 		//读取 VIBRATE_OUT 低电平有效
#define PIR_OUT 						PIR_OUT_FUN_IN(PIR_OUT_PIN_ID) 				//读取 PIR_OUT 高电平有效
#define HY_OUT							HY_OUT_FUN_IN(HY_OUT_PIN_ID)				//读取HY_OUT低电平有效
//#define HE_OUT							HE_OUT_FUN_IN(HE_OUT_PIN_ID)				//读取HE_OUT低电平有效
#define KR_OUT							KR_OUT_FUN_IN(KR_OUT_PIN_ID)				//读取KR_OUT低电平有效

extern uint8_t VIBRATE_OUT_flag;
extern uint8_t PIR_OUT_flag;
extern uint8_t HY_OUT_flag;
extern uint8_t HE_OUT_flag;
extern uint8_t KR_OUT_flag;

/* 供外部调用的函数声明 */

void bsp_InitIoSensor(void);

#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
