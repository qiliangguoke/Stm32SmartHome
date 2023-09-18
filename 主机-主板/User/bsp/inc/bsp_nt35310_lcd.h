/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : .h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_NT35310_LCD_H_
#define _BSP_NT35310_LCD_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"
#include "bsp.h"
///////////////////////////////////////

/* 宏定义区域 */
//移植修改区
//按照实际端口修改
//较前版本更加容易移植(注意，端口仅仅适用于非 JTAG/SWD 引脚，如果是 JTAG 引脚，需要打开 AFIO 时钟，并失能JTAG)

#define RCC_ALL_LCD ( LCD_CS_GPIO_CLK | LCD_RS_GPIO_CLK | LCD_WR_GPIO_CLK \
		| LCD_RD_GPIO_CLK | LCD_LED_GPIO_CLK \
		| LCD_D0_GPIO_CLK | LCD_D1_GPIO_CLK  \
        | LCD_D2_GPIO_CLK | LCD_D3_GPIO_CLK  \
		| LCD_D4_GPIO_CLK | LCD_D5_GPIO_CLK  \
		| LCD_D6_GPIO_CLK | LCD_D7_GPIO_CLK  \
		| LCD_D8_GPIO_CLK | LCD_D9_GPIO_CLK  \
		| LCD_D10_GPIO_CLK | LCD_D11_GPIO_CLK \
		| LCD_D12_GPIO_CLK | LCD_D13_GPIO_CLK \
		| LCD_D14_GPIO_CLK | LCD_D15_GPIO_CLK \
)

/******控制信号线******/
//背光引脚
#define LCD_LED_GPIO_PIN 			GPIO_Pin_0 //液晶背光引脚号
#define LCD_LED_PIN_ID 				0 //液晶背光引脚序号
#define LCD_LED_GPIO_PORT 			GPIOB //液晶背光端口号
#define LCD_LED_GPIO_CLK 			RCC_APB2Periph_GPIOB //液晶背光时钟
#define LCD_LED_FUN_OUT 			PBout //液晶背光输出端口配置函数
//#define LCD_LED_FUN_IN 		PBin //液晶背光输入端口配置函数

//片选引脚
//FSMC_NE4 为 PG12
#define LCD_CS_GPIO_PIN 			GPIO_Pin_12 //液晶片选引脚号
#define LCD_CS_PIN_ID 				12 //液晶片选引脚序号
#define LCD_CS_GPIO_PORT 			GPIOG //液晶片选端口号
#define LCD_CS_GPIO_CLK 			RCC_APB2Periph_GPIOG //液晶片选时钟
#define LCD_CS_FUN_OUT 				PGout //液晶片选输出端口配置函数
//#define LCD_CS_FUN_IN 		PGin //液晶片选输入端口配置函数
#define LCD_CS_FSMC_NE_ID 			4 //设置为 NE4，如果是其它需要再修改，必须对应上

//RS 命令/数据引脚，使用 FSMC 的地址信号控制，本引脚决定访问 LCD 时使用的地址（数据/命令）
//FSMC_A10 为 PG0
#define LCD_RS_GPIO_PIN 			GPIO_Pin_0 //液晶 RS 引脚号
#define LCD_RS_PIN_ID 				0 //液晶 RS 引脚序号
#define LCD_RS_GPIO_PORT 			GPIOG //液晶 RS 端口号
#define LCD_RS_GPIO_CLK 			RCC_APB2Periph_GPIOG //液晶 RS 时钟
#define LCD_RS_FUN_OUT 				PGout //液晶 RS 输出端口配置函数
//#define LCD_RS_FUN_IN 		PGin //液晶 RS 输入端口配置函数
#define LCD_RS_FSMC_ADDR_ID 		10 //设置为 A10，如果是其它再修改,必须对应上

