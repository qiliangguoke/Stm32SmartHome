#include "bsp.h" 


/* �ײ�Ӳ������ */

/*
STM32 ÿ��ϵ�ж�����Ψһ��һ��оƬ���кţ�96 λ bit����
STM32F10X �ĵ�ַ�� 0x1FFFF7E8 
STM32F20X �ĵ�ַ�� 0x1FFF7A10
STM32F30X �ĵ�ַ�� 0x1FFFF7AC
STM32F40X �ĵ�ַ�� 0x1FFF7A10
STM32L1XX �ĵ�ַ�� 0x1FF80050
*/

/* SN ��ʼ��ַ */
#define STM32F10X_SN_ADDR 0x1FFFF7E8


/* ���������������̷������� */
#define EXAMPLE_NAME "WSNEP_V01-012_TFT LCD ��ʾʵ�飨FSMC��"
#define EXAMPLE_DATE "2023-7-07"
#define DEMO_VER "1.0"

/* ���� LCD ��ʾ��ع�����Ϣ */
#define DEV_NAME "WSNEP_V01"
#define LCD_EXAMPLE_NAME "zhangyaozong"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);
static void PrintfHelp(void);




uint16_t co2_value=0;
float lux=0; //��Ϊ��ʾ��
uint16_t PM25_value=0;

uint8_t temp_max=28;  //�¶�����
uint8_t temp_min=20;  //�¶�����
uint8_t  VIA_fla=0;   //�𶯼���־λ
uint8_t  FIR_fla=0;   //�𶯼���־λ
uint8_t  ceive='1';     //���ռ���־λ

void data_static(void);     //���������ݺ���
void Command_control(void);  //���տ���ָ��
void Nt_DisPlay(void);  //��ӡ�߿�
void Show_L(void);		//��ʾ����
void fengshan_temp(void);	//ʵʱ����
void Key_Control(void);  //�¶Ⱥ���
void usart2_cmd_receive(void);//����2�մ���1��
void usart1_cmd_receive(void);//����1�� ����2��
void flag_chuli(void);//��־λ����
void check_receive(void);//ѡ�����
/*
*********************************************************************************************************
* �� �� ��: main
* ����˵��: c �������
* �� �Σ���
* �� �� ֵ: �������(���账��)
*********************************************************************************************************
*/


int main(void)
{
//	uint8_t x=1;
//	uint8_t lcd_id[12]; /* ��� LCD ID �ַ��� */
	
	/*
	ST �̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f10x.c �ļ�����Ҫ������
	���� CPU ϵͳ��ʱ�ӣ��ڲ� Flash ����ʱ������ FSMC �����ⲿ SRAM
	*/
	bsp_Init(); /* Ӳ����ʼ�� */
	PrintfLogo(); /* ��ӡ������Ϣ������ 1 */
	PrintfHelp(); /* ��ӡ������ʾ */
	
	bsp_InitUart2(57600);	//��ʼ������2
	
	bsp_InitUart4(9600);/*ע�⣬���� 4 ��ʼ������ LCD ֮ǰ*/
	bsp_InitUART5(9600);/*ע�⣬���� 5 ��ʼ������ LCD ֮ǰ*/
	bsp_InitIoSensor(); //IO �ʹ�����ģ���ʼ��
	bsp_InitRelay();		//�̵�����ʼ��
	bsp_InitFan(); 			/* ��ʼ������ */
	bsp_InitI2C();			//IIC��ʼ��
	bsp_InitSht3x();		//��ʪ�ȴ�������ʼ��
	
	TFTLCD_Init(); /* TFTLCD ��ʼ��*/
	
	POINT_COLOR=BLUE; //������ɫ
	Nt_DisPlay();
	
	bsp_StartAutoTimer(0, 500); /* ��ʱ�� 0 ���� 500 ���� */	
	while(bsp_InitOpt3001())//��ⲻ�� OPT3001 ( OPT3001 ��ʼ������ IIC ��ʼ�������� ID ��ȡ )
	{
		delay_ms(1000);
		LED1=!LED1;//DS0 ��˸
	}
	/* ����������ѭ���� */
	while(1) 
	{
		
		
		
		bsp_Idle(); /* CPU ����ʱִ�еĺ������� bsp.c */
		
        //usart1_cmd();//�̵�������ָ��
		if(ReceiveState==1)   //�ж��Ƿ���յ�
		{	
			Command_control(); //���ƺ���
			flag_chuli();//��־λ
			USART2_Send_Data(USART_RX_BUF,RxCounter);  //�Ѵ���1���յ���ָ��ȫ�����ͳ�ȥ
			Uart0_STA_Clr();   //����֮������
		}
		Show_L(); //LCD��ʾ����
		fengshan_temp();  //���ƺ���
		Key_Control();  //��������
//		usart1_cmd_receive(); //����1���ͺ���			
		
		
		/************************��ʪ�ȴ�����***************************/
		if (bsp_CheckTimer(0)) /* ��ʱ�� */
		{
			bsp_LedToggle(1);
			//check_receive();//ѡ����
			usart2_cmd_receive();  //�����յ�����2����Ϣ ��ӡ������1
			data_static();	  //��ʾ����������		
		}
			
	}
}

