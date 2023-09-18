#include "bsp.h"
/*
*********************************************************************************************************
* 函 数 名: bsp_InitFan
* 功能说明: 配置风扇 Fan 相关的 GPIO。
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitFan(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 打开 GPIO 时钟 */
	
	RCC_APB2PeriphClockCmd(RCC_ALL_FAN, ENABLE);
	
	/*
	配置所有的 FAN GPIO 为推挽输出模式
	由于将 GPIO 设置为输出时，GPIO 输出寄存器的值缺省是 0，因此会驱动 LED 点亮.
	这是我不希望的，因此在改变 GPIO 为输出前，先关闭 RELAY
	*/
	
	bsp_FanOff(1);
	bsp_FanOff(2);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = FAN1_GPIO_PIN; /* FAN1 端口 */
	GPIO_Init(FAN1_GPIO_PORT, &GPIO_InitStructure); /* 初始化 RELAY1 */
	GPIO_InitStructure.GPIO_Pin = FAN2_GPIO_PIN; /* FAN2 端口 */
	GPIO_Init(FAN2_GPIO_PORT, &GPIO_InitStructure); /* 初始化 FAN2 */
}
/*
*********************************************************************************************************
* 函 数 名: bsp_FanOn
* 功能说明: 开启指定的风扇。
* 形 参: _no : 风扇序号，范围 1 - 2
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_FanOn(uint8_t _no)
{
	if (_no == 1)
	{
		FAN1 = 1;
	}
	else if (_no == 2)
	{
		FAN2 = 1;
	}
}
/*
*********************************************************************************************************
* 函 数 名: bsp_FanOff
* 功能说明: 关闭指定的风扇。
* 形 参: _no : 风扇序号，范围 1 - 2
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_FanOff(uint8_t _no)
{
	if (_no == 1)
	{
		FAN1 = 0;
	}
	else if (_no == 2)
	{
		FAN2 = 0;
	}
}
/*
*********************************************************************************************************
* 函 数 名: bsp_FanToggle
* 功能说明: 翻转指定的风扇。
* 形 参: _no : 风扇序号，范围 1 - 2
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_FanToggle(uint8_t _no)
{
	if (_no == 1)
	{
		FAN1_GPIO_PORT->ODR ^= FAN1_GPIO_PIN;
	}
	else if (_no == 2)
	{
		FAN2_GPIO_PORT->ODR ^= FAN2_GPIO_PIN;
	}
}
/*
*********************************************************************************************************
* 函 数 名: bsp_IsFanOn
* 功能说明: 判断风扇是否已经闭合。
* 形 参: _no : 风扇序号，范围 1 - 2
* 返 回 值: 1 表示已经开启，0 表示关闭
*********************************************************************************************************
*/
uint8_t bsp_IsFanOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((FAN1_GPIO_PORT->ODR & FAN1_GPIO_PIN) == FAN1_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((FAN2_GPIO_PORT->ODR & FAN2_GPIO_PIN) == FAN2_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}
