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
#define EXAMPLE_NAME "WSNEP_V01-012_TFT LCD 显示实验（FSMC）"
#define EXAMPLE_DATE "2023-7-07"
#define DEMO_VER "1.0"

/* 定义 LCD 显示相关公共信息 */
#define DEV_NAME "WSNEP_V01"
#define LCD_EXAMPLE_NAME "zhangyaozong"

/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);
static void PrintfHelp(void);




uint16_t co2_value=0;
float lux=0; //作为显示用
uint16_t PM25_value=0;

uint8_t temp_max=28;  //温度上限
uint8_t temp_min=20;  //温度下限
uint8_t  VIA_fla=0;   //震动检测标志位
uint8_t  FIR_fla=0;   //震动检测标志位
uint8_t  ceive='1';     //接收检测标志位

void data_static(void);     //传感器数据函数
void Command_control(void);  //接收控制指令
void Nt_DisPlay(void);  //打印边框
void Show_L(void);		//显示数据
void fengshan_temp(void);	//实时控制
void Key_Control(void);  //温度函数
void usart2_cmd_receive(void);//串口2收串口1发
void usart1_cmd_receive(void);//串口1收 串口2发
void flag_chuli(void);//标志位接收
void check_receive(void);//选择接收
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
//	uint8_t x=1;
//	uint8_t lcd_id[12]; /* 存放 LCD ID 字符串 */
	
	/*
	ST 固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f10x.c 文件，主要功能是
	配置 CPU 系统的时钟，内部 Flash 访问时序，配置 FSMC 用于外部 SRAM
	*/
	bsp_Init(); /* 硬件初始化 */
	PrintfLogo(); /* 打印例程信息到串口 1 */
	PrintfHelp(); /* 打印操作提示 */
	
	bsp_InitUart2(57600);	//初始化串口2
	
	bsp_InitUart4(9600);/*注意，串口 4 初始化放在 LCD 之前*/
	bsp_InitUART5(9600);/*注意，串口 5 初始化放在 LCD 之前*/
	bsp_InitIoSensor(); //IO 型传感器模块初始化
	bsp_InitRelay();		//继电器初始化
	bsp_InitFan(); 			/* 初始化风扇 */
	bsp_InitI2C();			//IIC初始化
	bsp_InitSht3x();		//温湿度传感器初始化
	
	TFTLCD_Init(); /* TFTLCD 初始化*/
	
	POINT_COLOR=BLUE; //字体颜色
	Nt_DisPlay();
	
	bsp_StartAutoTimer(0, 500); /* 定时器 0 周期 500 毫秒 */	
	while(bsp_InitOpt3001())//检测不到 OPT3001 ( OPT3001 初始化，即 IIC 初始化和器件 ID 读取 )
	{
		delay_ms(1000);
		LED1=!LED1;//DS0 闪烁
	}
	/* 进入主程序循环体 */
	while(1) 
	{
		
		
		
		bsp_Idle(); /* CPU 空闲时执行的函数，在 bsp.c */
		
        //usart1_cmd();//继电器控制指令
		if(ReceiveState==1)   //判断是否接收到
		{	
			Command_control(); //控制函数
			flag_chuli();//标志位
			USART2_Send_Data(USART_RX_BUF,RxCounter);  //把串口1接收到的指令全部发送出去
			Uart0_STA_Clr();   //接收之后清零
		}
		Show_L(); //LCD显示函数
		fengshan_temp();  //控制函数
		Key_Control();  //按键控制
//		usart1_cmd_receive(); //串口1发送函数			
		
		
		/************************温湿度传感器***************************/
		if (bsp_CheckTimer(0)) /* 定时到 */
		{
			bsp_LedToggle(1);
			//check_receive();//选择发送
			usart2_cmd_receive();  //将接收到串口2的信息 打印到串口1
			data_static();	  //显示传感器数据		
		}
			
	}
}

