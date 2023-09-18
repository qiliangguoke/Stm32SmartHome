#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */
#define RCC_ALL_BEEP (BEEP_GPIO_CLK)

#define BEEP_GPIO_PIN         GPIO_Pin_8             //BEEP引脚号
#define BEEP_PIN_ID           8                      //BEEF引脚序号
#define BEEP_GPIO_PORT        GPIOA                   //BEEP端口号
#define BEEP_GPIO_CLK         RCC_APB2Periph_GPIOA    //BEEP时钟
#define BEEP_FUN_OUT          PAout                   //BEEP输出端口配置函数
//#define BEEP_FUN_IN           PGin                    //BEEP输入端口配置函数

////////////////////////////////////////////////////

//IO操作函数
#define BEEP                  BEEP_FUN_OUT(BEEP_PIN_ID)//BEEP


/*供外部调用的函数声明*/
void bsp_InitBeep(void);
void bsp_BeepOn(void);
void bsp_BeepOff(void);
void bsp_BeepToggle(void);
uint8_t bsp_IsBeepOn(void);

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
