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
#define EXAMPLE_NAME "WSNEP_V01-105_UART ʵ�飨CO2��"
#define EXAMPLE_DATE "2019-08-02"
#define DEMO_VER "1.0"


/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);
static void PrintfHelp(void);


void control_device(void);  //�����豸����
void data_static(void);		//������״̬��ӡ����
void Command_control(void);///����ָ��
void change_receive(void);//�ж��Ƿ���
uint16_t co2_value=0;		//������̼��ʾ
uint16_t pm25_value = 0;	//PM2.5̼��ʾ
float lux = 0;				//��ʾ����ǿ��
uint8_t temp_max=27;      //�����¶�����
uint8_t temp_min=25;      //�����¶�����
uint8_t pachuang=0;         //������־λ
uint8_t hongwai=0;         //�����־λ
uint8_t flag_ceive='0';
//bsp_FanOn(1); ���Կ�������
//bsp_RelayOn(1);�̵�����ת״̬
//bsp_LedOn(1); LED��״̬
//pm25_value = PM25_READ();//��ȡPM2.5��ֵ
//co2_value = CO2_READ();//��ȡ������̼��ֵ
//lux = (float)GetLuxValue() / 100 ;//��ȡʵ�ʹ���ֵ
//Tem_Value�¶�
//RH_Value ʪ��

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
	/*
	ST �̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f10x.c �ļ�����Ҫ������
	���� CPU ϵͳ��ʱ�ӣ��ڲ� Flash ����ʱ������ FSMC �����ⲿ SRAM
	*/
	
	bsp_Init(); /* Ӳ����ʼ�� */
	PrintfLogo(); /* ��ӡ������Ϣ������ 1 */
	PrintfHelp(); /* ��ӡ������ʾ */
	
	
	bsp_InitUart2(9600);
	bsp_InitUart3(9600);
	
	bsp_InitRelay(); /* ��ʼ���̵��� */
	bsp_InitFan(); /* ��ʼ������ */	
	bsp_InitOpt3001();	//��ʼ�����մ�����
	bsp_InitIoSensor(); //IO �ʹ�����ģ���ʼ��
	
	
	
	
	bsp_StartAutoTimer(0, 300); /* ��ʱ�� 0 ���� 300 ���� */
	
	
	/* ����������ѭ���� */
	while(1) 
	{
		bsp_Idle(); /* CPU ����ʱִ�еĺ������� bsp.c */
		//bsp_FanOn(1);  //���Կ�������

		
		if(ReceiveState==1)
		{
			Command_control();  //���տ���ָ��
			change_receive();  //�ж��Ƿ���
			Uart0_STA_Clr();
		}

		
				
//		if(USART_RX_BUF[0]>0X03)     //����ʹ��
//		{

//		}
		
		control_device();      //�����豸����		
		if (bsp_CheckTimer(0)) /* ��ʱ�� */
		{	
			data_static();   //������ʪ������
			
		}
		

		
	}
}
void change_receive(void)  //ѡ����պ���
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
	if(USART_RX_BUF[0]=='J')    //�̵�������Э��
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
	if(USART_RX_BUF[0]=='L')   //���ȿ���Э��
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
void control_device(void)   //�����豸����
{
	/*************�����¶ȸߵͿ�������***********/
	if(temp_max<Tem_Value)    //��������
	{
		bsp_FanOn(1);  
	}else if(temp_min>Tem_Value) //�رշ���
	{
		bsp_FanOff(1);
	}
	
	/*�����Ӧ��� ����������*/
	{
		if(PIR_OUT_flag==1)
		{
			bsp_LedOn(1);
			bsp_BeepOn();
			bsp_StartAutoTimer(1,2000); /* ��ʱ�� 1 ���� 1000 ���� */
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
		bsp_StartAutoTimer(2,2000); /* ��ʱ�� 1 ���� 1000 ���� */
	}
	if(bsp_CheckTimer(2))
	{
		bsp_StopTimer(2);
		pachuang=0;
	}
	
	

}

/********************************************************************************/
void data_static(void)     //���������ݺ���
{
	
	pm25_value = PM25_READ();//��ȡPM2.5��ֵ
	co2_value = CO2_READ();//��ȡ������̼��ֵ
	lux = (float)GetLuxValue() / 100 ;//��ȡʵ�ʹ���ֵ

	/****************��ʪ����ʾ**********************/
	{
		if( SHT3X_GetValue() )
		{
			bsp_InitI2C();
			bsp_InitSht3x();
		}
		delay_ms(50);
		if(flag_ceive=='1')
		{	
			printf("002,%.1f,%.1f,%d,%d,%.2f,%d,%d\r\n", Tem_Value,RH_Value,co2_value,pm25_value,lux,pachuang,hongwai);  //��ʾ��ʪ��
		}
	}
	
	
		
	
	if
	( VIBRATE_OUT_flag == 1 )
	{
		VIBRATE_OUT_flag = 0; //����
		//printf ( "VIBRATE ����������\r\n" ); //����ʱ IO Ϊ�͵�ƽ����λʱ IO Ϊ�ߵ�ƽ
				/************************�������ñ�־λ**********************/
	}
	if ( PIR_OUT_flag == 1 )
	{
		PIR_OUT_flag = 0; //����
		//printf ( "PIR ����������\r\n" ); //����ʱ IO Ϊ�ߵ�ƽ����λʱ IO Ϊ�͵�ƽ
		 //�����־λ
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
