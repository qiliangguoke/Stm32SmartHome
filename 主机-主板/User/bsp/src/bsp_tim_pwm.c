#include "bsp.h"

/*
***************************************************************************
*          函 数 名：bsp_GetRCCofGPIO
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx)
{
	uint32_t rcc=0;
	
	if(GPIOx==GPIOA)
	{
		rcc = RCC_APB2Periph_GPIOA;
	}
	else if(GPIOx==GPIOB)
	{
		rcc = RCC_APB2Periph_GPIOB;
	}
	else if(GPIOx==GPIOC)
	{
		rcc = RCC_APB2Periph_GPIOC;
	}
	else if(GPIOx==GPIOD)
	{
		rcc = RCC_APB2Periph_GPIOD;
	}
	else if(GPIOx==GPIOE)
	{
		rcc = RCC_APB2Periph_GPIOE;
	}
	else if(GPIOx==GPIOF)
	{
		rcc = RCC_APB2Periph_GPIOF;
	}
	else if(GPIOx==GPIOG)
	{
		rcc = RCC_APB2Periph_GPIOG;
	}
	
	return rcc;
}

/*
***************************************************************************
*          函 数 名：bsp_GetRCCofTIM
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIMx)
{
	uint32_t rcc=0;
	
	/*
		APB1 定时器有 TIM2,TIM3,TIM4,TIM5,TIM6,TIM7
		APB2 定时器有 TIM1,TIM8
	*/
	
	if(TIMx == TIM1)
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	else if(TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	
	/*下面是APB1时钟*/
	else if(TIMx == TIM2)
	{
		rcc = RCC_APB1Periph_TIM2;
	}
	else if(TIMx == TIM3)
	{
		rcc = RCC_APB1Periph_TIM3;
	}
	else if(TIMx == TIM4)
	{
		rcc = RCC_APB1Periph_TIM4;
	}
	else if(TIMx == TIM5)
	{
		rcc = RCC_APB1Periph_TIM5;
	}
	else if(TIMx == TIM6)
	{
		rcc = RCC_APB1Periph_TIM6;
	}
	else if(TIMx == TIM7)
	{
		rcc = RCC_APB1Periph_TIM7;
	}
	
	return rcc;   //返回
		
}
/*
***************************************************************************
*          函 数 名：bsp_ConfingTimGpio
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_PinX,TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/**/
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx),ENABLE);
	
	/**/
	if((TIMx==TIM1)||(TIMx==TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	/*配置GPIO*/
	GPIO_InitStructure.GPIO_Pin=GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
}
/*
***************************************************************************
*          函 数 名：bsp_SetTIMOutPWM
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel,
	uint32_t _ulFreq,uint32_t _ulDutyCycle)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	uint16_t usPeriod;								//
	uint16_t usPrescaler;							//
	uint32_t uiTIMxCLK;								//
	
	bsp_ConfigTimGpio(GPIOx,GPIO_Pin,TIMx);    /**/
	
	/*
	
	
	
	
	
	
	
	*/
	if((TIMx==TIM1)||(TIMx==TIM8))
	{
		/**/
		uiTIMxCLK =SystemCoreClock;
	}
	else
	{
		uiTIMxCLK = SystemCoreClock;  //System
	}
	
	if(_ulFreq<100)
	{
		usPrescaler = 10000-1;
		usPeriod = (uiTIMxCLK/10000)/_ulFreq-1;
	}
	else if(_ulFreq<3000)
	{
		usPrescaler = 100-1;
		usPeriod = (uiTIMxCLK/100)/_ulFreq-1;	
	}
	else
	{
		usPrescaler = 0;
		usPeriod = uiTIMxCLK / _ulFreq-1;	
	}
	
	/**/
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	/**/
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=(_ulDutyCycle*usPeriod)/10000;
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/**/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	
	if(_ucChannel==1)
	{
		TIM_OC1Init(TIMx,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	else if(_ucChannel==2)
	{
		TIM_OC2Init(TIMx,&TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	else if(_ucChannel==3)
	{
		TIM_OC3Init(TIMx,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	else if(_ucChannel==4)
	{
		TIM_OC4Init(TIMx,&TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	/*TIMx enable counter*/
	TIM_Cmd(TIMx,ENABLE);
	
	/**/
	if((TIMx==TIM1)||(TIMx==TIM8))
	{
		TIM_CtrlPWMOutputs(TIMx,ENABLE);
	}
}
/*
***************************************************************************
*          函 数 名：bsp_SetTIMforInt
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/

void bsp_SetTIMforInt(TIM_TypeDef* TIMx,uint32_t _ulFreq,uint8_t _PreemptionPriority,uint8_t _SubPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   /*时基结构体*/
	
	uint16_t usPeriod;					//暂存的自动重装在寄存器的值
	uint16_t usPrescaler;				//暂存自动重装载定时器的值
	uint32_t uiTIMxCLK;					//暂存时钟频率
	
	/*使能TIM时钟*/
	if((TIMx == TIM1)||(TIMx == TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	if(_ulFreq == 0)
	{
		TIM_Cmd(TIMx,DISABLE);
		
		/*关闭TIM定时更新中断（Update）*/
		{
			NVIC_InitTypeDef NVIC_InitStructure;   /*中断结构体在 misc.h中定义*/
			uint8_t irq = 0;   /*中断号，定义在 stm32f1xx.h*/
			
			if(TIMx == TIM1)
				irq = TIM1_UP_IRQn;
			else if(TIMx == TIM2)
				irq = TIM2_IRQn;
			else if(TIMx == TIM3)
				irq = TIM3_IRQn;
			else if(TIMx == TIM4)
				irq = TIM4_IRQn;
			else if(TIMx == TIM5)
				irq = TIM5_IRQn;
			else if(TIMx == TIM6)
				irq = TIM6_IRQn;
			else if(TIMx == TIM7)
				irq = TIM7_IRQn;
			else if(TIMx == TIM8)
				irq = TIM8_UP_IRQn;
			
			/*中断优先级NVIC设置*/
			NVIC_InitStructure.NVIC_IRQChannel = irq;									/*TIMx中断IROn*/
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
			NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
			NVIC_Init(&NVIC_InitStructure);
		/*初始化NVIC寄存器*/
		}
		return;
	}
	
	/*
	
	
	
	
	*/
	
	if((TIMx == TIM1)||(TIMx == TIM8))
	{
		/**/
		uiTIMxCLK = SystemCoreClock;
	}
	else 
	{
		uiTIMxCLK = SystemCoreClock;
	}
	
	if(_ulFreq<100)
	{
		usPrescaler = 10000-1;
		usPeriod = (uiTIMxCLK/10000)/_ulFreq-1;
	}
	else if(_ulFreq<3000)
	{
		usPrescaler = 100-1;
		usPeriod = (uiTIMxCLK/100)/_ulFreq-1;
	}
	else
	{
		usPrescaler = 0;
		usPeriod = uiTIMxCLK / _ulFreq - 1;
	}
	
	/*Timebase configuration*/
	
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	/**/
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
	
	/**/
	TIM_Cmd(TIMx,ENABLE);
	
	/**/
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		uint8_t irq=0;
		
		if(TIMx == TIM1)
				irq = TIM1_UP_IRQn;
			else if(TIMx == TIM2)
				irq = TIM2_IRQn;
			else if(TIMx == TIM3)
				irq = TIM3_IRQn;
			else if(TIMx == TIM4)
				irq = TIM4_IRQn;
			else if(TIMx == TIM5)
				irq = TIM5_IRQn;
			else if(TIMx == TIM6)
				irq = TIM6_IRQn;
			else if(TIMx == TIM7)
				irq = TIM7_IRQn;
			else if(TIMx == TIM8)
				irq = TIM8_UP_IRQn;
			
			/*中断优先级NVIC设置*/
			NVIC_InitStructure.NVIC_IRQChannel = irq;									/*TIMx中断IROn*/
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	}
	
}
