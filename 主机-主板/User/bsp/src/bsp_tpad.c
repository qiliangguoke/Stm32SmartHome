#include "bsp.h"

/**/
static void TPAD_TIM_GPIO_Config(void);
static void TPAD_TIM_Config(void);

static void bsp_TpadTimInit(void);
static void bsp_TpadReset(void);

static uint16_t TPAD_Get_Val(void);
static uint16_t TPAD_Get_MaxVal(uint8_t n);


/**/
vu16 tpad_default_val=0;       //空载的时候（没有手按下），计数器需要时间


/*
***************************************************************************
*          函 数 名：TPAD_Get_Val
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static uint16_t TPAD_Get_Val()
{
	bsp_TpadReset();     /**/
	
	/**/
	while(TIM_GetFlagStatus(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx)==RESET)
	{
		if(TIM_GetCounter(TIMx_CAP_TIM)>TPAD_ARR_MAX_VAL-500)
		{
			return TIM_GetCounter(TIMx_CAP_TIM);
		}
	};
	return TIMx_CAP_TIM_GetCapturex_FUN(TIMx_CAP_TIM);
}

/*
***************************************************************************
*          函 数 名：
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(n--)
	{
		temp = TPAD_Get_Val();//得到一次值
		if(temp>res)
		{
			res=temp;
		}
		
	};
	return res;
}

/*
***************************************************************************
*          函 数 名：
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
uint8_t bsp_TpadScan(void)
{
	static uint8_t keyen=0;  //0,
	uint8_t res=0;
	uint8_t sample=3;
	uint16_t rval;
	
	rval = TPAD_Get_MaxVal(sample);
	
	/**/
	if(rval>(tpad_default_val+TPAD_GATE_VAL))
	{
		if(keyen ==0)
		{
			res=1;      //keyen==0有效
		}
		//printf("r:%d\r\n",rval);
		keyen=3;
	}
	if(keyen)
	{
		keyen--;
	}
	return res;
	
}

/*
***************************************************************************
*          函 数 名：
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
}
/*
***************************************************************************
*          函 数 名：TPAD_TIM_Config
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static void TPAD_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	/**/
	TIM_TimeBaseStructure.TIM_Period = TPAD_ARR_MAX_VAL;
	TIM_TimeBaseStructure.TIM_Prescaler=TIMx_CAP_TIM_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIMx_CAP_TIM,&TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIMx_CAP_TIM_CHANNEL_x;  /**/
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; /**/
	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x03;
	
	TIM_ICInit(TIMx_CAP_TIM,&TIM_ICInitStructure);
	
	/**/
	TIM_Cmd(TIMx_CAP_TIM,ENABLE);
}

/*
***************************************************************************
*          函 数 名：bsp_TpadTimInit
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static void bsp_TpadTimInit(void)
{
	TPAD_TIM_GPIO_Config();
	TPAD_TIM_Config();
}

/*
***************************************************************************
*          函 数 名：bsp_TpadReset
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static void bsp_TpadReset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	
	//
	GPIO_InitStructure.GPIO_Pin=TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(TIMx_CAP_TIM_CH_PORT,TIMx_CAP_TIM_CH_PIN);
	
	delay_ms(5);  //放电等待
	
	TIM_SetCounter(TIMx_CAP_TIM,0);
	TIM_ClearITPendingBit(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx|TIMx_CAP_TIM_IT_UPDATE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
}

/*
***************************************************************************
*          函 数 名：
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
uint8_t bsp_TpadInit(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	bsp_TpadTimInit();
	
	for(i=0;i<10;i++)
	{
		buf[i]=TPAD_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp=0;
	for(i=2;i<8;i++)
	{
		temp+=buf[i];
	}
	
	tpad_default_val=temp/6;
	
	printf("tpad_default:%d\r\n",tpad_default_val);
	printf("\r\n\r\n");
	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
	{
		return 1;
	}
	return 0;
}
