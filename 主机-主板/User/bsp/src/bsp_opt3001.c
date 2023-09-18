#include "bsp.h"
/*
*********************************************************************************************************
* 函 数 名: Opt3001WriteRegister
* 功能说明: OPT3001 寄存器写函数
* 形 参: uint8_t registerName:寄存器地址
* uint16_t value: 需要往寄存器里面写的数据
* 返 回 值: 无
*********************************************************************************************************
*/
void Opt3001WriteRegister(uint8_t registerName, uint16_t value)
{
	IIC_Start(); //起始信号
	IIC_Send_Byte(OPT3001_ADDR); //发送设备地址+写信号
	IIC_Wait_Ack();
	IIC_Send_Byte(registerName); //发送存储单元地址
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value >> 8));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value & 0x00FF));
	IIC_Wait_Ack();
	IIC_Stop(); //停止信号
}
/*
*********************************************************************************************************
* 函 数 名: Opt3001WriteRegister
* 功能说明: 读出 OPT3001 内部数据,连续两个
* 形 参: uint8_t ST_Address:OPT3001 寄存器地址
* 返 回 值: uint16_t 数据
*********************************************************************************************************
*/
uint16_t OPT3001Multiple_read(uint8_t ST_Address)
{ 
	uint8_t msb=0, lsb=0; //高位和低位
	uint16_t _data; //数据
	
	IIC_Start(); //起始信号
	IIC_Send_Byte(OPT3001_ADDR); //发送设备地址+写信号
	IIC_Wait_Ack();
	IIC_Send_Byte(ST_Address); //发送存储单元地址
	IIC_Wait_Ack();
	IIC_Stop(); //停止信号
	IIC_Start(); //起始信号
	IIC_Send_Byte(OPT3001_ADDR|0x01); //发送设备地址+读信号
	IIC_Wait_Ack();
	msb = IIC_Read_Byte(0x01); //BUF[0]存储 发送 ACK
	lsb = IIC_Read_Byte(0x00); //发送 NACK
	IIC_Stop(); //停止信号
	delay_ms(5);
	_data = msb << 8;
	_data |= lsb;
	
	return _data;
}
/*
*********************************************************************************************************
* 函 数 名: GetOPT3001ManufacturerID
* 功能说明: 获取 OPT3001 的出厂编号
* 形 参: 无
* 返 回 值: uint16_t OPT3001 的出厂编号 默认值读出来应该是 0x5449
*********************************************************************************************************
*/
uint16_t GetOPT3001ManufacturerID(void)
{
	uint16_t IDNum=0;
	
	IDNum=OPT3001Multiple_read(OPT3001_MANUFACTURER_ID); //读取
	
	return IDNum;
}
/*
*********************************************************************************************************
* 函 数 名: GetOPT3001DeviceID
* 功能说明: 获取 OPT3001 的设备 ID
* 形 参: 无
* 返 回 值: uint16_t OPT3001 的设备 ID 初始值为 0x3001
*********************************************************************************************************
*/
uint16_t GetOPT3001DeviceID(void)
{
	 uint16_t IDNum=0;
	
	 IDNum=OPT3001Multiple_read(OPT3001_DEVICE_ID); //读取
	
	 return IDNum;
}
/*
*********************************************************************************************************
* 函 数 名: bsp_InitOpt3001
* 功能说明: OPT3001 初始化函数
* 形 参: 无
* 返 回 值: 0 为初始化成功,1 为初始化失败
*********************************************************************************************************
*/
uint8_t bsp_InitOpt3001(void)
{
	uint16_t ManufacturerIDNum = 0;
	uint16_t DeviceIDNum = 0;
	
	bsp_InitI2C(); //OPT3001 端口初始化
	
	OPT3001Config(); //配置 OPT3001 并且唤醒 OPT3001
	delay_ms(5);
	ManufacturerIDNum = GetOPT3001ManufacturerID();
	BSP_Printf("ManufacturerIDNum is %X\r\n",ManufacturerIDNum); //调试
	delay_us(100);
	DeviceIDNum = GetOPT3001DeviceID();
	BSP_Printf("DeviceIDNum is %X\r\n",DeviceIDNum); //调试
	
	if( (ManufacturerIDNum == OPT3001_ManufacturerID ) && ( DeviceIDNum == OPT3001_DeviceID ) ) //如果读出来的ID 都正确表示初始化成功
	{
		return 0;
	}
	else //否则初始化不成功
	{
		return 1;
	}
}
/*
*********************************************************************************************************
* 函 数 名: OPT3001Config
* 功能说明: OPT3001 寄存器配置
* 形 参: 无
* 返 回 值: 无
* 注 意:
* 12-15 位 RN[0,3]:0x1100,设置为 Full-Scale Mode
* 9-10：M[0,1]:0x10,设置 OPT3001 为连续转换模式
* 4bit: Latch=1,其他位初始化为 00 即可
*********************************************************************************************************
*/
void OPT3001Config(void)
{
	Opt3001WriteRegister(OPT3001_CONFIGURATION, 0xC410);
}
/*
*********************************************************************************************************
* 函 数 名: GetLuxValue
* 功能说明: 获取光强度
* 形 参: 无
* 返 回 值: 得到的是放大 100 后的值 占 4 个字节
*********************************************************************************************************
*/
uint32_t GetLuxValue(void)
{
	uint32_t Result=0;
	Result=OPT3001Multiple_read(OPT3001_RESULT); //读取
	Result=(1<<((Result& 0xF000)>>12))*(Result & 0x0FFF); //得到计算的值是真实值的 100 倍
	return Result;
}
