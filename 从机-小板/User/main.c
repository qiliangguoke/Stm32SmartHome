#include "bsp.h" 


/* 底层硬件驱动 */

/*
STM32 每个系列都会有唯一的一个芯片序列号（96 位 bit）：
STM32F10X 的地址是 0x1FFFF7E8 
STM32F20X 的地址是 0x1FFF7A10
STM32F30X 的地址是 0x1FFFF7AC
STM32F40X 的地址是 0x1FFF7A10
STM32L1XX 的地址是 0x1FF80050
*/

/* SN 起始地址 */
#define STM32F10X_SN_ADDR 0x1FFFF7E8


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME "WSNEP_V01-105_UART 实验（CO2）"
#define EXAMPLE_DATE "2019-08-02"
#define DEMO_VER "1.0"


/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);
static void PrintfHelp(void);


void control_device(void);  //控制设备函数
void data_static(void);		//传感器状态打印函数
void Command_control(void);///接收指令
void change_receive(void);//判断是否发送
uint16_t co2_value=0;		//二氧化碳显示
uint16_t pm25_value = 0;	//PM2.5碳显示
float lux = 0;				//显示光照强度
uint8_t temp_max=27;      //设置温度上限
uint8_t temp_min=25;      //设置温度下限
uint8_t pachuang=0;         //爬窗标志位
uint8_t hongwai=0;         //红外标志位
uint8_t flag_ceive='0';
//bsp_FanOn(1); 测试开启风扇
//bsp_RelayOn(1);继电器翻转状态
//bsp_LedOn(1); LED灯状态
//pm25_value = PM25_READ();//读取PM2.5的值
//co2_value = CO2_READ();//读取二氧化碳的值
//lux = (float)GetLuxValue() / 100 ;//获取实际光照值
//Tem_Value温度
//RH_Value 湿度

/*
*********************************************************************************************************
* 函 数 名: main
* 功能说明: c 程序入口
* 形 参：无
* 返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/


int main(void)
{
	/*
	ST 固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f10x.c 文件，主要功能是
	配置 CPU 系统的时钟，内部 Flash 访问时序，配置 FSMC 用于外部 SRAM
	*/
	
	bsp_Init(); /* 硬件初始化 */
	PrintfLogo(); /* 打印例程信息到串口 1 */
	PrintfHelp(); /* 打印操作提示 */
	
	
	bsp_InitUart2(9600);
	bsp_InitUart3(9600);
	
	bsp_InitRelay(); /* 初始化继电器 */
	bsp_InitFan(); /* 初始化风扇 */	
	bsp_InitOpt3001();	//初始化光照传感器
	bsp_InitIoSensor(); //IO 型传感器模块初始化
	
	
	
	
	bsp_StartAutoTimer(0, 300); /* 定时器 0 周期 300 毫秒 */
	
	
	/* 进入主程序循环体 */
	while(1) 
	{
		bsp_Idle(); /* CPU 空闲时执行的函数，在 bsp.c */
		//bsp_FanOn(1);  //测试开启风扇

		
		if(ReceiveState==1)
		{
			Command_control();  //接收控制指令
			change_receive();  //判断是否发送
			Uart0_STA_Clr();
		}

		
				
//		if(USART_RX_BUF[0]>0X03)     //测试使用
//		{

//		}
		
		control_device();      //控制设备函数		
		if (bsp_CheckTimer(0)) /* 定时到 */
		{	
			data_static();   //传输温湿度数据
			
		}
		

		
	}
}
void change_receive(void)  //选择接收函数
{
	if(USART_RX_BUF[0]=='2')
	{
		flag_ceive='1';
	}
	else if(USART_RX_BUF[0]=='1'||USART_RX_BUF[0]=='3'||USART_RX_BUF[0]=='4'||USART_RX_BUF[0]=='5'||USART_RX_BUF[0]=='6'||USART_RX_BUF[0]=='7')
	{
		flag_ceive='0';
	}
}
void Command_control(void)
{
	if(USART_RX_BUF[0]=='J')    //继电器控制协议
	{
		
		switch(USART_RX_BUF[1])
		{
			case '1':
			{
				bsp_RelayOn(1);
				bsp_RelayOn(2);
			}break;
			case '2':
			{
				bsp_RelayOff(1);
				bsp_RelayOff(2);
			}break;
			case '3':
			{
				bsp_RelayOn(3);
				bsp_RelayOn(4);		
			}break;
			case '4':
			{
				bsp_RelayOff(3);
				bsp_RelayOff(4);
			}break;
		}
	}
	if(USART_RX_BUF[0]=='F')   //风扇控制协议
	{
		
		switch(USART_RX_BUF[1])
		{
			case '1':
			{
				bsp_FanOn(1);
			}break;
			case '2':
			{
				bsp_FanOff(1);
			}break;
			case '3':
			{
				bsp_FanOn(2);		
			}break;
			case '4':
			{
				bsp_FanOff(2);
			}break;
		}
	}
	if(USART_RX_BUF[0]=='L')   //风扇控制协议
	{
		
		switch(USART_RX_BUF[1])
		{
			case '1':
			{
				bsp_LedOn(2);
				
			}break;
			case '2':
			{
				bsp_LedOff(2);
				bsp_BeepOff();
			}break;
			case '3':
			{
				bsp_BeepOn();
			}break;
			case '4':
			{
				bsp_BeepOff();
			}break;
		}
	}
	if(USART_RX_BUF[0]=='B')   //风扇控制协议
	{
		
		switch(USART_RX_BUF[1])
		{
			case '1':
			{
				bsp_BeepOn();
			}break;
			case '2':
			{
				bsp_BeepOff();
			}break;
		}
	}
		
}
void control_device(void)   //控制设备函数
{
	/*************根据温度高低开启风扇***********/
	if(temp_max<Tem_Value)    //开启风扇
	{
		bsp_FanOn(1);  
	}else if(temp_min>Tem_Value) //关闭风扇
	{
		bsp_FanOff(1);
	}
	
	/*人体感应检测 蜂鸣器开启*/
	{
		if(PIR_OUT_flag==1)
		{
			bsp_LedOn(1);
			bsp_BeepOn();
			bsp_StartAutoTimer(1,2000); /* 定时器 1 周期 1000 毫秒 */
			hongwai=12;			
		}
		if(bsp_CheckTimer(1))
		{
			bsp_BeepOff();
			hongwai=0;	
			bsp_StopTimer(1);
		}
		

	}
	
	if
	( VIBRATE_OUT_flag == 1 )
	{
		pachuang=12;
		bsp_StartAutoTimer(2,2000); /* 定时器 1 周期 1000 毫秒 */
	}
	if(bsp_CheckTimer(2))
	{
		bsp_StopTimer(2);
		pachuang=0;
	}
	
	

}

