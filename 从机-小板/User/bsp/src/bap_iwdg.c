#include "bsp.h"

/*
***************************************************************************
*          函 数 名：bsp_InitIwdg
*          功能说明：独立看门狗时间配置函数
*          形   参：uIWDGRlr:0 ----0x0FFF
					  _uIWDGPrer:     IWDG_Prescaler_4
											IWDG_Prescaler_8
											IWDG_Prescaler_16
											IWDG_Prescaler_32
											IWDG_Prescaler_64
											IWDG_Prescaler_128
											IWDG_Prescaler_256
*          返 回 值：无 
***************************************************************************
*/

void bsp_InitIwdg(uint8_t _uIWDGPrer,uint32_t _u1IWDGR1r)
{
	/*检查系统是否从独立看门狗复位中回复*/
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)
	{
		/*清楚符文标志*/
		RCC_ClearFlag();
		printf("**独立看门狗复位***************\r\n");
		
	}
	else 
	{
		/* 标志没有设置*/
		printf("****不是独立看门狗复位，可能是上电复位或者手动复位*******\r\n");
	}
	
	/*使能LSI*/
	RCC_LSICmd(ENABLE);
	
	/*等待直到LSI就绪*/
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET)
	{}
		
	/*写入0x5555表示允许访问IWDG_PR和IWDG_RLR寄存器*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*LSI预分频*/
	IWDG_SetPrescaler(_uIWDGPrer);
	
	/*设置重装载值*/
	IWDG_SetReload(_u1IWDGR1r);
		
	/*重载IWDG计数*/
	IWDG_ReloadCounter();
		
	/*使能IWDG(LSI oscillator 由硬件使能)*/
	IWDG_Enable();
}

/*
***************************************************************************
*          函 数 名：IWDG_Feed
*          功能说明：喂狗函数
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
