/*
*********************************************************************************************************
*
*	模块名称 : 独立按键驱动模块
*	文件名称 : bsp_key.c
*	说    明 : 
*	修改记录 :扫描独立按键，具有软件滤波机制，具有FIFO，可以检测如下事件：
				（1）按键按下
				（2）按键弹起
				（3）长按键
				（4）长按时自动连发
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/
#include "bsp.h"
/*
	该程序适用于WSNEP_V01开发板
	如果用于其他硬件，请修改GPIO定义和IsKeyDown1-IsKeyDown6函数和static void bsp_InitKeyHard(void)函数
	如果用户的按键个数小于4个，你可以将多余的按键全部定义为和第1个按键一样，并不影响程序功能
	#define KEY_COUNT 6 这个在bsp_key.h文件中定义
*/
static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;	/*按键FIFO变量，结构体*/

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);
/*
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参：无
*	返 回 值:返回值1 表示按下，0表示未按下
*********************************************************************************************************
*/
/*德致伦WSNEP_V01开发板*/
/*为了区分6个事件：WKUP单独按下，KEY0单独按下，KEY1单独按下，KEY2单独按下，WKUP和KEY0同时按下，WKUP和KEY1同时按下*/
static uint8_t IsKeyDown1(void)
{
	if((WKUP==WKUP_ACTIVE_LEVEL)&&(KEY0!=KEY0_ACTIVE_LEVEL)&&(KEY1!=KEY1_ACTIVE_LEVEL)
		&&(KEY2!=KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown2(void)
{
	if((WKUP!=WKUP_ACTIVE_LEVEL)&&(KEY0==KEY0_ACTIVE_LEVEL)&&(KEY1!=KEY1_ACTIVE_LEVEL)
		&&(KEY2!=KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown3(void)
{
	if((WKUP!=WKUP_ACTIVE_LEVEL)&&(KEY0!=KEY0_ACTIVE_LEVEL)&&(KEY1==KEY1_ACTIVE_LEVEL)
		&&(KEY2!=KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown4(void)
{
	if((WKUP!=WKUP_ACTIVE_LEVEL)&&(KEY0!=KEY0_ACTIVE_LEVEL)&&(KEY1!=KEY1_ACTIVE_LEVEL)
		&&(KEY2==KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown5(void)/**/
{
	if((WKUP==WKUP_ACTIVE_LEVEL)&&(KEY0==KEY0_ACTIVE_LEVEL)&&(KEY1!=KEY1_ACTIVE_LEVEL)
		&&(KEY2!=KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown6(void)/**/
{
	if((WKUP!=WKUP_ACTIVE_LEVEL)&&(KEY0==KEY0_ACTIVE_LEVEL)&&(KEY1==KEY1_ACTIVE_LEVEL)
		&&(KEY2!=KEY2_ACTIVE_LEVEL))
		return 1;
	else
		return 0;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键，该函数被bsp_Init()调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{

	bsp_InitKeyVar();/**/
	bsp_InitKeyHard();/**/
	
}
/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键
*	形    参：KeyCode:按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;
	if(++s_tKey.Write >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;
	if(s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}else
	{
		ret = s_tKey.Buf[s_tKey.Read];
		if(++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey2
*	功能说明: 从按键FIFO缓冲区读取一个键值。独立的读指针
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;
	if(s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}else
	{
		ret = s_tKey.Buf[s_tKey.Read2];
		if(++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参：_ucKey:按键ID，从0开始
*	返 回 值: 1表示按下，0表示未按下
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键的状态
*	形    参：_ucKey:按键ID，从0开始
			  _LOngTime:长按事件时间
			_RepeatSpeed:连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID,uint16_t _LongTime,uint8_t _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime=_LongTime;/*长按时间0表示不检测长按键事件*/
	s_tBtn[_ucKeyID].RepeatSpeed=_RepeatSpeed;/*按键连发的速度，0表示不支持连发*/
	s_tBtn[_ucKeyID].RepeatCount = 0;/*连发计数器*/
}
/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read=s_tKey.Write;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyHard
*	功能说明: 配置按键对应的GPIO
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 第一步：打开GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	/*关闭 jtag,使能SWD，可以用SWD模式调试*/
	
	/*第二步：配置所有按键GPIO*/
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;   //KEY0端口
	GPIO_InitStructure.GPIO_Mode = KEY0_GPIO_MODE; //KEY0端口模式
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure); //初始化KEY0
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;   //KEY1端口
	GPIO_InitStructure.GPIO_Mode = KEY1_GPIO_MODE; //KEY1端口模式
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure); //初始化KEY1
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;   //KEY2端口
	GPIO_InitStructure.GPIO_Mode = KEY2_GPIO_MODE; //KEY2端口模式
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure); //初始化KEY2
	
	GPIO_InitStructure.GPIO_Pin = WKUP_GPIO_PIN;   //WKUP端口
	GPIO_InitStructure.GPIO_Mode = WKUP_GPIO_MODE; //WKUP端口模式
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure); //初始化WKUP
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;
	/*对按键FIFO读写指针清零*/
	s_tKey.Read=0;
	s_tKey.Write=0;
	s_tKey.Read2=0;
	/*给每个按键结构体成员变量赋一组缺省值*/
	for(i=0;i<KEY_COUNT;i++)
	{
		s_tBtn[i].LongTime=KEY_LONG_TIME;/*长按时间0表示不检测长按键事件*/
		s_tBtn[i].Count=KEY_FILTER_TIME/2;/*计数器设置为滤波时间的一半*/
		s_tBtn[i].State=0;/*按键缺省状态，0为未按下*/
		//s_tBin[i].KeyCodeDown=3*i+1;	/*按键按下的键值代码*/
		//s_tBin[i].KeyCodeUp=3*i+2;	/*按键弹起的键值代码*/
		//s_tBin[i].KeyCodeLong=3*i+3;	/*按键被持续按下的键值代码*/
		s_tBtn[i].RepeatSpeed=0;/*按键连发的速度，0表示不支持连发*/
		s_tBtn[i].RepeatCount=0;/*连发计数器*/
	}
	/*如果需要单独更改某个按键的参数，可以在此单独赋值*/
	/*比如，我们希望按键1秒按下超过1秒后，自动重发相同键值*/
	//s_tBtn[KID_K1].LongTime=100;
	//s_tBtn[KID_K1].RespeatSpeed=5;/*每隔50ms自动发送键值*/
	
	/*判断按键按下的函数*/
	s_tBtn[0].IsKeyDownFunc=IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc=IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc=IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc=IsKeyDown4;
		
	/*组合键*/
	s_tBtn[4].IsKeyDownFunc=IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc=IsKeyDown6;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	/*
		如果没有初始化按键函数，则报错
		if(s_tBtn[i].IsKeyDownFunc==0)
		{
			printf("Fault:DEtectButton(),s_tBtn[i].IsKeyDownFunc undefine");
		}
	*/
	pBtn=&s_tBtn[i];
	if(pBtn ->IsKeyDownFunc())
	{
		if(pBtn->Count<KEY_FILTER_TIME)
		{
			pBtn->Count=KEY_FILTER_TIME;
		}
		else if(pBtn->Count<2*KEY_FILTER_TIME)
		{
			pBtn ->Count++;
		}
		else
		{
			if(pBtn->State==0)
			{
				pBtn->State=1;
				/*发送按钮按下的消息*/
				bsp_PutKey((uint8_t)(3*i+1));
			}
			if(pBtn->LongTime>0)
			{
				if(pBtn->LongCount < pBtn->LongTime)
				{
					/*发送按钮持续按下的消息*/
					if(++pBtn->LongCount == pBtn->LongTime)
					{
						/*键值放入按键FIFO*/
						bsp_PutKey((uint8_t)(3*i+3));
					}
				}
				else
				{
					if(pBtn->RepeatSpeed>0)
					{
						if(++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount=0;
							/*长按键后，每隔10ms发送1个按键*/
							bsp_PutKey((uint8_t)(3*i+1));
						}
					}
				}
			}
		}
	}	
			else{
				if(pBtn->Count>KEY_FILTER_TIME)
				{
					pBtn->Count=KEY_FILTER_TIME;
				}else if(pBtn->Count!=0)
				{
					pBtn->Count--;
				}else
				{
					if(pBtn ->State==1)
				{
					pBtn->State=0;
					/*发送按钮弹起的消息*/
					bsp_PutKey((uint8_t)(3*i+2));
				}
			}
				pBtn->LongCount=0;
				pBtn->RepeatCount=0;
		}
	}

/*
***************************************************************************
*          函 数 名:bsp_KeyScan
*          功能说明：扫描所有按键。非阻塞，被systick中断周期性的调用
*          形   参：无
*          返 回 值：无
***************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;
	for(i=0;i<KEY_COUNT;i++)
	{
		bsp_DetectKey(i);
	}
}
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
