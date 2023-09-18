/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : bsp_key.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*
*********************************************************************************************************
*/
#ifndef __bsp_key_H
#define __bsp_key_H

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
///////////////////////////////////////
/* 宏定义区域 */
#define RCC_ALL_KEY  (WKUP_GPIO_CLK|KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK)

#define WKUP_GPIO_PIN       GPIO_Pin_0       		 //WKUP引脚号
#define WKUP_PIN_ID         0                		 //WKUP引脚序号
#define WKUP_GPIO_PORT      GPIOA            		 //WKUP端口号
#define WKUP_GPIO_CLK       RCC_APB2Periph_GPIOA     //WKUP时钟
//#define WKUP_FUN_OUT        PAout                  //WKUP输出端口配置函数
#define WKUP_FUN_IN         PAin                     //WKUP输入端口配置函数
#define WKUP_GPIO_MODE      GPIO_Mode_IPD            //WKUP端口输入模式
#define WKUP_ACTIVE_LEVEL   1                        //WKUP按键有效电平为高电平

//////
#define KEY0_GPIO_PIN       GPIO_Pin_4                //KEY0引脚号
#define KEY0_PIN_ID         4                         //KEY0引脚序号
#define KEY0_GPIO_PORT      GPIOE                     //KEY0端口号
#define KEY0_GPIO_CLK       RCC_APB2Periph_GPIOE      //KEY0时钟
#define KEY0_FUN_OUT        PEout                     //KEY0输出端口号配置函数
#define KEY0_FUN_IN         PEin                      //KEY0输入端口号配置函数
#define KEY0_GPIO_MODE		GPIO_Mode_IPU             //KEY0端口输入模式
#define KEY0_ACTIVE_LEVEL   0 	                       //KEY0按键有效电平为低电平
//////
#define KEY1_GPIO_PIN       GPIO_Pin_3                //KEY1引脚号
#define KEY1_PIN_ID         3                         //KEY1引脚序号
#define KEY1_GPIO_PORT      GPIOE                     //KEY1端口号
#define KEY1_GPIO_CLK       RCC_APB2Periph_GPIOE      //KEY1时钟
#define KEY1_FUN_OUT        PEout                     //KEY1输出端口号配置函数
#define KEY1_FUN_IN         PEin                      //KEY1输入端口号配置函数
#define KEY1_GPIO_MODE		GPIO_Mode_IPU             //KEY1端口输入模式
#define KEY1_ACTIVE_LEVEL   0 	                       //KEY1按键有效电平为低电平
//////
#define KEY2_GPIO_PIN       GPIO_Pin_2                 //KEY2引脚号
#define KEY2_PIN_ID         2                          //KEY2引脚序号
#define KEY2_GPIO_PORT      GPIOE                      //KEY2端口号
#define KEY2_GPIO_CLK       RCC_APB2Periph_GPIOE       //KEY2时钟
#define KEY2_FUN_OUT        PEout                      //KEY2输出端口号配置函数
#define KEY2_FUN_IN         PEin                       //KEY2输入端口号配置函数
#define KEY2_GPIO_MODE		GPIO_Mode_IPU              //KEY2端口输入模式
#define KEY2_ACTIVE_LEVEL   0 	                       //KEY2按键有效电平为低电平
///////////////////////////
//IO操作函数
#define WKUP               	WKUP_FUN_IN(WKUP_PIN_ID)	//WKUP//读取按键1（WK_UP）
#define KEY0                KEY0_FUN_IN(KEY0_PIN_ID)    //KEY0//读取按键2
#define KEY1                KEY1_FUN_IN(KEY1_PIN_ID)    //KEY1//读取按键3
#define KEY2                KEY2_FUN_IN(KEY2_PIN_ID)    //KEY2//读取按键4
//////////////////////////
#define KEY_COUNT			6							/*按键个数，4个独立键+2个组合键*/

/*根据应用程序的功能重命名按键宏*/
#define WKUP_DOWN			KEY_1_DOWN
#define WKUP_UP				KEY_1_UP
#define WKUP_LONG			KEY_1_LONG

