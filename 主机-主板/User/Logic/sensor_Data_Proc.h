#ifndef __SENSOR_DATA_PROC_H_
#define __SENSOR_DATA_PROC_H_
//////////////////////////////////////
//头文件区
#include "bsp.h"
#include "usart1_cmd.h"
//////////////////////////////////
//变量区
//数据帧
#define FUN_CODE_DATA 0x03

//单个传感器的数据长度
#define SENSOR_SINGLE_DATA_LEN 8
#define DATA_LEN_OFFSET 3 //数据长度偏移地址
#define FRAME_TYPE_OFFSET 4 //帧类型偏移地址
#define DATA_TYPE_OFFSET 5 //数据帧的数据类型偏移地址
#define SENSOR_DATA_LEN_OFFSET 6 //传感器数据长度 偏移地址
#define SENSOR_DATA_START_OFFSET 7 //第一个传感器数据首地址 偏移地址
//////////////////////////////////////////////////////////////////////////////////

#define SENSOR_TEMP_CODE 0x01 //温度传感器
#define SENSOR_HUMI_CODE 0x02 //湿度传感器
#define SENSOR_LM_CODE 0x04 //光照传感器
#define SENSOR_CO2_CODE 0x05 //CO2 传感器
#define SENSOR_PM25_CODE 0x06 //PM2.5 传感器
#define ENVIRONMENT_SENSOR_TYPE_CODE_MSB 0x22 //环境传感器
#define ENVIRONMENT_SENSOR_TYPE_CODE_LSB 0x00 //
#define SENSOR_STATUS_PRE 0x00 //传感器预热
#define SENSOR_STATUS_NG 0x01 //传感器数值异常
#define SENSOR_STATUS_RUN 0x02 //传感器正常运行
#define DATA_TYPE_SENSOR 0x01 //传感器数据
////////////////////////////////////////////////////////////////////////////////////
////////////
//注意 x[3]为大端,x[0]为小端
typedef union 
{
	float f;
	uint8_t x[4];
}_sensorData;//传感器数据 float 与 4 字节 相互变换 使用联合体
//////////////////////////////////
//函数申明区
//字节形式传输温湿度信息
void sensor_TempHumi_Upload(float temperature,float humidity);

#endif
