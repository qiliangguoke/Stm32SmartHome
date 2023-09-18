#include "bsp.h"


uint8_t VIBRATE_OUT_flag = 0; 	//震动传感器触发标志
uint8_t PIR_OUT_flag = 0; 		//人体红外传感器触发标志
uint8_t HY_OUT_flag = 0;		//火焰传感器触发标志
uint8_t HE_OUT_flag = 0;		//霍尔传感器触发标志
uint8_t KR_OUT_flag = 0;		//可燃气体传感器出发标志

void SENSOR_IO_Init(void); 		//IO 初始化
void EXTI_Sensor_Init(void);	//中断初始化

/*
*********************************************************************************************************
* 函 数 名: bsp_InitIoSensor
* 功能说明: 高低电平型传感器初始化函数
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitIoSensor(void)
{
	SENSOR_IO_Init();
	EXTI_Sensor_Init();
}
/*
*********************************************************************************************************
* 函 数 名: SENSOR_IO_Init
* 功能说明: 高低电平型传感器初始化函数
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void SENSOR_IO_Init(void) //IO 初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //使能复用功能时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭 jtag，使能 SWD，可以用 SWD 模式调试
	
	RCC_APB2PeriphClockCmd(VIBRATE_OUT_GPIO_CLK,ENABLE); //使能 VIBRATE_OUT 时钟	
	//初始化 VIBRATE_OUT 
	GPIO_InitStructure.GPIO_Pin = VIBRATE_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = VIBRATE_OUT_GPIO_MODE; //设置成输入，默认上拉 
	GPIO_Init(VIBRATE_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化 VIBRATE
	
	RCC_APB2PeriphClockCmd(PIR_OUT_GPIO_CLK,ENABLE); //使能 PIR_OUT 时钟	
	//初始化 PIR_OUT 
	GPIO_InitStructure.GPIO_Pin = PIR_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = PIR_OUT_GPIO_MODE; //设置成输入，默认下拉 
	GPIO_Init(PIR_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化 PIR
		
	RCC_APB2PeriphClockCmd(HY_OUT_GPIO_CLK,ENABLE); //使能 HY_OUT 时钟	
	//初始化 HY_OUT 
	GPIO_InitStructure.GPIO_Pin = HY_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = HY_OUT_GPIO_MODE; //设置成输入，默认上拉 
	GPIO_Init(HY_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化 HY
	
//	RCC_APB2PeriphClockCmd(HE_OUT_GPIO_CLK,ENABLE); //使能 HE_OUT 时钟	
//	//初始化 HE_OUT 
//	GPIO_InitStructure.GPIO_Pin = HE_OUT_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = HE_OUT_GPIO_MODE; //设置成输入，默认上拉 
//	GPIO_Init(HE_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化 HE
	
	RCC_APB2PeriphClockCmd(KR_OUT_GPIO_CLK,ENABLE); //使能 KR_OUT 时钟	
	//初始化 HE_OUT 
	GPIO_InitStructure.GPIO_Pin = KR_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = KR_OUT_GPIO_MODE; //设置成输入，默认上拉 
	GPIO_Init(KR_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化 KR
}
/*
*********************************************************************************************************
* 函 数 名: EXTI_Sensor_Init
* 功能说明: 高低电平型传感器外部中断初始化函数
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void EXTI_Sensor_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭 jtag，使能 SWD，可以用 SWD 模式调试

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //使能复用功能时钟
	
	//VIBRATE 中断线以及中断初始化配置
	GPIO_EXTILineConfig(VIBRATE_OUT_PORT_SRC,VIBRATE_OUT_PIN_SRC); 
	
	EXTI_InitStructure.EXTI_Line = VIBRATE_OUT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = VIBRATE_OUT_EXTI_TRIG;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////	
	NVIC_InitStructure.NVIC_IRQChannel = VIBRATE_OUT_EXTI_IRQN; //使能 VIBRATE 所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	
	//PIR 中断线以及中断初始化配置
	GPIO_EXTILineConfig(PIR_OUT_PORT_SRC,PIR_OUT_PIN_SRC); 
	
	EXTI_InitStructure.EXTI_Line = PIR_OUT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = PIR_OUT_EXTI_TRIG;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////	
	NVIC_InitStructure.NVIC_IRQChannel = PIR_OUT_EXTI_IRQN; //使能 GAS 所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	
	//HY 中断线以及中断初始化配置
	GPIO_EXTILineConfig(HY_OUT_PORT_SRC,HY_OUT_PIN_SRC); 
	
	EXTI_InitStructure.EXTI_Line = HY_OUT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = HY_OUT_EXTI_TRIG;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////	
	NVIC_InitStructure.NVIC_IRQChannel = HY_OUT_EXTI_IRQN; //使能 HY 所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	
	//HE 中断线以及中断初始化配置
//	GPIO_EXTILineConfig(HE_OUT_PORT_SRC,HE_OUT_PIN_SRC); 
//	
//	EXTI_InitStructure.EXTI_Line = HE_OUT_EXTI_LINE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = HE_OUT_EXTI_TRIG;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
//	
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	NVIC_InitStructure.NVIC_IRQChannel = HE_OUT_EXTI_IRQN; //使能 HE 所在的外部中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	
	//KR 中断线以及中断初始化配置
	GPIO_EXTILineConfig(KR_OUT_PORT_SRC,KR_OUT_PIN_SRC); 
	
	EXTI_InitStructure.EXTI_Line = KR_OUT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = KR_OUT_EXTI_TRIG;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	NVIC_InitStructure.NVIC_IRQChannel = KR_OUT_EXTI_IRQN; //使能 KR 所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
}
/*
*********************************************************************************************************
* 函 数 名: EXTI3_IRQHandler
* 功能说明: 外部中断 3 服务程序
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void EXTI3_IRQHandler(void)
{	
	if(EXTI_GetITStatus(PIR_OUT_EXTI_LINE)!=RESET) 
	{
		if(PIR_OUT == PIR_OUT_ACTIVE_LEVEL) //PIR_OUT 动作为高电平
		{
			PIR_OUT_flag = 1;//标志置位，便于主函数轮询
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(PIR_OUT_EXTI_LINE);
	} 
}
/*
*********************************************************************************************************
* 函 数 名: EXTI15_10_IRQHandler
* 功能说明: 外部中断 15-10 服务程序
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(VIBRATE_OUT_EXTI_LINE)!=RESET) 
	{
		if(VIBRATE_OUT == VIBRATE_OUT_ACTIVE_LEVEL) //VIBRATE_OUT 动作为低电平
		{
			VIBRATE_OUT_flag = 1;//标志置位，便于主函数轮询
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(VIBRATE_OUT_EXTI_LINE);
	} 
	
	if(EXTI_GetITStatus(HY_OUT_EXTI_LINE)!=RESET) 
	{
		if(HY_OUT == HY_OUT_ACTIVE_LEVEL) //HY_OUT 动作为低电平
		{
			HY_OUT_flag = 1;//标志置位，便于主函数轮询
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(HY_OUT_EXTI_LINE);
	}
	
//	if(EXTI_GetITStatus(HE_OUT_EXTI_LINE)!=RESET) 
//	{
//		if(HE_OUT == HE_OUT_ACTIVE_LEVEL) //HE_OUT 动作为低电平
//		{
//			HE_OUT_flag = 1;//标志置位，便于主函数轮询
//		}
//		//中断处理，自行添加
//		EXTI_ClearITPendingBit(HE_OUT_EXTI_LINE);
//	}
	
	if(EXTI_GetITStatus(KR_OUT_EXTI_LINE)!=RESET) 
	{
		if(KR_OUT == KR_OUT_ACTIVE_LEVEL) //KR_OUT 动作为低电平
		{
			KR_OUT_flag = 1;//标志置位，便于主函数轮询
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(KR_OUT_EXTI_LINE);
	}
}
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
