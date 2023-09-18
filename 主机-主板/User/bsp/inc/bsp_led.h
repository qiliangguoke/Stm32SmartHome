#ifndef __BSP_LED_H
#define __BSP_LED_H

//导入官方库
#include "sys.h"

//移植修改区
//定义led灯的端口
#ifdef WSNEP_V01

#define RCC_ALL_LED				( LED0_GPIO_CLK | LED1_GPIO_CLK )

///LED0配置
#define LED0_GPIO_PIN			GPIO_Pin_5						//LED0引脚号
#define LED0_PIN_ID				5								//led引脚序号
#define LED0_GPIO_PORT			GPIOB							//led端口号 peb
#define LED0_GPIO_CLK			RCC_APB2Periph_GPIOB			//led0时钟
#define LED0_FUN_OUT			PBout							//led 配置函数


////////

///LED1配置
#define LED1_GPIO_PIN			GPIO_Pin_5						//LED1引脚号
#define LED1_PIN_ID				5								//led引脚序号
#define LED1_GPIO_PORT			GPIOE						//led端口号 peb
#define LED1_GPIO_CLK			RCC_APB2Periph_GPIOE			//led1时钟
#define LED1_FUN_OUT			PEout							//led 配置函数

////////

#endif

//io操作函数
#define LED0					LED0_FUN_OUT(LED0_PIN_ID)		//LED0
#define LED1					LED1_FUN_OUT(LED1_PIN_ID)		//led1






//函数区域
void bsp_LedOff(uint8_t _no);
void bsp_InitLed(void);
void bsp_LedToggle(uint8_t _no);
void bsp_LedOn(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);


#endif

