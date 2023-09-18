/*
*********************************************************************************************************
*
*	模块名称 : 串口驱动模块
*	文件名称 : bsp_uart.c
*	版    本 : V1.0
*	说    明 : 实现printf重定向到串口1，即支持printf信息到USART1
*

*********************************************************************************************************
*/
#include "bsp.h"

#ifdef EN_USART2_RX	//如果使能了接收

uint8_t USART2_RX_BUF[USART2_BUF_LEN];		//接收缓冲，最大USART2_BUF_LEN个字节
uint8_t ReceiveState2;						//接收标记
uint8_t USART2_RX_CNT = 0;					//接收到的数据长度
/*
*********************************************************************************************************
*	函 数 名: USART2_IRQHandler
*	功能说明:串口2中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/

void USART2_IRQHandler(void)//串口2中断服务函数
{
	uint8_t res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE )!= RESET)//接收中断（接收到一个字节的数据）
	{
		res = USART_ReceiveData(USART2);//读取接收到的数据
		if(USART2_RX_CNT <USART2_BUF_LEN)
		{
			USART2_RX_BUF[USART2_RX_CNT] = res;//记录接收到的数据值
			USART2_RX_CNT++;			//接收数增加1
			ReceiveState2=1;
		}
	}
}

#endif

/*
*********************************************************************************************************
*	函 数 名:UART2_STA_Clr		 
*	功能说明:串口2状态清楚函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/	
void Uart2_STA_Clr(void)
{
	USART2_RX_CNT=0;	//串口BUF计数清零
	ReceiveState2=0;	//接收状态清零
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart2
*	功能说明:初始化CPU的USART4串口硬件设备,未启用中断。
*	形    参:  uint32_t baud:波特率
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_InitUart2(uint32_t baud)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//1.串口4 TX = PC9  RX = PC10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开GPIO时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//打开UART时钟
	
	//配置 USART TX 为复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推免输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIOC.9
	
	//配置 USART RX 为复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIOA.10
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位

#ifdef EN_USART2_RX		//如果使能了接收
	//2.配置串口硬件参数
	USART_InitStructure.USART_BaudRate = baud;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_Init(USART2,&USART_InitStructure);//初始化串口1
	
		
	
	//3.Usart1 NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级3
	NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化NVIC寄存器
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启串口接收中断
	//4.使能串口2
	USART_Cmd(USART2,ENABLE);
	
#endif
		
}

/*
*********************************************************************************************************
*	函 数 名: USART2_Send_Data
*	功能说明:USART2发送LEN个字节
*	形    参:  buf:发送l区首地址
	len：发送的字节数（）
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t = 0;t < len;t ++)//循环发送数据
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
*	函 数 名: USART2_Receive_Data
*	功能说明:USART2查询接收到的数据
*	形    参:  buf:接收缓存首地址
	len：读到的数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = USART2_RX_CNT;
	uint8_t i = 0;
	
	*len = 0;//默认为0
	delay_ms(10);//等待10ms，连续超过10ms没有接收到一个数据则认为接受结束
	if(rxlen == USART2_RX_CNT && rxlen)//接收到了数据且接收完成了
	{
		for(i = 0;i < rxlen; i++)
		{
			buf[i] = USART2_RX_BUF[i];
		}
		*len = USART2_RX_CNT;//记录本次数据长度
		USART2_RX_CNT = 0;//清零
	}
}



