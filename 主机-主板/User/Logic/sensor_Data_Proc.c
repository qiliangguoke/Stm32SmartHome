#include "sensor_Data_Proc.h"
//设定传感器信息上报的缓存
uint8_t SENSOR_DATA_BYTE[248] = 
{FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,//帧头（3 个字节）
0x13, //数据区长度（1 个字节）
0x03, //类型为数据（1 个字节）
0x01, //为传感器数据（1 个字节）
0x10, //传感器数据长度（1 个字节）
//---- //传感器数据区（8*N 个字节）（N 最大为 30）
//- //校验（1 个字节）
};
_sensorData Sensor_Data_ToFloat;//传感器数据转换为浮点数
_sensorData Sensor_Data_To4Byte;//传感器数据转换为 4 字节 内存
/*
*********************************************************************************************************
* 函 数 名：sensor_TempHumi_Upload
* 功能说明：温湿度传感器字节协议上报函数
* 形 参： float temperature:温度浮点数数据
* float humidity:湿度浮点数数据
* 返 回 值：无
*********************************************************************************************************
*/ void sensor_TempHumi_Upload(float temperature,float humidity)
{
	uint8_t i = 0;//循环数据
	//定义一下温度传感器
	uint8_t sensor_Temp_buf[8] = {
	ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
	ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
	SENSOR_TEMP_CODE,
	SENSOR_STATUS_RUN,
	0,
	0,
	0,
	0
	};
	//定义一下湿度传感器
	uint8_t sensor_Humi_buf[8] = {
	ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
	ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
	SENSOR_HUMI_CODE,
	SENSOR_STATUS_RUN,
	0,
	0,
	0,
	0
	};
	//赋值，注意对共用体的的浮点数赋值
	Sensor_Data_To4Byte.f = temperature;
	//注意，内存中数组是小端模式存储的，因此需要转换为大端模式
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	Sensor_Data_To4Byte.f = humidity;
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	//偏移地址 0x03
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 19;//( 1 + 1 + 1 + 2 * 8 )长度 19 字节
	//偏移地址 0x04
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA; //数据帧
	//偏移地址 0x05
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR; //传感器数据
	//偏移地址 0x06
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 16; //长度 16 个字节
	//偏移地址 0x07
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 0 * 
	SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Temp_buf,8);//拷贝温度数据
	//偏移地址 0x0F
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 1 * 
	SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Humi_buf,8);//拷贝温度数据
	//计算校验和
	SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN ) ) ] = 
	CheckSum( &SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET] );//计算校验码;
	USART1_Send_Data(SENSOR_DATA_BYTE,24);//发送数据
}