//写使能引脚
#define LCD_WR_GPIO_PIN 			GPIO_Pin_5 //液晶写使能引脚号
#define LCD_WR_PIN_ID 				5 //液晶写使能引脚序号
#define LCD_WR_GPIO_PORT 			GPIOD //液晶写使能端口号
#define LCD_WR_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶写使能时钟
#define LCD_WR_FUN_OUT 				PDout //液晶写使能输出端口配置函数
//#define LCD_WR_FUN_IN PDin //液晶写使能输入端口配置函数

//读使能引脚
#define LCD_RD_GPIO_PIN 			GPIO_Pin_4 //液晶读使能引脚号
#define LCD_RD_PIN_ID 				4 //液晶读使能引脚序号
#define LCD_RD_GPIO_PORT 			GPIOD //液晶读使能端口号
#define LCD_RD_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶读使能时钟
#define LCD_RD_FUN_OUT 				PDout //液晶读使能输出端口配置函数
//#define LCD_WR_FUN_IN 		PDin //液晶读使能输入端口配置函数

/******数据信号线******/

//Dx 引脚
#define LCD_D0_GPIO_PIN 			GPIO_Pin_14 //液晶 数据 Dx 引脚号
#define LCD_D0_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D0_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D1_GPIO_PIN 			GPIO_Pin_15 //液晶 数据 Dx 引脚号
#define LCD_D1_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D1_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D2_GPIO_PIN 			GPIO_Pin_0 //液晶 数据 Dx 引脚号
#define LCD_D2_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D2_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D3_GPIO_PIN 			GPIO_Pin_1 //液晶 数据 Dx 引脚号
#define LCD_D3_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D3_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D4_GPIO_PIN 			GPIO_Pin_7 //液晶 数据 Dx 引脚号
#define LCD_D4_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D4_GPIO_CLK	 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D5_GPIO_PIN 			GPIO_Pin_8 //液晶 数据 Dx 引脚号
#define LCD_D5_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D5_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D6_GPIO_PIN 			GPIO_Pin_9 //液晶 数据 Dx 引脚号
#define LCD_D6_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D6_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D7_GPIO_PIN 			GPIO_Pin_10 //液晶 数据 Dx 引脚号
#define LCD_D7_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D7_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D8_GPIO_PIN 			GPIO_Pin_11 //液晶 数据 Dx 引脚号
#define LCD_D8_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D8_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D9_GPIO_PIN 			GPIO_Pin_12 //液晶 数据 Dx 引脚号
#define LCD_D9_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D9_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D10_GPIO_PIN 			GPIO_Pin_13 //液晶 数据 Dx 引脚号
#define LCD_D10_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D10_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D11_GPIO_PIN 			GPIO_Pin_14 //液晶 数据 Dx 引脚号
#define LCD_D11_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D11_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D12_GPIO_PIN 			GPIO_Pin_15 //液晶 数据 Dx 引脚号
#define LCD_D12_GPIO_PORT 			GPIOE //液晶 数据 Dx 端口号
#define LCD_D12_GPIO_CLK 			RCC_APB2Periph_GPIOE //液晶 数据 Dx 时钟
#define LCD_D13_GPIO_PIN 			GPIO_Pin_8 //液晶 数据 Dx 引脚号
#define LCD_D13_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D13_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D14_GPIO_PIN 			GPIO_Pin_9 //液晶 数据 Dx 引脚号
#define LCD_D14_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D14_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟
#define LCD_D15_GPIO_PIN 			GPIO_Pin_10 //液晶 数据 Dx 引脚号
#define LCD_D15_GPIO_PORT 			GPIOD //液晶 数据 Dx 端口号
#define LCD_D15_GPIO_CLK 			RCC_APB2Periph_GPIOD //液晶 数据 Dx 时钟

/////////////////////////////////////////////////////////////////////////////////////////////////
//移植修改区结束//////
//////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////
////////////////////////////////

