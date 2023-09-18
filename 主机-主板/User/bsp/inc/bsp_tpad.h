#ifndef __BSP_TPAD_H
#define __BSP_TPAD_H

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */
#define   TIMx_CAP_TIM                     			TIM5
#define   TIMx_CAP_TIM_APBxClock_FUN                RCC_APB1PeriphClockCmd
#define   TIMx_CAP_TIM_CLK				            RCC_APB1Periph_TIM5
#define   TPAD_ARR_MAX_VAL                          0XFFFF
#define   TIMx_CAP_TIM_PSC							(6-1)       //以12M的频率计数

//TIM输入捕获通道GPIO相关宏定义
#define   TIMx_CAP_TIM_CH_GPIO_CLK   				RCC_APB2Periph_GPIOA
#define   TIMx_CAP_TIM_CH_PORT						GPIOA
#define   TIMx_CAP_TIM_CH_PIN						GPIO_Pin_1
#define   TIMx_CAP_TIM_CHANNEL_x					TIM_Channel_2

//中断相关宏定义
#define   TIMx_CAP_TIM_IT_CCx						TIM_IT_CC2
#define   TIMx_CAP_TIM_IT_UPDATE					TIM_IT_Update
#define   TIMx_CAP_TIM_IT_IRQ						TIM5_IRQn
#define   TIMx_CAP_TIM_IT_INT_FUN					TIM5_IRQHandler

//获取捕获寄存器值函数宏定义
#define   TIMx_CAP_TIM_GetCapturex_FUN				TIM_GetCapture2

//捕捉信号极性函数宏定义
#define   TIMx_CAP_TIM_OCxPolarityConfig_FUN		TIM_OC2PolarityConfig

//触摸的门限值，也就是必须大于
#define   TPAD_GATE_VAL   100


/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
uint8_t bsp_TpadInit(void);
uint8_t bsp_TpadScan(void);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
