/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_uart.c
*	��    �� : V1.0
*	˵    �� : ʵ��printf�ض��򵽴���1����֧��printf��Ϣ��USART1
*

*********************************************************************************************************
*/
#include "bsp.h"

#ifdef EN_USART3_RX	//���ʹ���˽���

uint8_t USART3_RX_BUF[USART2_BUF_LEN];		//���ջ��壬���USART2_BUF_LEN���ֽ�

uint8_t USART3_RX_CNT = 0;					//���յ������ݳ���
/*
*********************************************************************************************************
*	�� �� ��: USART3_IRQHandler
*	����˵��:����3�ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void USART3_IRQHandler(void)//����1�жϷ�����
{
	uint8_t res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE )!= RESET)//�����жϣ����յ�һ���ֽڵ����ݣ�
	{
		res = USART_ReceiveData(USART3);//��ȡ���յ�������
		if(USART3_RX_CNT <USART3_BUF_LEN)
		{
			USART3_RX_BUF[USART3_RX_CNT] = res;//��¼���յ�������ֵ
			USART3_RX_CNT++;			//����������1
		}
		
	}
	
}

#endif

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart3
*	����˵��:��ʼ��CPU��USART3����Ӳ���豸,δ�����жϡ�
*	��    ��:  uint32_t baud:������
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart3(uint32_t baud)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//1.����4 TX = PC9  RX = PC10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��GPIOʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//��UARTʱ��
	
	//���� USART TX Ϊ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PC10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIOC.9
	
	//���� USART RX Ϊ���ù���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIOA.10
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ

#ifdef EN_USART2_RX		//���ʹ���˽���
	//2.���ô���Ӳ������
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART3,&USART_InitStructure);//��ʼ������1
	
		
	
	//3.Usart1 NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	//4.ʹ�ܴ���4
	USART_Cmd(USART3,ENABLE);
	
#endif
		
}

/*
*********************************************************************************************************
*	�� �� ��: USART3_Send_Data
*	����˵��:USART3����LEN���ֽ�
*	��    ��:  buf:����l���׵�ַ
	len�����͵��ֽ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t = 0;t < len;t ++)//ѭ����������
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,buf[t]);
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)== RESET);
	USART3_RX_CNT = 0;
}
/**********************************************************************************************************/
/*
*********************************************************************************************************
*	�� �� ��: USART2_Receive_Data
*	����˵��:USART2��ѯ���յ�������
*	��    ��:  buf:���ջ����׵�ַ
	len�����������ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = USART3_RX_CNT;
	uint8_t i = 0;
	
	*len = 0;//Ĭ��Ϊ0
	delay_ms(10);//�ȴ�10ms����������10msû�н��յ�һ����������Ϊ���ܽ���
	if(rxlen == USART3_RX_CNT && rxlen)//���յ��������ҽ��������
	{
		for(i = 0;i < rxlen; i++)
		{
			buf[i] = USART3_RX_BUF[i];
		}
		*len = USART3_RX_CNT;//��¼�������ݳ���
		USART3_RX_CNT = 0;//����
	}
}



