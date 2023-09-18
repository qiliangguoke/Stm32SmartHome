#ifndef __BSP_FAN_H
#define __BSP_FAN_H

#include "sys.h"

///////////////////////////////////////////////////////////
//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非 JTAG/SWD 引脚，如果是 JTAG 引脚，需要打开 AFIO 时钟，并失能 JTAG)


#ifdef WSNEP_V01
#define RCC_ALL_FAN ( FAN1_GPIO_CLK | FAN2_GPIO_CLK )
#define FAN1_GPIO_PIN GPIO_Pin_8 //FAN1 引脚号
#define FAN1_PIN_ID 8 //FAN1 引脚序号
#define FAN1_GPIO_PORT GPIOB //FAN1 端口号
#define FAN1_GPIO_CLK RCC_APB2Periph_GPIOB //FAN1 时钟
#define FAN1_FUN_OUT PBout //FAN1 输出端口配置函数
//#define FAN1_FUN_IN PBin //FAN1 输入端口配置函数

////
#define FAN2_GPIO_PIN GPIO_Pin_9 //FAN2 引脚号
#define FAN2_PIN_ID 9 //FAN2 引脚序号
#define FAN2_GPIO_PORT GPIOB //FAN2 端口号
#define FAN2_GPIO_CLK RCC_APB2Periph_GPIOB //FAN2 时钟
#define FAN2_FUN_OUT PBout //FAN2 输出端口配置函数
//#define FAN2_FUN_IN PBin //FAN2 输入端口配置函数
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
//IO 操作函数
#define FAN1 FAN1_FUN_OUT(FAN1_PIN_ID) //FAN1
#define FAN2 FAN2_FUN_OUT(FAN2_PIN_ID) //FAN2


/* 供外部调用的函数声明 */
void bsp_InitFan(void);
void bsp_FanOn(uint8_t _no);
void bsp_FanOff(uint8_t _no);
void bsp_FanToggle(uint8_t _no);
uint8_t bsp_IsFanOn(uint8_t _no);

#endif
