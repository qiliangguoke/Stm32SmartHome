#include "bsp.h"
/*
*********************************************************************************************************
* 函 数 名: bsp_InitI2C
* 功能说明: 配置 IIC 相关的 GPIO。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( SCL_GPIO_CLK, ENABLE ); //使能 SCL 时钟
	RCC_APB2PeriphClockCmd( SDA_GPIO_CLK, ENABLE ); //使能 SDA 时钟
	
	
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SCL_GPIO_PORT,SCL_GPIO_PIN); //SCL 输出高
	
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SDA_GPIO_PORT,SDA_GPIO_PIN); //SDA 输出高
}
/*
*********************************************************************************************************
* 函 数 名: IIC_Start
* 功能说明: 产生 IIC 起始信号。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void IIC_Start(void)
{
	SDA_OUT(); //sda 线输出
	IIC_SDA=1; 
	IIC_SCL=1;
	delay_us(4);
	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住 I2C 总线，准备发送或接收数据
} 
/*
*********************************************************************************************************
* 函 数 名: IIC_Stop
* 功能说明: 产生 IIC 结束信号。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void IIC_Stop(void)
{
	SDA_OUT();//sda 线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送 I2C 总线结束信号
	delay_us(4); 
}
/*
*********************************************************************************************************
* 函 数 名: IIC_Wait_Ack
* 功能说明: 等待应答信号到来。
* 形 参: 无
* 返 回 值: 1，接收应答失败
* 0，接收应答成功
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN(); //SDA 设置为输入 
	IIC_SDA=1;delay_us(1); 
	IIC_SCL=1;delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出 0 
	return 0; 
} 
/*
*********************************************************************************************************
* 函 数 名: IIC_Ack
* 功能说明: 产生 ACK 应答。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
/*
*********************************************************************************************************
* 函 数 名: IIC_NAck
* 功能说明: 不产生 ACK 应答。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
/*
*********************************************************************************************************
* 函 数 名: IIC_Send_Byte
* 功能说明: IIC 发送一个字节。
* 形 参: 返回从机有无应答
* 1，有应答
* 0，无应答
* 返 回 值: 无
*********************************************************************************************************
*/
void IIC_Send_Byte(uint8_t txd)
{ 
	 uint8_t t; 
	 SDA_OUT(); 
	 IIC_SCL=0;//拉低时钟开始数据传输
	 for(t=0;t<8;t++)
	 { 
		 //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 
		delay_us(2); //对 TEA5767 这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;
		delay_us(2);
	 }
} 
/*
*********************************************************************************************************
* 函 数 名: IIC_Read_Byte
* 功能说明: 读 1 个字节，ack=1 时，发送 ACK，ack=0，发送 nACK。
* 形 参: 返回从机有无应答
* 1，发送 ACK
* 0，发送 nACK
* 返 回 值: 返回读取到的 1 字节数据
*********************************************************************************************************
*/
u8 IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	SDA_IN();//SDA 设置为输入
	for(i=0;i<8;i++ )
	{
		 IIC_SCL=0; 
		 delay_us(2);
		 IIC_SCL=1;
		 receive<<=1;
		 if(READ_SDA)receive++; 
		 delay_us(1); 
	 }
	 if (!ack)
		IIC_NAck();//发送 nACK
	 else
		IIC_Ack(); //发送 ACK 
	 
	 return receive;
}
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
