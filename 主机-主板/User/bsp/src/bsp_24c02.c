#include "bsp.h"

/*
*********************************************************************************************************
* �� �� ����AT24CXX_Init
* ����˵����AT24CXX ��ʼ������Ҫ�� I2C ���߳�ʼ��
* �� �Σ���
* �� �� ֵ����
*********************************************************************************************************
*/
void AT24CXX_Init(void)
{
	bsp_InitI2C();
}
/*
*********************************************************************************************************
* �� �� ����AT24CXX_ReadOneByte
* ����˵������ AT24CXX ָ����ַ����һ������
* �� �Σ�uint6_t ReadAddr:��ʼ�����ĵ�ַ
* �� �� ֵ������������
*********************************************************************************************************
*/
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{ 
	uint8_t temp=0; 
	IIC_Start(); 
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0); //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		IIC_Wait_Ack();
	}
	else 
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1)); //����������ַ 0XA0,д����
	
	IIC_Wait_Ack(); 
	IIC_Send_Byte(ReadAddr%256); //���͵͵�ַ
	IIC_Wait_Ack(); 
	IIC_Start(); 
	IIC_Send_Byte(0XA1); //�������ģʽ 
	IIC_Wait_Ack();
	temp=IIC_Read_Byte(0); 
	IIC_Stop();//����һ��ֹͣ���� 
	
	return temp;
}
/*
*********************************************************************************************************
* �� �� ����AT24CXX_WriteOneByte
* ����˵������ AT24CXX ָ����ַд��һ������
* �� �Σ�uint16_t WriteAddr :д�����ݵ�Ŀ�ĵ�ַ
* uint8_t DataToWrite:Ҫд�������
* �� �� ֵ����
*********************************************************************************************************
*/
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{ 
	IIC_Start(); 
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0); //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1)); //����������ַ 0XA0,д����
	}
	IIC_Wait_Ack(); 
	IIC_Send_Byte(WriteAddr%256); //���͵͵�ַ
	IIC_Wait_Ack(); 
	IIC_Send_Byte(DataToWrite); //�����ֽ� 
	IIC_Wait_Ack(); 
	IIC_Stop();//����һ��ֹͣ����
	delay_ms(10);
}
/*
*********************************************************************************************************
* �� �� ����AT24CXX_WriteLenByte
* ����˵������ AT24CXX �����ָ����ַ��ʼд�볤��Ϊ Len ������,�ú�������д�� 16bit ���� 32bit ������.
* �� �Σ�uint16_t WriteAddr :��ʼд��ĵ�ַ
* uint32_t DataToWrite:���������׵�ַ
* uint8_t Len :Ҫд�����ݵĳ��� 2,4
* �� �� ֵ����
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
* �� �� ����AT24CXX_ReadLenByte
* ����˵������ AT24CXX �����ָ����ַ��ʼ��������Ϊ Len ������,�ú������ڶ��� 16bit ���� 32bit ������.
* �� �Σ�uint16_t ReadAddr :��ʼ�����ĵ�ַ
* uint8_t Len :Ҫ�������ݵĳ��� 2,4
* �� �� ֵ������
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
* �� �� ����AT24CXX_Check
* ����˵������� AT24CXX �Ƿ�����,�������� 24XX �����һ����ַ(255)���洢��־��.��������� 24C ϵ��,���
��ַҪ�޸�
* �� �Σ���
* �� �� ֵ������ 1:���ʧ��
* ���� 0:���ɹ�
*********************************************************************************************************
*/
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����д AT24CXX 
	if(temp==0X55)return 0; 
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
		temp=AT24CXX_ReadOneByte(255); 
		if(temp==0X55)return 0;
	}
	
	return 1; 
}
/*
*********************************************************************************************************
* �� �� ����AT24CXX_Read
* ����˵������ AT24CXX �����ָ����ַ��ʼ����ָ������������
* �� �Σ�uint16_t ReadAddr :��ʼ�����ĵ�ַ �� 24c02 Ϊ 0~255
* uint8_t pBuffer :���������׵�ַ
 uint16_t NumToRead:Ҫ�������ݵĸ���
* �� �� ֵ����
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
* �� �� ����AT24CXX_Write
* ����˵������ AT24CXX �����ָ����ַ��ʼд��ָ������������
* �� �Σ�uint16_t WriteAddr :��ʼд��ĵ�ַ �� 24c02 Ϊ 0~255
* uint8_t pBuffer :���������׵�ַ
* uint16_t NumToWrite:Ҫд�����ݵĸ���
* �� �� ֵ����
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
