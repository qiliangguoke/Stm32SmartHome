#ifndef __BSP_FAN_H
#define __BSP_FAN_H

#include "sys.h"

///////////////////////////////////////////////////////////
//��ֲ�޸���
//����ʵ�ʶ˿��޸�
//��ǰ�汾����������ֲ(ע�⣬�˿ڽ��������ڷ� JTAG/SWD ���ţ������ JTAG ���ţ���Ҫ�� AFIO ʱ�ӣ���ʧ�� JTAG)


#ifdef WSNEP_V01
#define RCC_ALL_FAN ( FAN1_GPIO_CLK | FAN2_GPIO_CLK )
#define FAN1_GPIO_PIN GPIO_Pin_8 //FAN1 ���ź�
#define FAN1_PIN_ID 8 //FAN1 �������
#define FAN1_GPIO_PORT GPIOB //FAN1 �˿ں�
#define FAN1_GPIO_CLK RCC_APB2Periph_GPIOB //FAN1 ʱ��
#define FAN1_FUN_OUT PBout //FAN1 ����˿����ú���
//#define FAN1_FUN_IN PBin //FAN1 ����˿����ú���

////
#define FAN2_GPIO_PIN GPIO_Pin_9 //FAN2 ���ź�
#define FAN2_PIN_ID 9 //FAN2 �������
#define FAN2_GPIO_PORT GPIOB //FAN2 �˿ں�
#define FAN2_GPIO_CLK RCC_APB2Periph_GPIOB //FAN2 ʱ��
#define FAN2_FUN_OUT PBout //FAN2 ����˿����ú���
//#define FAN2_FUN_IN PBin //FAN2 ����˿����ú���
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
//IO ��������
#define FAN1 FAN1_FUN_OUT(FAN1_PIN_ID) //FAN1
#define FAN2 FAN2_FUN_OUT(FAN2_PIN_ID) //FAN2


/* ���ⲿ���õĺ������� */
void bsp_InitFan(void);
void bsp_FanOn(uint8_t _no);
void bsp_FanOff(uint8_t _no);
void bsp_FanToggle(uint8_t _no);
uint8_t bsp_IsFanOn(uint8_t _no);

#endif
