#ifndef __BSP_IOSENSOR_H
#define __BSP_IOSENSOR_H

#include "sys.h"

///////////////////////////////////////////////////////////

//��ֲ�޸���
//����ʵ�ʶ˿��޸�
//��ǰ�汾����������ֲ(ע�⣬�˿ڽ��������ڷ� JTAG/SWD ���ţ������ JTAG ���ţ���Ҫ�� AFIO ʱ�ӣ���ʧ�� JTAG)



///////////////////////
//DO101 �𶯴�����
#define VIBRATE_OUT_GPIO_PIN 			GPIO_Pin_11 			//VIBRATE_OUT ���ź�
#define VIBRATE_OUT_PIN_ID 				11 						//VIBRATE_OUT �������
#define VIBRATE_OUT_GPIO_PORT 			GPIOA 					//VIBRATE_OUT �˿ں�
#define VIBRATE_OUT_GPIO_CLK 			RCC_APB2Periph_GPIOA 	//VIBRATE_OUT ʱ��
//#define VIBRATE_OUT_FUN_OUT 		PAout 					//VIBRATE_OUT ����˿����ú���

#define VIBRATE_OUT_FUN_IN 				PAin 					//VIBRATE_OUT ����˿����ú���
#define VIBRATE_OUT_GPIO_MODE 			GPIO_Mode_IPU 			//VIBRATE_OUT �ⲿ�ж�����ģʽ
#define VIBRATE_OUT_ACTIVE_LEVEL		0 						//VIBRATE_OUT ��Ч��ƽΪ�͵�ƽ
#define VIBRATE_OUT_PORT_SRC 			GPIO_PortSourceGPIOA	//VIBRATE_OUT �ⲿ�ж϶˿ں�
#define VIBRATE_OUT_PIN_SRC 			GPIO_PinSource11 		//VIBRATE_OUT �ⲿ�ж����ź�
#define VIBRATE_OUT_EXTI_LINE 			EXTI_Line11 			//VIBRATE_OUT �ⲿ�жϺ�
#define VIBRATE_OUT_EXTI_TRIG 			EXTI_Trigger_Falling 	//VIBRATE_OUT �ⲿ�жϴ�����ʽ
#define VIBRATE_OUT_EXTI_IRQN 			EXTI15_10_IRQn 			//VIBRATE_OUT �ⲿ�ж� NVIC ��

///////////////////////
//PIR ������⴫����
#define PIR_OUT_GPIO_PIN 				GPIO_Pin_3 			//PIR_OUT ���ź�
#define PIR_OUT_PIN_ID 					3 						//PIR_OUT �������
#define PIR_OUT_GPIO_PORT 				GPIOB 					//PIR_OUT �˿ں�
#define PIR_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOB 	//PIR_OUT ʱ��
//#define PIR_OUT_FUN_OUT 		PBout //PIR_OUT ����˿����ú���

#define PIR_OUT_FUN_IN 					PBin 					//PIR_OUT ����˿����ú���
#define PIR_OUT_GPIO_MODE 				GPIO_Mode_IPD 			//PIR_OUT �ⲿ�ж�����ģʽ
#define PIR_OUT_ACTIVE_LEVEL 			1 //PIR_OUT ��Ч��ƽΪ�ߵ�ƽ
#define PIR_OUT_PORT_SRC 				GPIO_PortSourceGPIOB 	//PIR_OUT �ⲿ�ж϶˿ں�
#define PIR_OUT_PIN_SRC 				GPIO_PinSource3 		//PIR_OUT �ⲿ�ж����ź�
#define PIR_OUT_EXTI_LINE 				EXTI_Line3 			//PIR_OUT �ⲿ�жϺ�
#define PIR_OUT_EXTI_TRIG 				EXTI_Trigger_Rising 	//PIR_OUT �ⲿ�жϴ�����ʽ
#define PIR_OUT_EXTI_IRQN 				EXTI3_IRQn 			//PIR_OUT �ⲿ�ж� NVIC ��

///////////////////////
//DO101 ���洫����
#define HY_OUT_GPIO_PIN 				GPIO_Pin_12 			//HY_OUT ���ź�
#define HY_OUT_PIN_ID 					12						//HY_OUT �������
#define HY_OUT_GPIO_PORT 				GPIOA 					//HY_OUT �˿ں�
#define HY_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOA 	//HY_OUT ʱ��
//#define HY_OUT_FUN_OUT 			PAout 					//HY_OUT ����˿����ú���

