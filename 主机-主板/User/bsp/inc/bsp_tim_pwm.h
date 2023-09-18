#ifndef _BSP_TIM_PWM_H
#define _BSP_TIM_PWM_H

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_SetTIMforInt(TIM_TypeDef* TIMx,uint32_t _ulFreq,uint8_t _PreemptionPriority,uint8_t _SubPriority);
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel,
	uint32_t _ulFreq,uint32_t _ulDutyCycle);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
