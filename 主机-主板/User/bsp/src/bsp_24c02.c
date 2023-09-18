#include "bsp.h"

/*
*********************************************************************************************************
* 函 数 名：AT24CXX_Init
* 功能说明：AT24CXX 初始化，主要是 I2C 总线初始化
* 形 参：无
* 返 回 值：无
*********************************************************************************************************
*/
void AT24CXX_Init(void)
{
	bsp_InitI2C();
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_ReadOneByte
* 功能说明：在 AT24CXX 指定地址读出一个数据
* 形 参：uint6_t ReadAddr:开始读数的地址
* 返 回 值：读到的数据
*********************************************************************************************************
*/
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{ 
	uint8_t temp=0; 
	IIC_Start(); 
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0); //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
		IIC_Wait_Ack();
	}
	else 
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1)); //发送器件地址 0XA0,写数据
	
	IIC_Wait_Ack(); 
	IIC_Send_Byte(ReadAddr%256); //发送低地址
	IIC_Wait_Ack(); 
	IIC_Start(); 
	IIC_Send_Byte(0XA1); //进入接收模式 
	IIC_Wait_Ack();
	temp=IIC_Read_Byte(0); 
	IIC_Stop();//产生一个停止条件 
	
	return temp;
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_WriteOneByte
* 功能说明：在 AT24CXX 指定地址写入一个数据
* 形 参：uint16_t WriteAddr :写入数据的目的地址
* uint8_t DataToWrite:要写入的数据
* 返 回 值：无
*********************************************************************************************************
*/
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{ 
	IIC_Start(); 
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0); //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1)); //发送器件地址 0XA0,写数据
	}
	IIC_Wait_Ack(); 
	IIC_Send_Byte(WriteAddr%256); //发送低地址
	IIC_Wait_Ack(); 
	IIC_Send_Byte(DataToWrite); //发送字节 
	IIC_Wait_Ack(); 
	IIC_Stop();//产生一个停止条件
	delay_ms(10);
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_WriteLenByte
* 功能说明：在 AT24CXX 里面的指定地址开始写入长度为 Len 的数据,该函数用于写入 16bit 或者 32bit 的数据.
* 形 参：uint16_t WriteAddr :开始写入的地址
* uint32_t DataToWrite:数据数组首地址
* uint8_t Len :要写入数据的长度 2,4
* 返 回 值：无
*********************************************************************************************************
*/
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{ 
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	} 
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_ReadLenByte
* 功能说明：在 AT24CXX 里面的指定地址开始读出长度为 Len 的数据,该函数用于读出 16bit 或者 32bit 的数据.
* 形 参：uint16_t ReadAddr :开始读出的地址
* uint8_t Len :要读出数据的长度 2,4
* 返 回 值：数据
*********************************************************************************************************
*/
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{ 
	uint8_t t;
	uint32_t temp=0;
	
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 
	}
	
	return temp; 
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_Check
* 功能说明：检查 AT24CXX 是否正常,这里用了 24XX 的最后一个地址(255)来存储标志字.如果用其他 24C 系列,这个
地址要修改
* 形 参：无
* 返 回 值：返回 1:检测失败
* 返回 0:检测成功
*********************************************************************************************************
*/
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写 AT24CXX 
	if(temp==0X55)return 0; 
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
		temp=AT24CXX_ReadOneByte(255); 
		if(temp==0X55)return 0;
	}
	
	return 1; 
}
/*
*********************************************************************************************************
* 函 数 名：AT24CXX_Read
* 功能说明：在 AT24CXX 里面的指定地址开始读出指定个数的数据
* 形 参：uint16_t ReadAddr :开始读出的地址 对 24c02 为 0~255
* uint8_t pBuffer :数据数组首地址
 uint16_t NumToRead:要读出数据的个数
* 返 回 值：无
*********************************************************************************************************
*/
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
} 

/*
*********************************************************************************************************
* 函 数 名：AT24CXX_Write
* 功能说明：在 AT24CXX 里面的指定地址开始写入指定个数的数据
* 形 参：uint16_t WriteAddr :开始写入的地址 对 24c02 为 0~255
* uint8_t pBuffer :数据数组首地址
* uint16_t NumToWrite:要写入数据的个数
* 返 回 值：无
*********************************************************************************************************
*/

void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
