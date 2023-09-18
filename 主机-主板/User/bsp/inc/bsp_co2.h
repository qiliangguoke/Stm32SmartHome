#ifndef __BSP_CO2_H
#define __BSP_CO2_H
#include "sys.h" 
#define CO2_TEMP_BUF_LEN 10 //定义 CO2_TEMP_BUF_LEN 缓冲长度为 10 个字节
//CO2 读取数据
uint16_t CO2_READ(void);
#endif
