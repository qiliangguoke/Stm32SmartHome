#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */

/**********************************************************/ 
#define RES_POT_GPIO_PIN                   GPIO_Pin_7              //RES_POT引脚号
#define RES_POT_PIN_ID                     7                       ///RES_POT引脚序号
#define RES_POT_GPIO_PORT                  GPIOF           
#define RES_POT_GPIO_CLK                   RCC_APB2Periph_GPIOF    
#define RES_POT_ADC_X                      ADC3
#define RES_POT_ADC_CHANNEL                ADC_Channel_5     
#define RES_POT_ADC_SAMPLETIME             ADC_SampleTime_55Cycles5  

#define SAMP_COUNT                         20
/**********************************************************/

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitAdc(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t ADC_SampleTime);
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times);

void bsp_InitAdcTemp(void);
short Get_Temprate(void);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
