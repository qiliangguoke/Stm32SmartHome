#include "bsp.h"
/*
*********************************************************************************************************
* �� �� ��: bsp_InitRelay
* ����˵��: ���ü̵��� Relay ��ص� GPIO��
* �� ��: ��
* �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitRelay(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* �� GPIO ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY, ENABLE);
	/*
	�������е� RELAY GPIO Ϊ�������ģʽ
	���ڽ� GPIO ����Ϊ���ʱ��GPIO ����Ĵ�����ֵȱʡ�� 0����˻����� LED ����.
	�����Ҳ�ϣ���ģ�����ڸı� GPIO Ϊ���ǰ���ȹر� RELAY
	*/
	bsp_RelayOff(1);
	bsp_RelayOff(2);
	bsp_RelayOff(3);
	bsp_RelayOff(4);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* �������ģʽ */
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN; /* RELAY1 �˿� */
	GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure); /* ��ʼ�� RELAY1 */
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN; /* RELAY2 �˿� */
	GPIO_Init(RELAY2_GPIO_PORT, &GPIO_InitStructure); /* ��ʼ�� RELAY2 */
	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN; /* RELAY3 �˿� */
	GPIO_Init(RELAY3_GPIO_PORT, &GPIO_InitStructure); /* ��ʼ�� RELAY3 */
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN; /* RELAY4 �˿� */
	GPIO_Init(RELAY4_GPIO_PORT, &GPIO_InitStructure); /* ��ʼ�� RELAY4 */
}
/*
*********************************************************************************************************
* �� �� ��: bsp_RelayOn
* ����˵��: �պ�ָ���ļ̵�����
* �� ��: _no : �̵�����ţ���Χ 1 - 4
* �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayOn(uint8_t _no)
{
	if (_no == 1)
	{
		RELAY1 = 1;
	}
	else if (_no == 2)
	{
		RELAY2 = 1;
	}
	else if (_no == 3)
	{
		RELAY3 = 1;
	}
	else if (_no == 4)
	{
		RELAY4 = 1;
	}
}
/*
*********************************************************************************************************
* �� �� ��: bsp_RelayOff
* ����˵��: �Ͽ�ָ���ļ̵�����
* �� ��: _no : �̵�����ţ���Χ 1 - 4
* �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayOff(uint8_t _no)
{
	if (_no == 1)
	{
		RELAY1 = 0;
	}
	else if (_no == 2)
	{
		RELAY2 = 0;
	}
	else if (_no == 3)
	{
		RELAY3 = 0;
	}
	else if (_no == 4)
	{
		RELAY4 = 0;
	}
}
/*
*********************************************************************************************************
* �� �� ��: bsp_RelayToggle
* ����˵��: ��תָ���ļ̵�����
* �� ��: _no : �̵�����ţ���Χ 1 - 2
* �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayToggle(uint8_t _no)
{
	if (_no == 1)
	{
		RELAY1_GPIO_PORT->ODR ^= RELAY1_GPIO_PIN;
	}
	else if (_no == 2)
	{
		RELAY2_GPIO_PORT->ODR ^= RELAY2_GPIO_PIN;
	}
	else if (_no == 3)
	{
		RELAY3_GPIO_PORT->ODR ^= RELAY3_GPIO_PIN;
	}
	else if (_no == 4)
	{
		RELAY4_GPIO_PORT->ODR ^= RELAY4_GPIO_PIN;
	}
}
/*
*********************************************************************************************************
* �� �� ��: bsp_IsRelayOn
* ����˵��: �жϼ̵����Ƿ��Ѿ��պϡ�
* �� ��: _no : �̵�����ţ���Χ 1 - 4
* �� �� ֵ: 1 ��ʾ�Ѿ��պϣ�0 ��ʾδ�պ�
*********************************************************************************************************
*/
uint8_t bsp_IsRelayOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((RELAY1_GPIO_PORT->ODR & RELAY1_GPIO_PIN) == RELAY1_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((RELAY2_GPIO_PORT->ODR & RELAY2_GPIO_PIN) == RELAY2_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 3)
	{
		if ((RELAY3_GPIO_PORT->ODR & RELAY3_GPIO_PIN) == RELAY3_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 4)
	{
		if ((RELAY4_GPIO_PORT->ODR & RELAY4_GPIO_PIN) == RELAY4_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}


