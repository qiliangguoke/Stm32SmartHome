#include "bsp.h"
#include "font.h" 
	   
//////////////////////////////////////////////////////////////////////////////////	 
//3.5寸TFT液晶驱动 
//支持驱动IC型号:NT35310	    	
//********************************************************************************

//LCD的画笔颜色和背景色	   
uint16_t POINT_COLOR = 0x9999;	//画笔颜色
uint16_t BACK_COLOR = 0xffff;	//背景色 
  
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//仅在本文件内使用的函数
static void TFTLCD_GPIO_Config ( void );
static void TFTLCD_FSMC_Config ( void );
static void NT35310_REG_Config ( void );


/*
*********************************************************************************************************
*	函 数 名: LCD_WR_REG
*	功能说明: 写寄存器函数
*	形    参: regval:寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WR_REG(uint16_t regval)
{	
	* ( __IO uint16_t * ) ( FSMC_Addr_NT35310_CMD ) = regval;//写入要写的寄存器序号
}
/*
*********************************************************************************************************
*	函 数 名: LCD_WR_DATA
*	功能说明: 写LCD数据
*	形    参: data:要写入的值
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WR_DATA(uint16_t data)
{
	* ( __IO uint16_t * ) ( FSMC_Addr_NT35310_DATA ) = data;
}
/*
*********************************************************************************************************
*	函 数 名: LCD_RD_DATA
*	功能说明: 读CD数据
*	形    参: 无
*	返 回 值: 返回值:读到的值
*********************************************************************************************************
*/
uint16_t LCD_RD_DATA(void)
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_NT35310_DATA ) );	
}					   
/*
*********************************************************************************************************
*	函 数 名: LCD_WriteReg
*	功能说明: 写寄存器
*	形    参: LCD_Reg:寄存器地址
*				LCD_RegValue:要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);		//写入要写的寄存器序号
	LCD_WR_DATA(LCD_RegValue);	//写入数据
}
/*
*********************************************************************************************************
*	函 数 名: LCD_ReadReg
*	功能说明: 读寄存器
*	形    参: LCD_Reg:寄存器地址
*	返 回 值: 返回值:读到的数据
*********************************************************************************************************
*/
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
/*
*********************************************************************************************************
*	函 数 名: LCD_WriteRAM_Prepare
*	功能说明: 开始写GRAM
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_WriteRAM
*	功能说明: LCD写GRAM
*	形    参: RGB_Code:颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WriteRAM(uint16_t RGB_Code)
{
	LCD_WR_DATA(RGB_Code);	//写十六位GRAM
}
/*
*********************************************************************************************************
*	函 数 名: opt_delay
*	功能说明: 当mdk -O1时间优化时需要设置
*	形    参: 延时i
*	返 回 值: 无
*********************************************************************************************************
*/
void opt_delay(uint8_t i)
{
	while(i--);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_ReadPoint
*	功能说明: 读取个某点的颜色值
*	形    参: x,y:坐标
*	返 回 值: 返回值:此点的颜色
*********************************************************************************************************
*/
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)
		return 0;	//超过了范围,直接返回
	
	LCD_SetCursor(x,y);
	LCD_WR_REG(0X2E);//5310发送读GRAM指令
    
 	r=LCD_RD_DATA();								//dummy Read	   
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						//实际坐标颜色
	
 	///NT35310要分2次读出
 	{
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//对于NT35310,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	} 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//NT35310需要公式转换一下
	
}			 
/*
*********************************************************************************************************
*	函 数 名: LCD_DisplayOn
*	功能说明: LCD开启显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DisplayOff
*	功能说明: LCD关闭显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//关闭显示
}   
/*
*********************************************************************************************************
*	函 数 名: LCD_SetCursor
*	功能说明: 设置光标位置
*	形    参: 	Xpos:横坐标
*				Ypos:纵坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
} 		 
/*
*********************************************************************************************************
*	函 数 名: LCD_Scan_Dir
*	功能说明: 设置LCD的自动扫描方向（一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.）
*	形    参: dir:0~7,代表8个方向(具体定义见lcd.h)
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t temp;  
	
	if( lcddev.dir==1 )
	{
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	
	switch(dir)//方向转换
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://从上到下,从左到右
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
		   
	LCD_WriteReg(0x36,regval);

	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}else  
	{
		if(lcddev.width>lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}  
		
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		
  	
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DrawPoint
*	功能说明: 画点
*	形    参: x,y:坐标	POINT_COLOR:此点的颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR);
	
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Fast_DrawPoint
*	功能说明: 快速画点
*	形    参: x,y:坐标	color:颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	
	LCD_WriteReg(lcddev.wramcmd,color);
	
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Display_Dir
*	功能说明: 设置LCD显示方向
*	形    参: dir:0,竖屏；1,横屏
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		
		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 

		lcddev.width=320;
		lcddev.height=480;
		
	}else 				//横屏
	{	  				
		lcddev.dir=1;	//横屏

		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		
		lcddev.width=480;
		lcddev.height=320; 			
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Set_Window
*	功能说明: 设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).（窗体大小:width*height. ）
*	形    参: 	sx,sy:窗口起始坐标(左上角)
*				width,height:窗口宽度和高度,必须大于0!!
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(sx>>8); 
	LCD_WR_DATA(sx&0XFF);	 
	LCD_WR_DATA(twidth>>8); 
	LCD_WR_DATA(twidth&0XFF);  
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(sy>>8); 
	LCD_WR_DATA(sy&0XFF); 
	LCD_WR_DATA(theight>>8); 
	LCD_WR_DATA(theight&0XFF); 
}
/*
*********************************************************************************************************
*	函 数 名: TFTLCD_GPIO_Config
*	功能说明: 初始化IO引脚，时钟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TFTLCD_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE); //使能 FSMC 时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_LCD,ENABLE); //使能端口时钟
	
	/* 配置 LCD 背光控制管脚 为推挽输出 */ 
	GPIO_InitStructure.GPIO_Pin = LCD_LED_GPIO_PIN; //推挽输出 背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_LED_GPIO_PORT, &GPIO_InitStructure);
	
	/* 配置 FSMC 相对应的控制线 为复用推挽输出
	* FSMC_NEx :LCD-CS
	* FSMC_Ax :LCD-RS
	* FSMC_NWE :LCD-WR
	* FSMC_NOE :LCD-RD
	*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LCD_CS_GPIO_PIN;
	GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_GPIO_PIN;
	GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_CS_GPIO_PIN;
	GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_WR_GPIO_PIN;
	GPIO_Init(LCD_WR_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RD_GPIO_PIN;
	GPIO_Init(LCD_RD_GPIO_PORT, &GPIO_InitStructure);
	
	/* 配置 FSMC 相对应的数据线,FSMC-D0~D15 为复用推挽输出*/
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LCD_D0_GPIO_PIN;
	GPIO_Init ( LCD_D0_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D1_GPIO_PIN;
	GPIO_Init ( LCD_D1_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D2_GPIO_PIN;
	GPIO_Init ( LCD_D2_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D3_GPIO_PIN;
	GPIO_Init ( LCD_D3_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D4_GPIO_PIN;
	GPIO_Init ( LCD_D4_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D5_GPIO_PIN;
	GPIO_Init ( LCD_D5_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D6_GPIO_PIN;
	GPIO_Init ( LCD_D6_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D7_GPIO_PIN;
	GPIO_Init ( LCD_D7_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D8_GPIO_PIN;
	GPIO_Init ( LCD_D8_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D9_GPIO_PIN;
	GPIO_Init ( LCD_D9_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D10_GPIO_PIN;
	GPIO_Init ( LCD_D10_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D11_GPIO_PIN;
	GPIO_Init ( LCD_D11_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D12_GPIO_PIN;
	GPIO_Init ( LCD_D12_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D13_GPIO_PIN;
	GPIO_Init ( LCD_D13_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D14_GPIO_PIN;
	GPIO_Init ( LCD_D14_GPIO_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = LCD_D15_GPIO_PIN;
	GPIO_Init ( LCD_D15_GPIO_PORT, & GPIO_InitStructure );
	/* 结束引脚和时钟配置 ------------------------------------*/
	
}

/*
*********************************************************************************************************
*	函 数 名: TFTLCD_FSMC_Config
*	功能说明: 初始化FSMC配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TFTLCD_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef writeTiming;
	
	readWriteTiming.FSMC_AddressSetupTime = 0x01; //地址建立时间（ADDSET）为 2 个 HCLK 1/36M=27ns
	readWriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间（ADDHLD）模式 A 未用到
	readWriteTiming.FSMC_DataSetupTime = 0x0f; // 数据保存时间为 16 个 HCLK,因为液晶驱动 IC 的读数据的时候，速度不能太快
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	
	writeTiming.FSMC_AddressSetupTime = 0x00; //地址建立时间（ADDSET）为 1 个 HCLK 
	writeTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间（A
	writeTiming.FSMC_DataSetupTime = 0x03; //数据保存时间为 4 个 HCLK
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAMx;// 这 里 我 们 使 用 NE4 ，也 就 对应BTCR[6],[7]。
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType=FSMC_MemoryType_SRAM;//
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为 16bit 
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable; //存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming; //写时序
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); //初始化 FSMC 配置
	
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAMx, ENABLE); // 使能 BANK1 
	
}

/*
*********************************************************************************************************
*	函 数 名: NT35310_REG_Config
*	功能说明: 初始化NT35310寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void NT35310_REG_Config ( void )
{
	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0xFE);

	LCD_WR_REG(0xEE);
	LCD_WR_DATA(0xDE);
	LCD_WR_DATA(0x21);

	LCD_WR_REG(0xF1);
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xDF);
	LCD_WR_DATA(0x10);

	//VCOMvoltage//
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x8F);	  //5f

	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0xE2);LCD_WR_DATA(0xE2);LCD_WR_DATA(0xE2);
	
	LCD_WR_REG(0xBF);
	LCD_WR_DATA(0xAA);

	LCD_WR_REG(0xB0);
	LCD_WR_DATA(0x0D);LCD_WR_DATA(0x00);LCD_WR_DATA(0x0D);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x11);LCD_WR_DATA(0x00);LCD_WR_DATA(0x19);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);LCD_WR_DATA(0x00);LCD_WR_DATA(0x2D);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3D);LCD_WR_DATA(0x00);LCD_WR_DATA(0x5D);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x80);LCD_WR_DATA(0x00);LCD_WR_DATA(0x8B);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x96);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x02);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB3);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x8B);LCD_WR_DATA(0x00);LCD_WR_DATA(0x96);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA1);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB5);
	LCD_WR_DATA(0x02);LCD_WR_DATA(0x00);LCD_WR_DATA(0x03);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x3F);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5E);LCD_WR_DATA(0x00);LCD_WR_DATA(0x64);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8C);LCD_WR_DATA(0x00);LCD_WR_DATA(0xAC);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);LCD_WR_DATA(0x00);LCD_WR_DATA(0x70);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x90);LCD_WR_DATA(0x00);LCD_WR_DATA(0xEB);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB8);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xBA);
	LCD_WR_DATA(0x24);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x20);LCD_WR_DATA(0x00);LCD_WR_DATA(0x54);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xFF);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x0A);LCD_WR_DATA(0x00);LCD_WR_DATA(0x04);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x3C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x3A);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x39);LCD_WR_DATA(0x00);LCD_WR_DATA(0x37);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x36);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);LCD_WR_DATA(0x00);LCD_WR_DATA(0x2F);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x29);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);LCD_WR_DATA(0x00);LCD_WR_DATA(0x24);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);LCD_WR_DATA(0x00);LCD_WR_DATA(0x23);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x36);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);LCD_WR_DATA(0x00);LCD_WR_DATA(0x2F);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x29);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);LCD_WR_DATA(0x00);LCD_WR_DATA(0x24);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);LCD_WR_DATA(0x00);LCD_WR_DATA(0x23);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x62);LCD_WR_DATA(0x00);LCD_WR_DATA(0x05);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF0);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);LCD_WR_DATA(0x00);LCD_WR_DATA(0xA4);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);LCD_WR_DATA(0x00);LCD_WR_DATA(0x50);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x17);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x95);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE6);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x32);LCD_WR_DATA(0x00);LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x65);LCD_WR_DATA(0x00);LCD_WR_DATA(0x76);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x20);LCD_WR_DATA(0x00);LCD_WR_DATA(0x17);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC7);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC8);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC9);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x16);LCD_WR_DATA(0x00);LCD_WR_DATA(0x1C);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);LCD_WR_DATA(0x00);LCD_WR_DATA(0x36);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x46);LCD_WR_DATA(0x00);LCD_WR_DATA(0x52);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);LCD_WR_DATA(0x00);LCD_WR_DATA(0x7A);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB9);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);LCD_WR_DATA(0x00);LCD_WR_DATA(0xCA);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD9);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x16);LCD_WR_DATA(0x00);LCD_WR_DATA(0x1C);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x22);LCD_WR_DATA(0x00);LCD_WR_DATA(0x36);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);LCD_WR_DATA(0x00);LCD_WR_DATA(0x52);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);LCD_WR_DATA(0x00);LCD_WR_DATA(0x7A);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB9);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);LCD_WR_DATA(0x00);LCD_WR_DATA(0xCA);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD8);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE2);
	LCD_WR_DATA(0x05);LCD_WR_DATA(0x00);LCD_WR_DATA(0x0B);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1B);LCD_WR_DATA(0x00);LCD_WR_DATA(0x34);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);LCD_WR_DATA(0x4F);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x61);LCD_WR_DATA(0x00);LCD_WR_DATA(0x79);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);LCD_WR_DATA(0x97);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB7);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xC7);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD6);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE3);
	LCD_WR_DATA(0x05);LCD_WR_DATA(0x00);LCD_WR_DATA(0xA);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x33);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);LCD_WR_DATA(0x50);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x62);LCD_WR_DATA(0x00);LCD_WR_DATA(0x78);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);LCD_WR_DATA(0x97);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB7);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xC7);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD5);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE4);
	LCD_WR_DATA(0x01);LCD_WR_DATA(0x00);LCD_WR_DATA(0x01);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);LCD_WR_DATA(0x00);LCD_WR_DATA(0x2A);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x4B);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);LCD_WR_DATA(0x00);LCD_WR_DATA(0x74);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);LCD_WR_DATA(0x00);LCD_WR_DATA(0x93);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB3);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);LCD_WR_DATA(0x00);LCD_WR_DATA(0xC4);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD3);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE5);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);LCD_WR_DATA(0x00);LCD_WR_DATA(0x29);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);LCD_WR_DATA(0x00);LCD_WR_DATA(0x4B);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);LCD_WR_DATA(0x00);LCD_WR_DATA(0x74);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);LCD_WR_DATA(0x00);LCD_WR_DATA(0x93);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);LCD_WR_DATA(0x00);LCD_WR_DATA(0xB3);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);LCD_WR_DATA(0x00);LCD_WR_DATA(0xC4);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);LCD_WR_DATA(0x00);LCD_WR_DATA(0xD3);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);LCD_WR_DATA(0x00);LCD_WR_DATA(0xF3);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE6);
	LCD_WR_DATA(0x11);LCD_WR_DATA(0x00);LCD_WR_DATA(0x34);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);LCD_WR_DATA(0x00);LCD_WR_DATA(0x76);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);LCD_WR_DATA(0x00);LCD_WR_DATA(0x66);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);LCD_WR_DATA(0x00);LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);LCD_WR_DATA(0x45);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x43);LCD_WR_DATA(0x00);LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE7);
	LCD_WR_DATA(0x32);LCD_WR_DATA(0x00);LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);LCD_WR_DATA(0x00);LCD_WR_DATA(0x66);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x67);LCD_WR_DATA(0x00);LCD_WR_DATA(0x67);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);LCD_WR_DATA(0x00);LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);LCD_WR_DATA(0x00);LCD_WR_DATA(0x23); LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);LCD_WR_DATA(0x00);LCD_WR_DATA(0x45);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);LCD_WR_DATA(0x00);LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);LCD_WR_DATA(0x00);LCD_WR_DATA(0x66);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);LCD_WR_DATA(0x00);LCD_WR_DATA(0xAA);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);LCD_WR_DATA(0x00);LCD_WR_DATA(0x99);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);LCD_WR_DATA(0x00);LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);LCD_WR_DATA(0x00);LCD_WR_DATA(0x55);LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0xAA);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);

	LCD_WR_REG(0x00);
	LCD_WR_DATA(0xAA);

	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x00);LCD_WR_DATA(0x50);LCD_WR_DATA(0x00);LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF9);
	LCD_WR_DATA(0x06);LCD_WR_DATA(0x10);LCD_WR_DATA(0x29);LCD_WR_DATA(0x00);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);	//66

	LCD_WR_REG(0x11);
	delay_ms(100);
	LCD_WR_REG(0x29);
	LCD_WR_REG(0x35);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x51);
	LCD_WR_DATA(0xFF);
	LCD_WR_REG(0x53);
	LCD_WR_DATA(0x2C);
	LCD_WR_REG(0x55);
	LCD_WR_DATA(0x82);

}
/*
*********************************************************************************************************
*	函 数 名: TFTLCD_Init
*	功能说明: 初始化TFT LCD
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TFTLCD_Init ( void )
{
	TFTLCD_GPIO_Config();
	TFTLCD_FSMC_Config();
	
	delay_ms(50); 		// delay 50 ms 
	
	//尝试看看是不是 NT35510
	LCD_WR_REG(0XD4); 
	lcddev.id=LCD_RD_DATA(); //dummy read 
	lcddev.id=LCD_RD_DATA(); //读回 0x01
	lcddev.id=LCD_RD_DATA(); //读回 0x53
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA(); //这里读回 0x10
	
	BSP_Printf(" LCD ID:%x\r\n",lcddev.id); //打印 LCD ID 
	
	NT35310_REG_Config ();
	
	LCD_Display_Dir(0); //实验默认设置为竖屏
	
	LCD_LED=1; //点亮背光
	LCD_Clear(WHITE);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Clear
*	功能说明: 清屏函数
*	形    参: color:要清屏的填充色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	LCD_SetCursor(0x00,0x0000);			//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA(color);
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Fill
*	功能说明: 在指定区域内填充单个颜色
*	形    参: 	(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
*				color:要填充的颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM	  
		for(j=0;j<xlen;j++)
		{
			LCD_WR_DATA(color);		//显示颜色 
		}	    
	}
	 
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Color_Fill
*	功能说明: 在指定区域内填充指定颜色块
*	形    参: 	(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
*				color:要填充的颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA(color[i*width+j]);	//写入数据
		}
		
	}		  
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLine
*	功能说明: 画线
*	形    参: 	x1,y1:起点坐标 
*				x2,y2:终点坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)
	{
		incx=1; //设置单步方向 
	}
	else if(delta_x==0)
	{
		incx=0;//垂直线
	}		
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	} 
	if(delta_y>0)
	{
		incy=1;
	}		
	else if(delta_y==0)
	{
		incy=0;//水平线
	}		
	else
	{
		incy=-1;
		delta_y=-delta_y;
	} 
	if( delta_x>delta_y)
	{
		distance=delta_x; //选取基本增量坐标轴 
	}
	else 
	{
		distance=delta_y;
	}		
	for(t=0;t<=distance+1;t++)//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DrawRectangle
*	功能说明: 画矩形
*	形    参: (x1,y1),(x2,y2):矩形的对角坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Draw_Circle
*	功能说明: 在指定位置画一个指定大小的圆
*	形    参: 	(x,y):中心点
*				r:半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
/*
*********************************************************************************************************
*	函 数 名: LCD_ShowChar
*	功能说明: 在指定位置显示一个字符
*	形    参: 	x,y:起始坐标
*				num:要显示的字符:" "--->"~"
*				size:字体大小 12/16/24
*				mode:叠加方式(1)还是非叠加方式(0)
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)
		{
			temp=asc2_1206[num][t]; 	 	//调用1206字体
		}
		else if(size==16)
		{
			temp=asc2_1608[num][t];	//调用1608字体
		}
		else if(size==24)
		{
			temp=asc2_2412[num][t];	//调用2412字体
		}
		else
		{ 
			return;								//没有的字库
		}
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
			{
				LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			}
			else if(mode==0)
			{
				LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			}
			temp<<=1;
			y++;
			if(y>=lcddev.height)
			{
				return;		//超区域了
			}
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)
				{
					return;	//超区域了
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}   
/*
*********************************************************************************************************
*	函 数 名: LCD_Pow
*	功能说明: m^n函数
*	形    参: m 底数 n 幂数
*	返 回 值: 返回值:m^n次方.
*********************************************************************************************************
*/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)
	{
		result*=m;    
	}
	return result;
}	
/*
*********************************************************************************************************
*	函 数 名: LCD_ShowNum
*	功能说明: 显示数字,高位为0,则不显示
*	形    参: 	x,y:起始坐标
*				num:数值(0~4294967295)
*				len :数字的位数
*				size:字体大小 12/16/24
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_ShowxNum
*	功能说明: 显示数字,高位为0,还是显示
*	形    参: 	x,y:起始坐标
*				num:数值(0~999999999);
*				len:长度(即要显示的位数)
*				size:字体大小 12/16/24
*				mode:	[7]:0,不填充;1,填充0.
*						[6:1]:保留
*						[0]:0,非叠加显示;1,叠加显示.
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)
				{
					LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				}
				else 
				{
					LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
				}
 				continue;
			}else 
			{
				enshow=1;
			}				
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
}		
/*
*********************************************************************************************************
*	函 数 名: LCD_ShowString
*	功能说明: 显示字符串
*	形    参: 	x,y:起始坐标
*				width,height:区域大小 
*				size:字体大小 12/16/24
*				*p:字符串起始地址
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width)
		{	x=x0;
			y+=size;
		}
        if(y>=height)
		{
			break;//退出
		}
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
/*
*********************************************************************************************************
*	函 数 名: Show_Chinese32
*	功能说明: 显示一个指定大小的汉字
*	形    参: x,y :汉字的坐标
*			  index:汉字在数组中的位置 
*			  size:字体大小 12/16/24
*			  pointcolor:画笔颜色
*	     	  backcolor：背景颜色
*			  mode:0,正常显示,1,叠加显示
*	返 回 值: 无
*********************************************************************************************************
*/
void Show_Chinese32(uint16_t x,uint16_t y,uint16_t index,uint8_t size,uint16_t pointcolor,uint16_t backcolor,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size);	//一个汉字数据字节数，直接存就可比如size=24一个汉字是｛72数据｝，16是32
	for(t=0;t<csize;t++)
	{   												   
		temp=hz32[index][t];			//得到点阵数据                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,pointcolor);
			else if(mode==0)
				LCD_Fast_DrawPoint(x,y,backcolor); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
}

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
