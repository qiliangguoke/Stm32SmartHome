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

#if EN_USART1_RX	//���ʹ���˽���

uint8_t USART_RX_BUF[USART_REC_LEN];		//���ջ��壬���USART_REC_LEN���ֽ�
uint8_t ReceiveState = 0;					//�����ַ���
uint16_t RxCounter = 0;						//����״ָ̬ʾ��0��δ���ܣ�1�����գ�

#endif

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart1
*	����˵��:��ʼ��CPU��USART1����Ӳ���豸,δ�����жϡ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart1(uint32_t baud)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
#if EN_USART1_RX	//���ʹ���˽���
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	
#endif
	
	//1.����1 TX = PA9  RX = PA10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��GPIOʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//��UARTʱ��
	
	//���� USART TX Ϊ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIOA.9
	
	//���� USART RX Ϊ���ù���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIOA.10
	
	//2.���ô���Ӳ������
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART1,&USART_InitStructure);//��ʼ������1
	
#if EN_USART1_RX		//���ʹ���˽���
	
	//3.Usart1 NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//�������ڿ����ж�
#endif
	//4.ʹ�ܴ���1
	USART_Cmd(USART1,ENABLE);	
}

void USART1_IRQHandler(void)//����1�жϷ�����
{
	uint8_t Res = Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE )!= RESET)//�����жϣ����յ�һ���ֽڵ����ݣ�
	{
		USART_RX_BUF[RxCounter++] = USART1->DR;//�ѽ��յ������ݱ���
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)//�����жϣ����յ�1֡�����ݣ�
	{
		Res = USART1->SR;//��SR�Ĵ���
		Res = USART1->DR;//��DR�Ĵ������ȶ�SR���ڶ�DR������Ϊ�����IDLE�жϣ�
		ReceiveState = 1;//��ǽ���״̬��λ
	}
}

//����0״̬�������
void Uart0_STA_Clr(void)
{
	RxCounter = 0;//����BUF��������
	ReceiveState = 0;//����״̬����
}

//�������´��룬֧��printf������������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//���塪��sys����exit�����Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}

//�ض���fputc��������������ʹ��printf�����Ӵ���1��ӡ���
int fputc(int ch,FILE *f)
{
	while((USART1->SR&0x40)==0){};//ѭ�����ͣ�ֱ���������
	USART1->DR = (u8)ch;
	return ch;	
}
#endif

//USART1����len���֣�buf���������׵�ַ len�����͵��ֽ���
void USART1_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++)//ѭ������
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)== RESET);
		USART_SendData(USART1,buf[t]);			
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)== RESET);
}


/**********************************************************************************************************/
