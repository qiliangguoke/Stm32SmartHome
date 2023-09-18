#include "sensor_Data_Proc.h"
//�趨��������Ϣ�ϱ��Ļ���
uint8_t SENSOR_DATA_BYTE[248] = 
{FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,//֡ͷ��3 ���ֽڣ�
0x13, //���������ȣ�1 ���ֽڣ�
0x03, //����Ϊ���ݣ�1 ���ֽڣ�
0x01, //Ϊ���������ݣ�1 ���ֽڣ�
0x10, //���������ݳ��ȣ�1 ���ֽڣ�
//---- //��������������8*N ���ֽڣ���N ���Ϊ 30��
//- //У�飨1 ���ֽڣ�
};
_sensorData Sensor_Data_ToFloat;//����������ת��Ϊ������
_sensorData Sensor_Data_To4Byte;//����������ת��Ϊ 4 �ֽ� �ڴ�
/*
*********************************************************************************************************
* �� �� ����sensor_TempHumi_Upload
* ����˵������ʪ�ȴ������ֽ�Э���ϱ�����
* �� �Σ� float temperature:�¶ȸ���������
* float humidity:ʪ�ȸ���������
* �� �� ֵ����
*********************************************************************************************************
*/ void sensor_TempHumi_Upload(float temperature,float humidity)
{
	uint8_t i = 0;//ѭ������
	//����һ���¶ȴ�����
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
	//����һ��ʪ�ȴ�����
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
	//��ֵ��ע��Թ�����ĵĸ�������ֵ
	Sensor_Data_To4Byte.f = temperature;
	//ע�⣬�ڴ���������С��ģʽ�洢�ģ������Ҫת��Ϊ���ģʽ
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	Sensor_Data_To4Byte.f = humidity;
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	//ƫ�Ƶ�ַ 0x03
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 19;//( 1 + 1 + 1 + 2 * 8 )���� 19 �ֽ�
	//ƫ�Ƶ�ַ 0x04
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA; //����֡
	//ƫ�Ƶ�ַ 0x05
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR; //����������
	//ƫ�Ƶ�ַ 0x06
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 16; //���� 16 ���ֽ�
	//ƫ�Ƶ�ַ 0x07
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 0 * 
	SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Temp_buf,8);//�����¶�����
	//ƫ�Ƶ�ַ 0x0F
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 1 * 
	SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Humi_buf,8);//�����¶�����
	//����У���
	SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN ) ) ] = 
	CheckSum( &SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET] );//����У����;
	USART1_Send_Data(SENSOR_DATA_BYTE,24);//��������
}
