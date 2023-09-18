#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H
#include "sys.h"
///////////////////////////////////////////////////////////
//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非 JTAG/SWD 引脚，如果是 JTAG 引脚，需要打开 AFIO 时钟，并失能 JTAG)
#ifdef WSNEP_V01

#define RCC_ALL_RELAY 		( RELAY1_GPIO_CLK | RELAY2_GPIO_CLK | RELAY3_GPIO_CLK | RELAY4_GPIO_CLK)
#define RELAY1_GPIO_PIN 	GPIO_Pin_0 //RELAY1 引脚号
#define RELAY1_PIN_ID 		0 //RELAY1 引脚序号
#define RELAY1_GPIO_PORT 	GPIOC //RELAY1 端口号
#define RELAY1_GPIO_CLK 	RCC_APB2Periph_GPIOC //RELAY1 时钟
#define RELAY1_FUN_OUT 		PCout //RELAY1 输出端口配置函数

//#define RELAY1_FUN_IN PCin //RELAY1 输入端口配置函数
////

#define RELAY2_GPIO_PIN 	GPIO_Pin_1 //RELAY2 引脚号
#define RELAY2_PIN_ID 		1 //RELAY2 引脚序号
#define RELAY2_GPIO_PORT 	GPIOC //RELAY2 端口号
#define RELAY2_GPIO_CLK 	RCC_APB2Periph_GPIOC //RELAY2 时钟
#define RELAY2_FUN_OUT 		PCout //RELAY2 输出端口配置函数


//#define RELAY2_FUN_IN PCin //RELAY2 输入端口配置函数

///

#define RELAY3_GPIO_PIN 	GPIO_Pin_2 //RELAY3 引脚号
#define RELAY3_PIN_ID		2 //RELAY3 引脚序号
#define RELAY3_GPIO_PORT 	GPIOC //RELAY3 端口号
#define RELAY3_GPIO_CLK 	RCC_APB2Periph_GPIOC //RELAY3 时钟
#define RELAY3_FUN_OUT 		PCout //RELAY3 输出端口配置函数

//#define RELAY3_FUN_IN PCin //RELAY3 输入端口配置函数
////

#define RELAY4_GPIO_PIN 	GPIO_Pin_3 //RELAY4 引脚号
#define RELAY4_PIN_ID 		3 //RELAY4 引脚序号
#define RELAY4_GPIO_PORT	GPIOC //RELAY4 端口号
#define RELAY4_GPIO_CLK 	RCC_APB2Periph_GPIOC //RELAY4 时钟
#define RELAY4_FUN_OUT 		PCout //RELAY4 输出端口配置函数

//#define RELAY4_FUN_IN PCin //RELAY4 输入端口配置函数

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////


//IO 操作函数
#define RELAY1 				RELAY1_FUN_OUT(RELAY1_PIN_ID) //REALY1
#define RELAY2 				RELAY2_FUN_OUT(RELAY2_PIN_ID) //REALY2   
#define RELAY3 				RELAY3_FUN_OUT(RELAY3_PIN_ID) //REALY3
#define RELAY4 				RELAY4_FUN_OUT(RELAY4_PIN_ID) //REALY4

/* 供外部调用的函数声明 */
void bsp_InitRelay(void);
void bsp_RelayOn(uint8_t _no);
void bsp_RelayOff(uint8_t _no);
void bsp_RelayToggle(uint8_t _no);
uint8_t bsp_IsRelayOn(uint8_t _no);

#endif