void flag_chuli(void)//对标志位赋值
{
	switch(USART_RX_BUF[0])
	{
		case '1': 
		{
			ceive='1';
		}break;
		case '2':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		case '3':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		case '4':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		case '5':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		case '6':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		case '7':
		{
			USART2_Send_Data(USART_RX_BUF,RxCounter); 
			ceive='0';
		}break;
		
	}
}
/*****************************/
void check_receive(void)
{
	
     if(ceive=='2')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='1')  //01   001 002 003
		{
			usart2_cmd_receive();
		}
	}
	else if(ceive=='3')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='2')
		{
			usart2_cmd_receive();
		}
	}
	else if(ceive=='4')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='3')
		{
			usart2_cmd_receive();
		}
	}
	else if(ceive=='5')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='4')
		{
			usart2_cmd_receive();
		}
	}
	else if(ceive=='6')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='5')
		{
			usart2_cmd_receive();
		}
	}
	else if(ceive=='7')
	{
		if(USART2_RX_BUF[1]=='0'&&USART2_RX_BUF[2]=='6')
		{
			usart2_cmd_receive();
		}
	}
}
/*
*********************************************************************************************************
*	函 数 名: usart1_cmd_receive
*	功能说明: 串口1接收，串口2发送
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void usart1_cmd_receive(void)
{
	if(ReceiveState)
	{
		USART2_Send_Data(USART_RX_BUF,RxCounter);
		Uart0_STA_Clr();
	}
}

/*
*********************************************************************************************************
*	函 数 名: usart2_cmd_receive
*	功能说明: 串口2接收，串口1发送
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void usart2_cmd_receive(void)
{
	if(ReceiveState2)
	{
		USART1_Send_Data(USART2_RX_BUF,USART2_RX_CNT);
		Uart2_STA_Clr();
	}
}
void Show_L(void)
{
		char wendu_shu[10];
		char shidu_shu[10];
		char cStr[7];
	
		sprintf( wendu_shu, "%.2f",((float)(Tem_Value))); //温度值
		sprintf( shidu_shu, "%.2f",((float)(RH_Value))); //湿度值
		sprintf( cStr, "%.2f",((float)(lux))); //光照度值
	
		LCD_ShowString(204,52,200,16,16,(uint8_t*)wendu_shu); 
		LCD_ShowString(204,82,200,16,16,(uint8_t*)shidu_shu); 
		LCD_ShowString(204,112,200,16,16,(uint8_t*)cStr); 	
		LCD_ShowNum(188,142,co2_value,5,16);   //显示温度
		LCD_ShowNum(181,172,PM25_value,5,16);	  //显示湿度
	
		LCD_ShowNum(105,262,temp_max,2,16);   //显示温度top
		LCD_ShowNum(245,262,temp_min,2,16);   //显示温度
		//LCD_ShowString(170,262,200,16,16,(uint8_t*)"temp_min:");	 //显示温度字母	
		

		if(USART_RX_BUF[0]=='J'&&(USART_RX_BUF[1]=='1'||USART_RX_BUF[1]=='3'))
		{
			Show_Chinese32(205,202,26,16,GREEN,WHITE,0);    //显示开
		}
		else Show_Chinese32(205,202,27,16,RED,WHITE,0);    //显示关
		
		if(temp_max<Tem_Value||(USART_RX_BUF[0]=='F'&&(USART_RX_BUF[1]=='1'||USART_RX_BUF[1]=='3')))
		{
			Show_Chinese32(205,232,26,16,GREEN,WHITE,0);    //显示开
		}
		else Show_Chinese32(205,232,27,16,RED,WHITE,0);    //显示关
		
		if(FIR_fla==12)			
		{	Show_Chinese32(100,325,28,20,RED,WHITE,0);    //显示有
			Show_Chinese32(130,325,29,20,RED,WHITE,0);    //显示有
			Show_Chinese32(160,325,30,20,RED,WHITE,0);    //显示有
			Show_Chinese32(190,325,31,20,RED,WHITE,0);    //显示有
		}
		else
			LCD_Fill(100,320,250,345,WHITE);
		
		if(VIA_fla==12)
		{	
			Show_Chinese32(100,360,28,20,RED,WHITE,0);    //显示有
			Show_Chinese32(130,360,29,20,RED,WHITE,0);    //显示有
			Show_Chinese32(160,360,32,20,RED,WHITE,0);    //显示有
			Show_Chinese32(190,360,33,20,RED,WHITE,0);    //显示有
		}
		else
			LCD_Fill(100,350,250,400,WHITE);
}
void fengshan_temp(void)  //风扇设限开关
{
	if(temp_max<Tem_Value) bsp_FanOn(1);
	else bsp_FanOff(1);
	
	
	//人体检测蜂鸣器响
	if(PIR_OUT_flag==1)
	{
		FIR_fla=12;
		bsp_BeepOn();
		bsp_StartTimer(1, 2000); /* 定时器 0 周期 500 毫秒 */
	}
	
	if(bsp_CheckTimer(1))
	{
		FIR_fla=0;
		bsp_StopTimer(1);
		bsp_BeepOff();		
	}
	//*****************
	
	//震动传感器
	 if(VIBRATE_OUT_flag==1)
	 {
		VIA_fla=12;
		bsp_StartTimer(2, 2000); /* 定时器 0 周期 500 毫秒 */
	 }
	 if(bsp_CheckTimer(2))
	 {
		 VIA_fla=0;
		bsp_StopTimer(1);
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
				bsp_RelayOn(1);	//打开继电器1
				bsp_RelayOn(2);//打开继电器2
			}break;
			case '2':
			{
				bsp_RelayOff(1);	//关闭继电器1
				bsp_RelayOff(2);	//关闭继电器2
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
	if(USART_RX_BUF[0]=='L')   //led控制协议
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
void data_static(void)     //传感器数据函数
{
	
	
	co2_value = CO2_READ();//读取二氧化碳的值
	lux = (float)GetLuxValue() / 100 ;//获取实际光照值
	PM25_value = PM25_READ();//读取pm2.5值


	/****************温湿度显示**********************/
	{
		if( SHT3X_GetValue() )
		{
			bsp_InitI2C();
			bsp_InitSht3x();
		}
		//printf("%.1f,%.1f,%d,%d,%.2f,%d,%d\r\n", Tem_Value,RH_Value,co2_value,pm25_value,lux,pachuang,hongwai);  //显示温湿度
		//printf("%.1f,%.1f,%d,%.1f,%d,%d,%d",Tem_Value, RH_Value,co2_value,lux,PM25_value,FIR_fla,VIA_fla);  //温度，湿度，二氧化碳，光照强度

	}
			
		if ( VIBRATE_OUT_flag == 1 )   //震动传感器
		{
			VIBRATE_OUT_flag = 0; //清零
			//printf ( "VIBRATE 传感器动作\r\n" ); //动作时 IO 为低电平，复位时 IO 为高电平
		}
		
		
		if ( PIR_OUT_flag == 1 )	//人体检测传感器
		{
			PIR_OUT_flag = 0; //清零
			//printf ( "PIR 传感器动作\r\n" ); //动作时 IO 为高电平，复位时 IO 为低电平
		}
		
		//判断标志位选择是否发送
		if(ceive=='1')
		{
			printf("01,%.1f,%.1f,%d,%.1f,%d,%d,%d",Tem_Value, RH_Value,co2_value,lux,PM25_value,FIR_fla,VIA_fla); 
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
void Nt_DisPlay(void)
{
	uint8_t lcd_id[12]; /* 存放 LCD ID 字符串 */
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将 LCD ID 打印到 lcd_id 数组。
	
	Show_Chinese32(30,13,2,24,GRAY,WHITE,0);    //显示智
	Show_Chinese32(55,13,3,24,GRAY,WHITE,0);    //显示能
	Show_Chinese32(80,13,4,24,GRAY,WHITE,0);    //显示家
	Show_Chinese32(105,13,5,24,GRAY,WHITE,0);    //显示居		
	Show_Chinese32(55,52,0,16,GRAY,WHITE,0);    //显示温
	Show_Chinese32(81,52,1,16,GRAY,WHITE,0);    //显示度
	Show_Chinese32(170,10,6,12,GRAY,WHITE,0);    //显示2
	Show_Chinese32(180,10,7,12,GRAY,WHITE,0);    //显示组
	Show_Chinese32(55,82,8,16,GRAY,WHITE,0);    //显示湿
	Show_Chinese32(81,82,9,16,GRAY,WHITE,0);    //显示度
	Show_Chinese32(55,112,10,16,GRAY,WHITE,0);    //显示光
	Show_Chinese32(81,112,11,16,GRAY,WHITE,0);    //显示照
	Show_Chinese32(44,142,12,16,GRAY,WHITE,0);    //显示C
	Show_Chinese32(69,142,13,16,GRAY,WHITE,0);    //显示O
	Show_Chinese32(95,142,14,16,GRAY,WHITE,0);    //显示2
	Show_Chinese32(39,172,15,16,GRAY,WHITE,0);    //显示P
	Show_Chinese32(55,172,16,16,GRAY,WHITE,0);    //显示M
	Show_Chinese32(71,172,17,16,GRAY,WHITE,0);    //显示2
	Show_Chinese32(87,172,18,16,GRAY,WHITE,0);    //显示.
	Show_Chinese32(103,172,19,16,GRAY,WHITE,0);    //显示5
	Show_Chinese32(39,202,20,16,GRAY,WHITE,0);    //显示电
	Show_Chinese32(55,202,21,16,GRAY,WHITE,0);    //显示电
	Show_Chinese32(71,202,22,16,GRAY,WHITE,0);    //显示电
	Show_Chinese32(87,202,23,16,GRAY,WHITE,0);    //显示电
	Show_Chinese32(39,232,24,16,GRAY,WHITE,0);    //显示风
	Show_Chinese32(55,232,25,16,GRAY,WHITE,0);    //显示扇
	Show_Chinese32(71,232,26,16,GRAY,WHITE,0);    //显示开
	Show_Chinese32(87,232,27,16,GRAY,WHITE,0);    //显示关
	
	LCD_ShowString(170,25,200,12,12,(uint8_t*)EXAMPLE_DATE); //日期显示
	//LCD_ShowString(170,10,200,16,16,(uint8_t*)LCD_EXAMPLE_NAME);	//显示姓名
	LCD_ShowString(30,440,200,16,16,(uint8_t*)DEMO_VER);
	LCD_ShowString(30,455,200,16,16,lcd_id); /* 显示 LCD ID */ 
	
	LCD_ShowString(30,262,200,16,16,(uint8_t*)"temp_max:");  //显示温度字母
	LCD_ShowString(170,262,200,16,16,(uint8_t*)"temp_min:");	 //显示温度字母	
	
	LCD_DrawLine(5,5,5,475);//竖线1
	LCD_DrawLine(150,45,150,285);//竖线2
	LCD_DrawLine(315,5,315,475);//竖线3
	LCD_DrawLine(5,5,315,5);//横线1
	LCD_DrawLine(5,45,315,45);//横线2
	LCD_DrawLine(5,75,315,75);//横线3
	LCD_DrawLine(5,105,315,105);//横线4
	LCD_DrawLine(5,135,315,135);//横线5
	LCD_DrawLine(5,165,315,165);//横线7
	LCD_DrawLine(5,195,315,195);//横线8
	LCD_DrawLine(5,225,315,225);//横线9
	LCD_DrawLine(5,255,315,255);//横线10
	LCD_DrawLine(5,285,315,285);//横线11
	LCD_DrawLine(5,475,315,475);//横线11
	
	
	
	
	
	//		LCD_ShowNum(180,55,Tem_Value,3,24);   //显示温度
//		LCD_ShowNum(180,95,RH_Value,3,24);	  //显示湿度
//		LCD_ShowNum(180,135,lux,3,24);	  //显示光照强度
//		LCD_ShowNum(180,175,co2_value,3,24);	  //二氧化碳浓度
}

void Key_Control(void)
{
	uint8_t ucKeyCode;
	/* 处理按键事件 */
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode > 0)
		{
			/* 有键按下 */
			switch(ucKeyCode)
			{
				case WKUP_DOWN://WKUP
				{
					temp_max++;					
				}break;
				case KEY0_DOWN://KEY0
				{
					temp_max--;

				}break;
			}
		}
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
