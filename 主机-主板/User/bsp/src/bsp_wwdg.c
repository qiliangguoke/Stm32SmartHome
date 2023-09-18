#include "bsp.h"

/*
***************************************************************************
*          函 数 名： bsp_InitWwdg
*          功能说明： 窗口看门狗配置
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_InitWwdgNVIC(FunctionalState NewState);

void bsp_InitWwdg(uint8_t _ucTreg,uint8_t _ucWreg,uint32_t WWDG_Prescaler)
{
	/*检测系统是否从窗口看门狗复位中恢复*/
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)!=RESET)
	{
		/*清楚复位标志*/
		RCC_ClearFlag();
		printf("**窗口看门狗复位******************\r\n");
	}
	else
	{
		/*WWDGRST标志没有设置*/
		printf("**不是窗口看门狗复位，可能是上电复位或者手动按键复位*************\r\n");
	}
	
	/*使能WWDG时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	/*
	窗口看门狗分频设置
	比如选择WWDG_Prescaler_8
	*/
	WWDG_SetPrescaler(WWDG_Prescaler);
	
	/*
	设置窗口值是_ucWreg,用户必须在小于_ucWreg且大于0x40时刷新计数器，
	要不会造成系统复位
	*/
	WWDG_SetWindowValue(_ucWreg);
	
	/*
	使能WWDG，设置计数器
	比如设置_ucTreg=127 8分频时，那么溢出时间就是=~910us*64=58.24ms
	*/
	WWDG_Enable(_ucTreg);
	
	/*清除EWI中断标志*/
	WWDG_ClearFlag();
	
	/*设置WWDG的NVIC*/
	bsp_InitWwdgNVIC(ENABLE);
	
	/*使能EW中断*/
	WWDG_EnableIT();
}

/*
***************************************************************************
*          函 数 名：bsp_InitWwdgNVIC
*          功能说明：窗口看门狗NVIC配置
*          形   参：FunctionalState NewState
*          返 回 值：无 
***************************************************************************
*/
void bsp_InitWwdgNVIC(FunctionalState NewState)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=NewState;
	NVIC_Init(&NVIC_InitStructure);

}

/*
***************************************************************************
*          函 数 名：WWDG_IRQHandler
*          功能说明：窗口看门狗调用中断服务程序
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/

void WWDG_IRQHandler(void)
{
	/*特别注意，下面两句的执行顺序不能反了，反了就出问题了*/
	/*清除EWI标志*/
	if(WWDG_GetFlagStatus()!=RESET)
	{
		/*清除复位标志*/
		WWDG_ClearFlag();
		
		/*更新窗口看门狗计数器*/
		WWDG_SetCounter(127);
	}
}