/********************************************************************************/
void data_static(void)     //传感器数据函数
{
	
	pm25_value = PM25_READ();//读取PM2.5的值
	co2_value = CO2_READ();//读取二氧化碳的值
	lux = (float)GetLuxValue() / 100 ;//获取实际光照值

	/****************温湿度显示**********************/
	{
		if( SHT3X_GetValue() )
		{
			bsp_InitI2C();
			bsp_InitSht3x();
		}
		delay_ms(50);
		if(flag_ceive=='1')
		{	
			printf("002,%.1f,%.1f,%d,%d,%.2f,%d,%d\r\n", Tem_Value,RH_Value,co2_value,pm25_value,lux,pachuang,hongwai);  //显示温湿度
		}
	}
	
	
		
	
	if
	( VIBRATE_OUT_flag == 1 )
	{
		VIBRATE_OUT_flag = 0; //清零
		//printf ( "VIBRATE 传感器动作\r\n" ); //动作时 IO 为低电平，复位时 IO 为高电平
				/************************爬窗设置标志位**********************/
	}
	if ( PIR_OUT_flag == 1 )
	{
		PIR_OUT_flag = 0; //清零
		//printf ( "PIR 传感器动作\r\n" ); //动作时 IO 为高电平，复位时 IO 为低电平
		 //红外标志位
	}
	
	
	
	

//暂时不用	
//	if ( HY_OUT_flag == 1 )
//	{
//		HY_OUT_flag = 0; //清零
//		printf ( "HY 传感器动作\r\n" ); //动作时 IO 为低电平，复位时 IO 为高电平
//	}
////			if ( HE_OUT_flag == 1 )
////			{
////				HE_OUT_flag = 0; //清零
////				printf ( "HE 传感器动作\r\n" ); //动作时 IO 为低电平，复位时 IO 为高电平
////			}
//	if ( KR_OUT_flag == 1 )
//	{
//		KR_OUT_flag = 0; //清零
//		printf ( "KR 传感器动作\r\n" ); //动作时 IO 为低电平，复位时 IO 为高电平
//	}
}
/*
*********************************************************************************************************
* 函 数 名: PrintfHelp
* 功能说明: 打印操作提示
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
static void PrintfHelp(void)
{
	printf("操作提示:\r\n");
	printf("1.本程序中，每 1 秒 DS0 红灯翻转一次\r\n");
	printf("2.每 1 秒读取并显示一次 CO2 数据，注意 CO2 上电预热 3 分钟后数据才准确\r\n");
	printf("\r\n\r\n");
}
/*
*********************************************************************************************************
* 函 数 名: PrintfLogo
* 功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开串口调试助手观察结果
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
/* 检测 CPU ID */
{
	uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;
	CPU_Sn0 = *(__IO uint32_t*)(STM32F10X_SN_ADDR);
	CPU_Sn1 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 4);
	CPU_Sn2 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 8);
	printf("\r\nCPU : STM32F103ZET6, LQFP144, UID = %08X %08X %08X\n\r"
	, CPU_Sn2, CPU_Sn1, CPU_Sn0);
}
	printf("\r\n");
	printf("*************************************************************\r\n");
	printf("* 例程名称 : %s\r\n", EXAMPLE_NAME);/* 打印例程名称 */
	printf("* 例程版本 : %s\r\n", DEMO_VER); /* 打印例程版本 */
	printf("* 发布日期 : %s\r\n", EXAMPLE_DATE); /* 打印例程日期 */
	/* 打印 ST 固件库版本，这 3 个定义宏在 stm32f10x.h 文件中 */
	printf("* 固 件 库 版 本 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", 
	__STM32F10X_STDPERIPH_VERSION_MAIN,
	__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \r\n"); /* 打印一行空格 */
	printf("*************************************************************\r\n");
}
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
