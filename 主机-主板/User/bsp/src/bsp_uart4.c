#include "bsp.h"

#ifdef EN_UART4_RX //如果使能了接收
//接收缓存区
uint8_t UART4_RX_BUF[UART4_BUF_LEN]; //接收缓冲,最大 UART4_BUF_LEN 个字节.
//接收到的数据长度
uint8_t UART4_RX_CNT=0; 

/*
*********************************************************************************************************
* 函 数 名: UART4_IRQHandler
* 功能说明: 串口 4 中断。
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
	uint8_t res; 
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //接收到数据
	{
		res =USART_ReceiveData(UART4); //读取接收到的数据
		if(UART4_RX_CNT<UART4_BUF_LEN)
		{
			UART4_RX_BUF[UART4_RX_CNT]=res; //记录接收到的值
			UART4_RX_CNT++; //接收数据增加 1 
		} 
	} 
} 
#endif
/*
*********************************************************************************************************
* 函 数 名: bsp_InitUart4
* 功能说明: 初始化 CPU 的 UART4 串口硬件设备。
* 形 参：uint32_t bound:波特率
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart4(uint32_t bound)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能 GPIOC 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能 UART4 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);//复位串口 4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);//停止复位
	
#ifdef EN_UART4_RX //如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //使能串口 4 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(UART4, ENABLE); //使能串口
	#endif

}
/*
*********************************************************************************************************
* 函 数 名: UART4_Send_Data
* 功能说明: UART4 发送 len 个字节
* 形 参： buf:发送区首地址
* len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过 64 个字节)
* 返 回 值: 无
*********************************************************************************************************
*/
void UART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
 for(t=0;t<len;t++) //循环发送数据
{ 
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); 
	USART_SendData(UART4,buf[t]);
}
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	UART4_RX_CNT=0; 
}
/*
*********************************************************************************************************
* 函 数 名: UART4_Receive_Data
* 功能说明: UART4 查询接收到的数据
* 形 参： buf:接收缓存首地址
* len:读到的数据长度
* 返 回 值: 无
*********************************************************************************************************
*/
void UART4_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART4_RX_CNT;
	uint8_t i=0;
	*len=0; //默认为 0
	delay_ms(10); //等待 10ms,连续超过 10ms 没有接收到一个数据,则认为接收结束
if(rxlen==UART4_RX_CNT&&rxlen) //接收到了数据,且接收完成了
{
	for(i=0;i<rxlen;i++)
	{
		buf[i]=UART4_RX_BUF[i];
	}
		*len=UART4_RX_CNT; //记录本次数据长度
		UART4_RX_CNT=0; //清零
}
}
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
