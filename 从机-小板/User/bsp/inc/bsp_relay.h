#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H
#include "sys.h"
///////////////////////////////////////////////////////////
//��ֲ�޸���
//����ʵ�ʶ˿��޸�
//��ǰ�汾����������ֲ(ע�⣬�˿ڽ��������ڷ� JTAG/SWD ���ţ������ JTAG ���ţ���Ҫ�� AFIO ʱ�ӣ���ʧ�� JTAG)
#ifdef WSNEP_V01

#define RCC_ALL_RELAY 		( RELAY1_GPIO_CLK | RELAY2_GPIO_CLK | RELAY3_GPIO_CLK | RELAY4_GPIO_CLK)
#define RELAY1_GPIO_PIN 	GPIO_Pin_7 //RELAY1 ���ź�
#define RELAY1_PIN_ID 		7 //RELAY1 �������
#define RELAY1_GPIO_PORT 	GPIOB //RELAY1 �˿ں�
#define RELAY1_GPIO_CLK 	RCC_APB2Periph_GPIOB //RELAY1 ʱ��
#define RELAY1_FUN_OUT 		PBout //RELAY1 ����˿����ú���

//#define RELAY1_FUN_IN PCin //RELAY1 ����˿����ú���
////

#define RELAY2_GPIO_PIN 	GPIO_Pin_6 //RELAY2 ���ź�
#define RELAY2_PIN_ID 		6 //RELAY2 �������
#define RELAY2_GPIO_PORT 	GPIOB //RELAY2 �˿ں�
#define RELAY2_GPIO_CLK 	RCC_APB2Periph_GPIOB //RELAY2 ʱ��
#define RELAY2_FUN_OUT 		PBout //RELAY2 ����˿����ú���


//#define RELAY2_FUN_IN PCin //RELAY2 ����˿����ú���

///

#define RELAY3_GPIO_PIN 	GPIO_Pin_5 //RELAY3 ���ź�
#define RELAY3_PIN_ID		5 //RELAY3 �������
#define RELAY3_GPIO_PORT 	GPIOB //RELAY3 �˿ں�
#define RELAY3_GPIO_CLK 	RCC_APB2Periph_GPIOB //RELAY3 ʱ��
#define RELAY3_FUN_OUT 		PBout //RELAY3 ����˿����ú���

//#define RELAY3_FUN_IN PCin //RELAY3 ����˿����ú���
////

#define RELAY4_GPIO_PIN 	GPIO_Pin_4 //RELAY4 ���ź�
#define RELAY4_PIN_ID 		4 //RELAY4 �������
#define RELAY4_GPIO_PORT	GPIOB //RELAY4 �˿ں�
#define RELAY4_GPIO_CLK 	RCC_APB2Periph_GPIOB //RELAY4 ʱ��
#define RELAY4_FUN_OUT 		PBout //RELAY4 ����˿����ú���

//#define RELAY4_FUN_IN PCin //RELAY4 ����˿����ú���

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////


//IO ��������
#define RELAY1 				RELAY1_FUN_OUT(RELAY1_PIN_ID) //REALY1
#define RELAY2 				RELAY2_FUN_OUT(RELAY2_PIN_ID) //REALY2   
#define RELAY3 				RELAY3_FUN_OUT(RELAY3_PIN_ID) //REALY3
#define RELAY4 				RELAY4_FUN_OUT(RELAY4_PIN_ID) //REALY4

/* ���ⲿ���õĺ������� */
void bsp_InitRelay(void);
void bsp_RelayOn(uint8_t _no);
void bsp_RelayOff(uint8_t _no);
void bsp_RelayToggle(uint8_t _no);
uint8_t bsp_IsRelayOn(uint8_t _no);

#endif
