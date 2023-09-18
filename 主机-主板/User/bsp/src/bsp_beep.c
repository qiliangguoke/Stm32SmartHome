#include"bsp.h"

/*
***************************************************************************
*          函 数 名:bsp_InitBeep
*          功能说明：配置有源蜂鸣器相关的GPIO，该函数被bsp_Init()调用。
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_InitBeep(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_BEEP,ENABLE);
	
	/*配置蜂鸣器为推挽输出模式
	  由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，不会驱动蜂鸣器鸣叫
	*/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStructure);
		
}
/*
***************************************************************************
*          函 数 名：bsp_BeepOn
*          功能说明：鸣叫蜂鸣器
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_BeepOn(void)
{
	//BEEP_GPIO_PORT->BSRR = BEEP_GPIO_PIN;
	BEEP=1;
}

/*
***************************************************************************
*          函 数 名：bsp_BeepOff
*          功能说明：静音蜂鸣器
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_BeepOff()
{
	//BEEP_GPIO_PORT_BRR = BEEP_GPIO_PIN;
	BEEP=0;
}
/*
***************************************************************************
*          函 数 名：bsp_LedToggle
*          功能说明：翻转蜂鸣器
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_BeepToggle(void)
{
	BEEP_GPIO_PORT->ODR^=BEEP_GPIO_PIN;
}

/*
***************************************************************************
*          函 数 名：bsp_IsBeepOn
*          功能说明：判断蜂鸣器是否已经鸣叫
*          形   参：无
*          返 回 值：1表示鸣叫，0表示静音 
***************************************************************************
*/
uint8_t bsp_IsBeepOn()
{
	if((BEEP_GPIO_PORT->ODR&BEEP_GPIO_PIN)==BEEP_GPIO_PIN)
	{
		return 1;
	}
	return 0;
}
