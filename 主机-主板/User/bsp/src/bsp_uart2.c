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

#ifdef EN_USART2_RX	//���ʹ���˽���

uint8_t USART2_RX_BUF[USART2_BUF_LEN];		//���ջ��壬���USART2_BUF_LEN���ֽ�
uint8_t ReceiveState2;						//���ձ��
uint8_t USART2_RX_CNT = 0;					//���յ������ݳ���
/*
*********************************************************************************************************
*	�� �� ��: USART2_IRQHandler
*	����˵��:����2�ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void USART2_IRQHandler(void)//����2�жϷ�����
{
	uint8_t res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE )!= RESET)//�����жϣ����յ�һ���ֽڵ����ݣ�
	{
		res = USART_ReceiveData(USART2);//��ȡ���յ�������
		if(USART2_RX_CNT <USART2_BUF_LEN)
		{
			USART2_RX_BUF[USART2_RX_CNT] = res;//��¼���յ�������ֵ
			USART2_RX_CNT++;			//����������1
			ReceiveState2=1;
		}
	}
}

#endif

/*
*********************************************************************************************************
*	�� �� ��:UART2_STA_Clr		 
*	����˵��:����2״̬�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/	
void Uart2_STA_Clr(void)
{
	USART2_RX_CNT=0;	//����BUF��������
	ReceiveState2=0;	//����״̬����
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart2
*	����˵��:��ʼ��CPU��USART4����Ӳ���豸,δ�����жϡ�
*	��    ��:  uint32_t baud:������
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart2(uint32_t baud)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//1.����4 TX = PC9  RX = PC10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��GPIOʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//��UARTʱ��
	
	//���� USART TX Ϊ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIOC.9
	
	//���� USART RX Ϊ���ù���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIOA.10
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ

#ifdef EN_USART2_RX		//���ʹ���˽���
	//2.���ô���Ӳ������
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART2,&USART_InitStructure);//��ʼ������1
	
		
	
	//3.Usart1 NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	//4.ʹ�ܴ���2
	USART_Cmd(USART2,ENABLE);
	
#endif
		
}

/*
*********************************************************************************************************
*	�� �� ��: USART2_Send_Data
*	����˵��:USART2����LEN���ֽ�
*	��    ��:  buf:����l���׵�ַ
	len�����͵��ֽ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t = 0;t < len;t ++)//ѭ����������
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,buf[t]);
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)== RESET);
	USART2_RX_CNT = 0;
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
void USART2_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = USART2_RX_CNT;
	uint8_t i = 0;
	
	*len = 0;//Ĭ��Ϊ0
	delay_ms(10);//�ȴ�10ms����������10msû�н��յ�һ����������Ϊ���ܽ���
	if(rxlen == USART2_RX_CNT && rxlen)//���յ��������ҽ��������
	{
		for(i = 0;i < rxlen; i++)
		{
			buf[i] = USART2_RX_BUF[i];
		}
		*len = USART2_RX_CNT;//��¼�������ݳ���
		USART2_RX_CNT = 0;//����
	}
}



