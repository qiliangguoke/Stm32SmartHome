/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ�ƣ�����½�����ʹ�ã��������ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-08-02 waroyal  ��ʽ����
*
*	Copyright (C), 2014-2019, �����׵���
*
*********************************************************************************************************
*/

#include "bsp.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);
	
	bsp_LedOff(1);
	bsp_LedOff(2);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */
	
	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
	
}



/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if (_no == 1)
	{
		LED0=0;
	}
	else if (_no==2)
	{
		LED1=0;
	}
}

void bsp_LedOff(uint8_t _no)
{
	if (_no == 1)
	{
		LED0=1;
	}
	else if (_no==2)
	{
		LED1=1;
	}
}

void bsp_LedToggle(uint8_t _no)
{
	if(_no==1)
	{
		LED0_GPIO_PORT->ODR^=LED0_GPIO_PIN;
	}
	else if (_no==2)
	{
		LED1_GPIO_PORT->ODR^=LED1_GPIO_PIN;
	}
}

uint8_t bsp_IsLedOn(uint8_t _on)
{
	if(_on==1)
	{
		if((LED0_GPIO_PORT->ODR & LED0_GPIO_PIN)==LED0_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if (_on==2)
	{
		if((LED1_GPIO_PORT->ODR & LED1_GPIO_PIN)==LED1_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}


/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
