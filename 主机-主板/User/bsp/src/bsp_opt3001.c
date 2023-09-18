#include "bsp.h"
/*
*********************************************************************************************************
* �� �� ��: Opt3001WriteRegister
* ����˵��: OPT3001 �Ĵ���д����
* �� ��: uint8_t registerName:�Ĵ�����ַ
* uint16_t value: ��Ҫ���Ĵ�������д������
* �� �� ֵ: ��
*********************************************************************************************************
*/
void Opt3001WriteRegister(uint8_t registerName, uint16_t value)
{
	IIC_Start(); //��ʼ�ź�
	IIC_Send_Byte(OPT3001_ADDR); //�����豸��ַ+д�ź�
	IIC_Wait_Ack();
	IIC_Send_Byte(registerName); //���ʹ洢��Ԫ��ַ
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value >> 8));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value & 0x00FF));
	IIC_Wait_Ack();
	IIC_Stop(); //ֹͣ�ź�
}
/*
*********************************************************************************************************
* �� �� ��: Opt3001WriteRegister
* ����˵��: ���� OPT3001 �ڲ�����,��������
* �� ��: uint8_t ST_Address:OPT3001 �Ĵ�����ַ
* �� �� ֵ: uint16_t ����
*********************************************************************************************************
*/
uint16_t OPT3001Multiple_read(uint8_t ST_Address)
{ 
	uint8_t msb=0, lsb=0; //��λ�͵�λ
	uint16_t _data; //����
	
	IIC_Start(); //��ʼ�ź�
	IIC_Send_Byte(OPT3001_ADDR); //�����豸��ַ+д�ź�
	IIC_Wait_Ack();
	IIC_Send_Byte(ST_Address); //���ʹ洢��Ԫ��ַ
	IIC_Wait_Ack();
	IIC_Stop(); //ֹͣ�ź�
	IIC_Start(); //��ʼ�ź�
	IIC_Send_Byte(OPT3001_ADDR|0x01); //�����豸��ַ+���ź�
	IIC_Wait_Ack();
	msb = IIC_Read_Byte(0x01); //BUF[0]�洢 ���� ACK
	lsb = IIC_Read_Byte(0x00); //���� NACK
	IIC_Stop(); //ֹͣ�ź�
	delay_ms(5);
	_data = msb << 8;
	_data |= lsb;
	
	return _data;
}
/*
*********************************************************************************************************
* �� �� ��: GetOPT3001ManufacturerID
* ����˵��: ��ȡ OPT3001 �ĳ������
* �� ��: ��
* �� �� ֵ: uint16_t OPT3001 �ĳ������ Ĭ��ֵ������Ӧ���� 0x5449
*********************************************************************************************************
*/
uint16_t GetOPT3001ManufacturerID(void)
{
	uint16_t IDNum=0;
	
	IDNum=OPT3001Multiple_read(OPT3001_MANUFACTURER_ID); //��ȡ
	
	return IDNum;
}
/*
*********************************************************************************************************
* �� �� ��: GetOPT3001DeviceID
* ����˵��: ��ȡ OPT3001 ���豸 ID
* �� ��: ��
* �� �� ֵ: uint16_t OPT3001 ���豸 ID ��ʼֵΪ 0x3001
*********************************************************************************************************
*/
uint16_t GetOPT3001DeviceID(void)
{
	 uint16_t IDNum=0;
	
	 IDNum=OPT3001Multiple_read(OPT3001_DEVICE_ID); //��ȡ
	
	 return IDNum;
}
/*
*********************************************************************************************************
* �� �� ��: bsp_InitOpt3001
* ����˵��: OPT3001 ��ʼ������
* �� ��: ��
* �� �� ֵ: 0 Ϊ��ʼ���ɹ�,1 Ϊ��ʼ��ʧ��
*********************************************************************************************************
*/
uint8_t bsp_InitOpt3001(void)
{
	uint16_t ManufacturerIDNum = 0;
	uint16_t DeviceIDNum = 0;
	
	bsp_InitI2C(); //OPT3001 �˿ڳ�ʼ��
	
	OPT3001Config(); //���� OPT3001 ���һ��� OPT3001
	delay_ms(5);
	ManufacturerIDNum = GetOPT3001ManufacturerID();
	BSP_Printf("ManufacturerIDNum is %X\r\n",ManufacturerIDNum); //����
	delay_us(100);
	DeviceIDNum = GetOPT3001DeviceID();
	BSP_Printf("DeviceIDNum is %X\r\n",DeviceIDNum); //����
	
	if( (ManufacturerIDNum == OPT3001_ManufacturerID ) && ( DeviceIDNum == OPT3001_DeviceID ) ) //�����������ID ����ȷ��ʾ��ʼ���ɹ�
	{
		return 0;
	}
	else //�����ʼ�����ɹ�
	{
		return 1;
	}
}
/*
*********************************************************************************************************
* �� �� ��: OPT3001Config
* ����˵��: OPT3001 �Ĵ�������
* �� ��: ��
* �� �� ֵ: ��
* ע ��:
* 12-15 λ RN[0,3]:0x1100,����Ϊ Full-Scale Mode
* 9-10��M[0,1]:0x10,���� OPT3001 Ϊ����ת��ģʽ
* 4bit: Latch=1,����λ��ʼ��Ϊ 00 ����
*********************************************************************************************************
*/
void OPT3001Config(void)
{
	Opt3001WriteRegister(OPT3001_CONFIGURATION, 0xC410);
}
/*
*********************************************************************************************************
* �� �� ��: GetLuxValue
* ����˵��: ��ȡ��ǿ��
* �� ��: ��
* �� �� ֵ: �õ����ǷŴ� 100 ���ֵ ռ 4 ���ֽ�
*********************************************************************************************************
*/
uint32_t GetLuxValue(void)
{
	uint32_t Result=0;
	Result=OPT3001Multiple_read(OPT3001_RESULT); //��ȡ
	Result=(1<<((Result& 0xF000)>>12))*(Result & 0x0FFF); //�õ������ֵ����ʵֵ�� 100 ��
	return Result;
}