void flag_chuli(void)//�Ա�־λ��ֵ
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
*	�� �� ��: usart1_cmd_receive
*	����˵��: ����1���գ�����2����
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: usart2_cmd_receive
*	����˵��: ����2���գ�����1����
*	��    �Σ���
*	�� �� ֵ: ��
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
	
		sprintf( wendu_shu, "%.2f",((float)(Tem_Value))); //�¶�ֵ
		sprintf( shidu_shu, "%.2f",((float)(RH_Value))); //ʪ��ֵ
		sprintf( cStr, "%.2f",((float)(lux))); //���ն�ֵ
	
		LCD_ShowString(204,52,200,16,16,(uint8_t*)wendu_shu); 
		LCD_ShowString(204,82,200,16,16,(uint8_t*)shidu_shu); 
		LCD_ShowString(204,112,200,16,16,(uint8_t*)cStr); 	
		LCD_ShowNum(188,142,co2_value,5,16);   //��ʾ�¶�
		LCD_ShowNum(181,172,PM25_value,5,16);	  //��ʾʪ��
	
		LCD_ShowNum(105,262,temp_max,2,16);   //��ʾ�¶�top
		LCD_ShowNum(245,262,temp_min,2,16);   //��ʾ�¶�
		//LCD_ShowString(170,262,200,16,16,(uint8_t*)"temp_min:");	 //��ʾ�¶���ĸ	
		

		if(USART_RX_BUF[0]=='J'&&(USART_RX_BUF[1]=='1'||USART_RX_BUF[1]=='3'))
		{
			Show_Chinese32(205,202,26,16,GREEN,WHITE,0);    //��ʾ��
		}
		else Show_Chinese32(205,202,27,16,RED,WHITE,0);    //��ʾ��
		
		if(temp_max<Tem_Value||(USART_RX_BUF[0]=='F'&&(USART_RX_BUF[1]=='1'||USART_RX_BUF[1]=='3')))
		{
			Show_Chinese32(205,232,26,16,GREEN,WHITE,0);    //��ʾ��
		}
		else Show_Chinese32(205,232,27,16,RED,WHITE,0);    //��ʾ��
		
		if(FIR_fla==12)			
		{	Show_Chinese32(100,325,28,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(130,325,29,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(160,325,30,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(190,325,31,20,RED,WHITE,0);    //��ʾ��
		}
		else
			LCD_Fill(100,320,250,345,WHITE);
		
		if(VIA_fla==12)
		{	
			Show_Chinese32(100,360,28,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(130,360,29,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(160,360,32,20,RED,WHITE,0);    //��ʾ��
			Show_Chinese32(190,360,33,20,RED,WHITE,0);    //��ʾ��
		}
		else
			LCD_Fill(100,350,250,400,WHITE);
}
void fengshan_temp(void)  //�������޿���
{
	if(temp_max<Tem_Value) bsp_FanOn(1);
	else bsp_FanOff(1);
	
	
	//�������������
	if(PIR_OUT_flag==1)
	{
		FIR_fla=12;
		bsp_BeepOn();
		bsp_StartTimer(1, 2000); /* ��ʱ�� 0 ���� 500 ���� */
	}
	
	if(bsp_CheckTimer(1))
	{
		FIR_fla=0;
		bsp_StopTimer(1);
		bsp_BeepOff();		
	}
	//*****************
	
	//�𶯴�����
	 if(VIBRATE_OUT_flag==1)
	 {
		VIA_fla=12;
		bsp_StartTimer(2, 2000); /* ��ʱ�� 0 ���� 500 ���� */
	 }
	 if(bsp_CheckTimer(2))
	 {
		 VIA_fla=0;
		bsp_StopTimer(1);
	 }
}
void Command_control(void)
{
	if(USART_RX_BUF[0]=='J')    //�̵�������Э��
	{
		
		switch(USART_RX_BUF[1])
		{
			case '1':
			{
				bsp_RelayOn(1);	//�򿪼̵���1
				bsp_RelayOn(2);//�򿪼̵���2
			}break;
			case '2':
			{
				bsp_RelayOff(1);	//�رռ̵���1
				bsp_RelayOff(2);	//�رռ̵���2
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
	if(USART_RX_BUF[0]=='F')   //���ȿ���Э��
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
	if(USART_RX_BUF[0]=='L')   //led����Э��
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
	if(USART_RX_BUF[0]=='B')   //���ȿ���Э��
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
void data_static(void)     //���������ݺ���
{
	
	
	co2_value = CO2_READ();//��ȡ������̼��ֵ
	lux = (float)GetLuxValue() / 100 ;//��ȡʵ�ʹ���ֵ
	PM25_value = PM25_READ();//��ȡpm2.5ֵ


	/****************��ʪ����ʾ**********************/
	{
		if( SHT3X_GetValue() )
		{
			bsp_InitI2C();
			bsp_InitSht3x();
		}
		//printf("%.1f,%.1f,%d,%d,%.2f,%d,%d\r\n", Tem_Value,RH_Value,co2_value,pm25_value,lux,pachuang,hongwai);  //��ʾ��ʪ��
		//printf("%.1f,%.1f,%d,%.1f,%d,%d,%d",Tem_Value, RH_Value,co2_value,lux,PM25_value,FIR_fla,VIA_fla);  //�¶ȣ�ʪ�ȣ�������̼������ǿ��

	}
			
		if ( VIBRATE_OUT_flag == 1 )   //�𶯴�����
		{
			VIBRATE_OUT_flag = 0; //����
			//printf ( "VIBRATE ����������\r\n" ); //����ʱ IO Ϊ�͵�ƽ����λʱ IO Ϊ�ߵ�ƽ
		}
		
		
		if ( PIR_OUT_flag == 1 )	//�����⴫����
		{
			PIR_OUT_flag = 0; //����
			//printf ( "PIR ����������\r\n" ); //����ʱ IO Ϊ�ߵ�ƽ����λʱ IO Ϊ�͵�ƽ
		}
		
		//�жϱ�־λѡ���Ƿ���
		if(ceive=='1')
		{
			printf("01,%.1f,%.1f,%d,%.1f,%d,%d,%d",Tem_Value, RH_Value,co2_value,lux,PM25_value,FIR_fla,VIA_fla); 
		}
	
//��ʱ����	
//	if ( HY_OUT_flag == 1 )
//	{
//		HY_OUT_flag = 0; //����
//		printf ( "HY ����������\r\n" ); //����ʱ IO Ϊ�͵�ƽ����λʱ IO Ϊ�ߵ�ƽ
//	}
////			if ( HE_OUT_flag == 1 )
////			{
////				HE_OUT_flag = 0; //����
////				printf ( "HE ����������\r\n" ); //����ʱ IO Ϊ�͵�ƽ����λʱ IO Ϊ�ߵ�ƽ
////			}
//	if ( KR_OUT_flag == 1 )
//	{
//		KR_OUT_flag = 0; //����
//		printf ( "KR ����������\r\n" ); //����ʱ IO Ϊ�͵�ƽ����λʱ IO Ϊ�ߵ�ƽ
//	}
	
}
void Nt_DisPlay(void)
{
	uint8_t lcd_id[12]; /* ��� LCD ID �ַ��� */
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//�� LCD ID ��ӡ�� lcd_id ���顣
	
	Show_Chinese32(30,13,2,24,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(55,13,3,24,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(80,13,4,24,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(105,13,5,24,GRAY,WHITE,0);    //��ʾ��		
	Show_Chinese32(55,52,0,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(81,52,1,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(170,10,6,12,GRAY,WHITE,0);    //��ʾ2
	Show_Chinese32(180,10,7,12,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(55,82,8,16,GRAY,WHITE,0);    //��ʾʪ
	Show_Chinese32(81,82,9,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(55,112,10,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(81,112,11,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(44,142,12,16,GRAY,WHITE,0);    //��ʾC
	Show_Chinese32(69,142,13,16,GRAY,WHITE,0);    //��ʾO
	Show_Chinese32(95,142,14,16,GRAY,WHITE,0);    //��ʾ2
	Show_Chinese32(39,172,15,16,GRAY,WHITE,0);    //��ʾP
	Show_Chinese32(55,172,16,16,GRAY,WHITE,0);    //��ʾM
	Show_Chinese32(71,172,17,16,GRAY,WHITE,0);    //��ʾ2
	Show_Chinese32(87,172,18,16,GRAY,WHITE,0);    //��ʾ.
	Show_Chinese32(103,172,19,16,GRAY,WHITE,0);    //��ʾ5
	Show_Chinese32(39,202,20,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(55,202,21,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(71,202,22,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(87,202,23,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(39,232,24,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(55,232,25,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(71,232,26,16,GRAY,WHITE,0);    //��ʾ��
	Show_Chinese32(87,232,27,16,GRAY,WHITE,0);    //��ʾ��
	
	LCD_ShowString(170,25,200,12,12,(uint8_t*)EXAMPLE_DATE); //������ʾ
	//LCD_ShowString(170,10,200,16,16,(uint8_t*)LCD_EXAMPLE_NAME);	//��ʾ����
	LCD_ShowString(30,440,200,16,16,(uint8_t*)DEMO_VER);
	LCD_ShowString(30,455,200,16,16,lcd_id); /* ��ʾ LCD ID */ 
	
	LCD_ShowString(30,262,200,16,16,(uint8_t*)"temp_max:");  //��ʾ�¶���ĸ
	LCD_ShowString(170,262,200,16,16,(uint8_t*)"temp_min:");	 //��ʾ�¶���ĸ	
	
	LCD_DrawLine(5,5,5,475);//����1
	LCD_DrawLine(150,45,150,285);//����2
	LCD_DrawLine(315,5,315,475);//����3
	LCD_DrawLine(5,5,315,5);//����1
	LCD_DrawLine(5,45,315,45);//����2
	LCD_DrawLine(5,75,315,75);//����3
	LCD_DrawLine(5,105,315,105);//����4
	LCD_DrawLine(5,135,315,135);//����5
	LCD_DrawLine(5,165,315,165);//����7
	LCD_DrawLine(5,195,315,195);//����8
	LCD_DrawLine(5,225,315,225);//����9
	LCD_DrawLine(5,255,315,255);//����10
	LCD_DrawLine(5,285,315,285);//����11
	LCD_DrawLine(5,475,315,475);//����11
	
	
	
	
	
	//		LCD_ShowNum(180,55,Tem_Value,3,24);   //��ʾ�¶�
//		LCD_ShowNum(180,95,RH_Value,3,24);	  //��ʾʪ��
//		LCD_ShowNum(180,135,lux,3,24);	  //��ʾ����ǿ��
//		LCD_ShowNum(180,175,co2_value,3,24);	  //������̼Ũ��
}

void Key_Control(void)
{
	uint8_t ucKeyCode;
	/* �������¼� */
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode > 0)
		{
			/* �м����� */
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
* �� �� ��: PrintfHelp
* ����˵��: ��ӡ������ʾ
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfHelp(void)
{
	printf("������ʾ:\r\n");
	printf("1.�������У�ÿ 1 �� DS0 ��Ʒ�תһ��\r\n");
	printf("2.ÿ 1 ���ȡ����ʾһ�� CO2 ���ݣ�ע�� CO2 �ϵ�Ԥ�� 3 ���Ӻ����ݲ�׼ȷ\r\n");
	printf("\r\n\r\n");
}
/*
*********************************************************************************************************
* �� �� ��: PrintfLogo
* ����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴򿪴��ڵ������ֹ۲���
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
/* ��� CPU ID */
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
	printf("* �������� : %s\r\n", EXAMPLE_NAME);/* ��ӡ�������� */
	printf("* ���̰汾 : %s\r\n", DEMO_VER); /* ��ӡ���̰汾 */
	printf("* �������� : %s\r\n", EXAMPLE_DATE); /* ��ӡ�������� */
	/* ��ӡ ST �̼���汾���� 3 ��������� stm32f10x.h �ļ��� */
	printf("* �� �� �� �� �� : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", 
	__STM32F10X_STDPERIPH_VERSION_MAIN,
	__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \r\n"); /* ��ӡһ�пո� */
	printf("*************************************************************\r\n");
}
/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