#define HY_OUT_FUN_IN 					PAin 					//HY_OUT ����˿����ú���
#define HY_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//HY_OUT �ⲿ�ж�����ģʽ
#define HY_OUT_ACTIVE_LEVEL				0 						//HY_OUT ��Ч��ƽΪ�͵�ƽ
#define HY_OUT_PORT_SRC 				GPIO_PortSourceGPIOA	//HY_OUT �ⲿ�ж϶˿ں�
#define HY_OUT_PIN_SRC 					GPIO_PinSource12 		//HY_OUT �ⲿ�ж����ź�
#define HY_OUT_EXTI_LINE 				EXTI_Line12 			//HY_OUT �ⲿ�жϺ�
#define HY_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//HY_OUT �ⲿ�жϴ�����ʽ
#define HY_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//HY_OUT �ⲿ�ж� NVIC ��


//DO211 �������������
//#define HE_OUT_GPIO_PIN 				GPIO_Pin_13			//HE_OUT ���ź�
//#define HE_OUT_PIN_ID 					13						//HE_OUT �������
//#define HE_OUT_GPIO_PORT 				GPIOC					//HE_OUT �˿ں�
//#define HE_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOC 	//HE_OUT ʱ��
////#define HE_OUT_FUN_OUT 			PCout 					//HE_OUT ����˿����ú���

//#define HE_OUT_FUN_IN 					PCin 					//HE_OUT ����˿����ú���
//#define HE_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//HE_OUT �ⲿ�ж�����ģʽ
//#define HE_OUT_ACTIVE_LEVEL				0 						//HE_OUT ��Ч��ƽΪ�͵�ƽ
//#define HE_OUT_PORT_SRC 				GPIO_PortSourceGPIOC	//HE_OUT �ⲿ�ж϶˿ں�
//#define HE_OUT_PIN_SRC 					GPIO_PinSource13 		//HE_OUT �ⲿ�ж����ź�
//#define HE_OUT_EXTI_LINE 				EXTI_Line13			//HE_OUT �ⲿ�жϺ�
//#define HE_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//HE_OUT �ⲿ�жϴ�����ʽ
//#define HE_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//HE_OUT �ⲿ�ж� NVIC ��


//DO101 ��ȼ���崫�������
#define KR_OUT_GPIO_PIN 				GPIO_Pin_15			//KR_OUT ���ź�
#define KR_OUT_PIN_ID 					15						//KR_OUT �������
#define KR_OUT_GPIO_PORT 				GPIOA 					//KR_OUT �˿ں�
#define KR_OUT_GPIO_CLK 				RCC_APB2Periph_GPIOA 	//KR_OUT ʱ��
//#define KR_OUT_FUN_OUT 			PAout 					//KR_OUT ����˿����ú���

#define KR_OUT_FUN_IN 					PAin 					//KR_OUT ����˿����ú���
#define KR_OUT_GPIO_MODE 				GPIO_Mode_IPU 			//KR_OUT �ⲿ�ж�����ģʽ
#define KR_OUT_ACTIVE_LEVEL				0 						//KR_OUT ��Ч��ƽΪ�͵�ƽ
#define KR_OUT_PORT_SRC 				GPIO_PortSourceGPIOA	//KR_OUT �ⲿ�ж϶˿ں�
#define KR_OUT_PIN_SRC 					GPIO_PinSource15 		//KR_OUT �ⲿ�ж����ź�
#define KR_OUT_EXTI_LINE 				EXTI_Line15 			//KR_OUT �ⲿ�жϺ�
#define KR_OUT_EXTI_TRIG 				EXTI_Trigger_Falling 	//KR_OUT �ⲿ�жϴ�����ʽ
#define KR_OUT_EXTI_IRQN 				EXTI15_10_IRQn 			//KR_OUT �ⲿ�ж� NVIC ��

/////////////////////////////////////////////////////////////////////////////////////////////////
//IO ��������
#define VIBRATE_OUT 					VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID) 		//��ȡ VIBRATE_OUT �͵�ƽ��Ч
#define PIR_OUT 						PIR_OUT_FUN_IN(PIR_OUT_PIN_ID) 				//��ȡ PIR_OUT �ߵ�ƽ��Ч
#define HY_OUT							HY_OUT_FUN_IN(HY_OUT_PIN_ID)				//��ȡHY_OUT�͵�ƽ��Ч
//#define HE_OUT							HE_OUT_FUN_IN(HE_OUT_PIN_ID)				//��ȡHE_OUT�͵�ƽ��Ч
#define KR_OUT							KR_OUT_FUN_IN(KR_OUT_PIN_ID)				//��ȡKR_OUT�͵�ƽ��Ч

extern uint8_t VIBRATE_OUT_flag;
extern uint8_t PIR_OUT_flag;
extern uint8_t HY_OUT_flag;
extern uint8_t HE_OUT_flag;
extern uint8_t KR_OUT_flag;

/* ���ⲿ���õĺ������� */

void bsp_InitIoSensor(void);

#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
