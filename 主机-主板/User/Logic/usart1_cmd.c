#include "bsp.h"
#include "usart1_cmd.h"
//�ж�֡ͷ�Ƿ���ȷ



uint8_t RELAY_RSP_BYTE[9] = {FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,0x04,0x02,0x01,0x01,0xFF,0xFF};
/*
***************************************************************************
*          �� �� ����usart1_cmd
*          ����˵������������
*          ��   �Σ���
*          �� �� ֵ���� 
***************************************************************************
*/
void usart1_cmd(void)
{
	
	u16 len;		//�����ݴ�
	uint8_t status = 0; //״̬�ݴ�
	
	if ( ReceiveState )
	{
		len = RxCounter;
		printf("456");
		if( (USART_RX_BUF[0] == FRAME_HEADER_BYTE_1st) && (USART_RX_BUF[1] == FRAME_HEADER_BYTE_2nd) && 
		(USART_RX_BUF[2] == FRAME_HEADER_BYTE_3rd) )
		{
			printf("�ֽ�Э��--֡ͷ��ȷ\r\n");
			
			//�ж�У�����ȷ
			if(USART_RX_BUF[len-1] == CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))//9-5=4
			{
				printf("�ֽ�Э��--У�����ȷ\r\n");
				
				//���ݳ���Ҳ��ȷ
				if( USART_RX_BUF[3] == (len - FRAME_CONST_BYTE_LEN) ) //  9-5=4 0x04
				{
					printf("�ֽ�Э��--���ݳ�����ȷ\r\n");
					switch( USART_RX_BUF[FRAME_DATA_FIELD_OFFSET] )   //��������һλ
					{
						//��λ��->��λ��������
						case FUN_CODE_CMD:        //��һλ���������λ��->��λ��������
						{
							switch( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 1 ] )    //��������2λ���̵������� ָ��
							{
							//�Ǽ̵���������
							case FUN_CODE_DEVICE_RELAY:
							{
								//�̵����Ų�����ȷ �� 1~4 ֮��
								if( ( ( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] ) > 0 ) &&     
								( ( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] ) < 5 ) )     //��������3λ��ѡ��̵�����λ��
								{
									//���¼̵�����
									RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = 
									USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ];      //����̵���λ��
									
									//�жϾ�������
									switch( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 3 ] )     //�̵���״̬
									{
										//�պ�����
										case FUN_CODE_CMD_RELAY_ON:
										{
											printf("�ֽ�Э��--�̵����պ�����\r\n");
											bsp_RelayOn( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );    //�պϼ̵���2
											//���¼̵���״̬
											RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;								
											//����У��λ������
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//���ͷ�����Ϣ
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										
										//�Ͽ�����
										case FUN_CODE_CMD_RELAY_OFF:
										{
											printf("�ֽ�Э��--�̵����Ͽ�����\r\n");
											bsp_RelayOff( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );   //�Ͽ��̵���2
											//���¼̵���״̬
											RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = 
											FUN_CODE_RSP_RELAY_OFF;
											//����У��λ������
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//���ͷ�����Ϣ
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										
										//��ת����
										case FUN_CODE_CMD_RELAY_TOGGLE:
										{
											printf("�ֽ�Э��--�̵�����ת����\r\n");
											bsp_RelayToggle( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );
											status = 
											bsp_IsRelayOn( RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] );//��ѯ��Ӧ�̵���״̬
											if(status==0)
											{
												//���¼̵���״̬
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_OFF;
											}
											else if(status==1)
											{
												//���¼̵���״̬
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;
											}
											//����У��λ������
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//���ͷ�����Ϣ
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										//��ѯ����
										case FUN_CODE_CMD_RELAY_QUERY:
										{
											printf("�ֽ�Э��--�̵�����ѯ����\r\n");
											status = 
											bsp_IsRelayOn( RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] );//��ѯ��Ӧ�̵���״̬
											if(status==0)
											{
												//���¼̵���״̬
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_OFF;
											}
											else if(status==1)
											{
												//���¼̵���״̬
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;
											}
											//����У��λ������
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//���ͷ�����Ϣ
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										default:break;
									}
								}
								else
								{
								//�̵����Ų�������
								}
							}break;
							default:break;
							}
						}break;
						default:break;
					}
				}
			}
		}
		Uart0_STA_Clr();//����0״̬���
	}
}

/*
*********************************************************************************************************
*	�� �� ����CheckSum
*	����˵������У��
*	��    �Σ�	u8* buf �����׵�ַ��ַ
*				u16 len ��ҪУ����ֽڳ���
*	�� �� ֵ��u8 sum �ۼӺ�
*********************************************************************************************************
*/
uint8_t CheckSum(uint8_t *buf,uint16_t len)   //  ��5λ  ��һ������λ��len  4
{
	
    uint8_t i;
	uint8_t sum=0;
	for(i=0;i<len;i++)
		sum+=buf[i];
	if(sum<10)return sum;
	else if(sum>10)return sum%10;
	 
	return sum;
		
}

