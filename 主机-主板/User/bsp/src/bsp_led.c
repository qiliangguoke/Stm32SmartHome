#include "bsp.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/*
*********************************************************************************************************
*	函 数 名:
*	功能说明: 
*	形    参：
*	返 回 值: 
********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no){
	
	
	if(_no==1){
		
		LED0=1;
	}
	else if(_no==2)
	{
		LED1=1;
	}
}



/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed(void)
*	功能说明: 初始化
*	形    参：无
*	返 回 值: 无
********************************************************************************************************
*/
void bsp_InitLed(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;//实例化结构体
	
	RCC_APB2PeriphClockCmd(RCC_ALL_LED,ENABLE);
	
	
	bsp_LedToggle(1);
	bsp_LedToggle(2);

	//关闭led1
	GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);  // 参数：大端口 小端口号
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	
	GPIO_InitStructure.GPIO_Pin=LED0_GPIO_PIN;
	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
}


/*
*********************************************************************************************************
*	函 数 名:bsp_LEdOn
*	功能说明: 开灯
*	形    参：uint8_t
*	返 回 值: 无
********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no){
	
	if(_no==1)
	{	
		LED0=0;
	}
	else if(_no==2)
	{
		LED1=0;
	}
}
/*
*********************************************************************************************************
*	函 数 名:bsp_LedToggle
*	功能说明: 反转
*	形    参：uint8_t
*	返 回 值: 无
********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no){
	
	if(_no==1)
	{
		
		LED0_GPIO_PORT->ODR ^=LED0_GPIO_PIN;
		
	}
	else if(_no==2)
	{
		LED1_GPIO_PORT->ODR ^=LED1_GPIO_PIN;
	}
}
/*
*********************************************************************************************************
*	函 数 名:bsp_IsLedOn
*	功能说明: 查询输出状态
*	形    参：uint8_t
*	返 回 值: uint8_t
********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if(_no ==1)
	{
		if((LED0_GPIO_PORT->ODR & LED1_GPIO_PIN)==LED1_GPIO_PIN)
		{
			return 1;
		}
		return 0;
		
	}
	else if(_no ==2)
	{
		if((LED1_GPIO_PORT->ODR&LED1_GPIO_PIN)==LED0_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}
	return 0;
}
