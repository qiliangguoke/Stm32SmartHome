#include "bsp.h"

static void bsp_InitAdc(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t ADC_SampleTime);
static uint16_t Get_Adc(ADC_TypeDef* ADCx);
static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times);

/*
***************************************************************************
*          函 数 名：bsp_InitAdc
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static void bsp_InitAdc(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	if(ADCx == ADC1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);
		
		ADC_DeInit(ADC1);
	}
	else if(ADCx == ADC2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);
		
		ADC_DeInit(ADC2);
	}
	else if(ADCx == ADC3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);
		
		ADC_DeInit(ADC3);	
	}
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;      //ADC工作模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADCx,&ADC_InitStructure);
	
	
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime);
	ADC_Cmd(ADCx,ENABLE);
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
	
		
}

/*
***************************************************************************
*          函 数 名：Get_Adc
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static uint16_t Get_Adc(ADC_TypeDef* ADCx)
{
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));
	
	return ADC_GetConversionValue(ADCx);
}
/*
***************************************************************************
*          函 数 名：Get_Adc_Average
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times)
{
	u32 temp_val=0;
	
	u8 t;
	
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ADCx);
	}
	
	return temp_val/times;
}

/*
***************************************************************************
*          函 数 名：bsp_InitAdcPotentiometer
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_InitAdcPotentiometer(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RES_POT_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RES_POT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(RES_POT_GPIO_PORT,&GPIO_InitStructure);

	bsp_InitAdc(RES_POT_ADC_X,RES_POT_ADC_CHANNEL,RES_POT_ADC_SAMPLETIME);
}
/*
***************************************************************************
*          函 数 名：Get_Ade_Potentiometer
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
uint16_t Get_Ade_Potentiometer(void)
{
	uint16_t adcx;
	adcx = Get_Adc_Average(RES_POT_ADC_X,SAMP_COUNT);
	
	return adcx;
}

/*
***************************************************************************
*          函 数 名：bsp_InitAdcTemp
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void bsp_InitAdcTemp(void)
{
	/**/
	bsp_InitAdc(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55Cycles5);
	
	/**/
	ADC_TempSensorVrefintCmd(ENABLE);
}
/*
***************************************************************************
*          函 数 名：Get_Temprate
*          功能说明：
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
	double temperate;
	
	adcx=Get_Adc_Average(ADC1,SAMP_COUNT);
	temperate=(float)adcx*(3.3/4096);
	temperate=(1.43-temperate)/0.0043+25;
	result=temperate*=100;
	
	return result;
}
