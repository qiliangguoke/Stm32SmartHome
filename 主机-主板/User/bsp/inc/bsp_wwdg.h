#ifndef _BSP_WWDG_H
#define _BSP_WWDG_H

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

/*初始化*/
void bsp_InitWwdg(uint8_t _ucTreg,uint8_t _ucWreg,uint32_t WWDG_Prescaler);
void bsp_InitWwdgNVIC(FunctionalState NewState);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