//以下区域无需修改！！！！//////
////////////////////////////////
////////////////////////////////
///******************************* NT35310 显示屏的 FSMC 参数定义 ***************************/

#if LCD_CS_FSMC_NE_ID == 1
//由片选引脚决定的 NOR/SRAM 块
#define 	FSMC_Bank1_NORSRAMx 	FSMC_Bank1_NORSRAM1
#define 	FSMC_Bank1_ADDRx 		0x60000000
#elif LCD_CS_FSMC_NE_ID == 2
//由片选引脚决定的 NOR/SRAM 块
#define 	FSMC_Bank1_NORSRAMx 	FSMC_Bank1_NORSRAM2
#define 	FSMC_Bank1_ADDRx 		0x64000000
#elif LCD_CS_FSMC_NE_ID == 3
//由片选引脚决定的 NOR/SRAM 块
#define 	FSMC_Bank1_NORSRAMx 	FSMC_Bank1_NORSRAM3
#define 	FSMC_Bank1_ADDRx 		0x68000000
#elif LCD_CS_FSMC_NE_ID == 4

//由片选引脚决定的 NOR/SRAM 块
#define 	FSMC_Bank1_NORSRAMx 	FSMC_Bank1_NORSRAM4
#define 	FSMC_Bank1_ADDRx 		0x6C000000

#endif

//FSMC_Bank1_NORSRAM 用于 LCD 命令操作的地址
#define 	FSMC_Addr_NT35310_CMD ( ( uint32_t ) ( FSMC_Bank1_ADDRx | ( ( ( 1 << \
LCD_RS_FSMC_ADDR_ID ) - 1 ) << 1 ) ) )

//FSMC_Bank1_NORSRAM 用于 LCD 数据操作的地址 
#define FSMC_Addr_NT35310_DATA ( ( uint32_t ) ( FSMC_Bank1_ADDRx | ( ( 1 << \
LCD_RS_FSMC_ADDR_ID ) << 1 ) ) )
/////////////////////////////////////////////////////////////////////////////////////////////////


//IO 操作函数
#define LCD_LED LCD_LED_FUN_OUT(LCD_LED_PIN_ID) //LCD_LED LCD 背光

///////////////////////////////////////
/* 外部变量申明区 */



//LCD重要参数集
typedef struct  
{										    
	uint16_t	width;			//LCD 宽度
	uint16_t	height;			//LCD 高度
	uint16_t	id;				//LCD ID
	uint8_t		dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t	setxcmd;		//设置x坐标指令
	uint16_t	setycmd;		//设置y坐标指令 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

///////////////////////////////////////

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0xF81F
#define GRED 			 0xFFE0
#define GBLUE			 0x07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0xBC40 //棕色
#define BRRED 			 0xFC07 //棕红色
#define GRAY  			 0x8430 //灰色
//GUI颜色

#define DARKBLUE      	 0x01CF	//深蓝色
#define LIGHTBLUE      	 0x7D7C	//浅蓝色  
#define GRAYBLUE       	 0x5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0x841F //浅绿色
#define LGRAY 			 0xC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0xA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0x2B12 //浅棕蓝色(选择条目的反色)
///////////////////////////////////////

/* 函数申明区 */
void TFTLCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(uint16_t Color);	 												//清屏
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);										//设置光标
void LCD_DrawPoint(uint16_t x,uint16_t y);											//画点
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);								//快速画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 											//读点 
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 			//画圆
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//画线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   				//画矩形
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   				//填充单色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//填充指定颜色
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//显示一个字符串,12/16字体

void Show_Chinese32(uint16_t x,uint16_t y,uint16_t index,uint8_t size,uint16_t pointcolor,uint16_t backcolor,uint8_t mode);


void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_Scan_Dir(uint8_t dir);									//设置屏扫描方向
void LCD_Display_Dir(uint8_t dir);								//设置屏幕显示方向
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);	//设置窗口					   						   																			 

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
