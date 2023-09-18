#ifndef __SENSOR_DATA_PROC_H_
#define __SENSOR_DATA_PROC_H_
//////////////////////////////////////
//ͷ�ļ���
#include "bsp.h"
#include "usart1_cmd.h"
//////////////////////////////////
//������
//����֡
#define FUN_CODE_DATA 0x03

//���������������ݳ���
#define SENSOR_SINGLE_DATA_LEN 8
#define DATA_LEN_OFFSET 3 //���ݳ���ƫ�Ƶ�ַ
#define FRAME_TYPE_OFFSET 4 //֡����ƫ�Ƶ�ַ
#define DATA_TYPE_OFFSET 5 //����֡����������ƫ�Ƶ�ַ
#define SENSOR_DATA_LEN_OFFSET 6 //���������ݳ��� ƫ�Ƶ�ַ
#define SENSOR_DATA_START_OFFSET 7 //��һ�������������׵�ַ ƫ�Ƶ�ַ
//////////////////////////////////////////////////////////////////////////////////

#define SENSOR_TEMP_CODE 0x01 //�¶ȴ�����
#define SENSOR_HUMI_CODE 0x02 //ʪ�ȴ�����
#define SENSOR_LM_CODE 0x04 //���մ�����
#define SENSOR_CO2_CODE 0x05 //CO2 ������
#define SENSOR_PM25_CODE 0x06 //PM2.5 ������
#define ENVIRONMENT_SENSOR_TYPE_CODE_MSB 0x22 //����������
#define ENVIRONMENT_SENSOR_TYPE_CODE_LSB 0x00 //
#define SENSOR_STATUS_PRE 0x00 //������Ԥ��
#define SENSOR_STATUS_NG 0x01 //��������ֵ�쳣
#define SENSOR_STATUS_RUN 0x02 //��������������
#define DATA_TYPE_SENSOR 0x01 //����������
////////////////////////////////////////////////////////////////////////////////////
////////////
//ע�� x[3]Ϊ���,x[0]ΪС��
typedef union 
{
	float f;
	uint8_t x[4];
}_sensorData;//���������� float �� 4 �ֽ� �໥�任 ʹ��������
//////////////////////////////////
//����������
//�ֽ���ʽ������ʪ����Ϣ
void sensor_TempHumi_Upload(float temperature,float humidity);

#endif