#define KEY0_DOWN			KEY_2_DOWN
#define KEY0_UP				KEY_2_UP
#define KEY0_LONG			KEY_2_LONG

#define KEY1_DOWN			KEY_3_DOWN
#define KEY1_UP				KEY_3_UP
#define KEY1_LONG			KEY_3_LONG

#define KEY2_DOWN			KEY_4_DOWN
#define KEY2_UP				KEY_4_UP
#define KEY2_LONG			KEY_4_LONG

#define SYS_DOWN_WKUP_KEY0			KEY_5_DOWN	/*WKUP KEY0 组合键*/
#define SYS_UP_WKUP_KEY0			KEY_5_UP
#define SYS_LONG_WKUP_KEY0			KEY_5_LONG

#define SYS_DOWN_KEY0_KEY1			KEY_6_DOWN /*KEY0 KEY1 组合键*/
#define SYS_UP_KEY0_KEY1			KEY_6_UP
#define SYS_LONG_KEY0_KEY1			KEY_6_LONG

///////////////////////////////////////
/* 外部变量申明区 */
/*按键ID，主要用于bsp_KeyState()函数的入口参数*/
typedef enum
{
	KID_K1=0,
	KID_K2,
	KID_K3,
	KID_K4,
}KEY_ID_E;
/*
	按键滤波时间50ms,单位10ms.
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME		5
#define KEY_LONG_TIME		100				/*单位10ms, 持续1秒，认为长按事件*/
/*每个按键对应1个全局的结构体变量*/
typedef struct
{
	/*下面是一个函数指针，指向判断按键是否按下的函数*/
	uint8_t(*IsKeyDownFunc)(void);	/*按键按下的判断函数，1表示按下*/	
	uint8_t		Count;	/*滤波器计数器*/	
	uint16_t	LongCount;	/*长按计数器*/	
	uint16_t	LongTime;	/*按键按下持续时间，0表示不检测长按*/	
	uint8_t		State;	/*按键当前状态（按下还是弹起）*/
	uint8_t		RepeatSpeed;	/*连续按键周期*/
	uint8_t		RepeatCount;	/*连续按键计数器*/
}KEY_T;
/*
	定义键值代码，必须按如下次序定时每个键的按下、弹起和长按事件

	推荐enum，不用#define,原因
	（1）便于新增键值，方便调整顺序，使代码看起来舒服点
	（2）编译器可帮我们避免键值重复。
*/

typedef enum
{
	KEY_NONE =0,	/*0表示按键事件*/
	KEY_1_DOWN,		/*1键按下*/
	KEY_1_UP,		/*1键弹起*/	
	KEY_1_LONG,		/*1键长按*/	
	
	KEY_2_DOWN,		/*2键按下*/
	KEY_2_UP,		/*2键弹起*/
	KEY_2_LONG,		/*2键长按*/
	
	KEY_3_DOWN,		/*3键按下*/
	KEY_3_UP,		/*3键弹起*/
	KEY_3_LONG,		/*3键长按*/
	
	KEY_4_DOWN,		/*4键按下*/
	KEY_4_UP,		/*4键弹起*/
	KEY_4_LONG,		/*4键长按*/
	
	/*组合键*/
	KEY_5_DOWN,		/*5键按下*/
	KEY_5_UP,		/*5键弹起*/
	KEY_5_LONG,		/*5键长按*/
	
	KEY_6_DOWN,		/*6键按下*/
	KEY_6_UP,		/*6键弹起*/
	KEY_6_LONG,		/*6键长按*/
	
}KEY_ENUM;

/*按键FIFO用到变量*/
#define KEY_FIFO_SIZE    10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/*键值缓冲区*/
	uint8_t Read;					/*缓冲区读指针1*/
	uint8_t Write;					/*缓冲区写指针*/
	uint8_t Read2;					/*缓冲区读指针2*/
}KEY_FIFO_T;
///////////////////////////////////////
/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID,uint16_t _LongTime,uint8_t _RepeatSpeed);
void bsp_ClearKey(void);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/


