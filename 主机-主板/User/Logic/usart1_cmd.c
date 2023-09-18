#include "bsp.h"
#include "usart1_cmd.h"
//判断帧头是否正确



uint8_t RELAY_RSP_BYTE[9] = {FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,0x04,0x02,0x01,0x01,0xFF,0xFF};
/*
***************************************************************************
*          函 数 名：usart1_cmd
*          功能说明：串口命令
*          形   参：无
*          返 回 值：无 
***************************************************************************
*/
void usart1_cmd(void)
{
	
	u16 len;		//长度暂存
	uint8_t status = 0; //状态暂存
	
	if ( ReceiveState )
	{
		len = RxCounter;
		printf("456");
		if( (USART_RX_BUF[0] == FRAME_HEADER_BYTE_1st) && (USART_RX_BUF[1] == FRAME_HEADER_BYTE_2nd) && 
		(USART_RX_BUF[2] == FRAME_HEADER_BYTE_3rd) )
		{
			printf("字节协议--帧头正确\r\n");
			
			//判断校验和正确
			if(USART_RX_BUF[len-1] == CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))//9-5=4
			{
				printf("字节协议--校验和正确\r\n");
				
				//数据长度也正确
				if( USART_RX_BUF[3] == (len - FRAME_CONST_BYTE_LEN) ) //  9-5=4 0x04
				{
					printf("字节协议--数据长度正确\r\n");
					switch( USART_RX_BUF[FRAME_DATA_FIELD_OFFSET] )   //数据区第一位
					{
						//上位机->下位机的命令
						case FUN_CODE_CMD:        //第一位控制命令，上位机->下位机的命令
						{
							switch( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 1 ] )    //数据区第2位，继电器控制 指令
							{
							//是继电器的命令
							case FUN_CODE_DEVICE_RELAY:
							{
								//继电器号参数正确 在 1~4 之间
								if( ( ( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] ) > 0 ) &&     
								( ( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] ) < 5 ) )     //数据区第3位，选择继电器的位号
								{
									//更新继电器号
									RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = 
									USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ];      //放入继电器位号
									
									//判断具体命令
									switch( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 3 ] )     //继电器状态
									{
										//闭合命令
										case FUN_CODE_CMD_RELAY_ON:
										{
											printf("字节协议--继电器闭合命令\r\n");
											bsp_RelayOn( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );    //闭合继电器2
											//更新继电器状态
											RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;								
											//计算校验位并更新
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//发送反馈信息
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										
										//断开命令
										case FUN_CODE_CMD_RELAY_OFF:
										{
											printf("字节协议--继电器断开命令\r\n");
											bsp_RelayOff( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );   //断开继电器2
											//更新继电器状态
											RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = 
											FUN_CODE_RSP_RELAY_OFF;
											//计算校验位并更新
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//发送反馈信息
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										
										//翻转命令
										case FUN_CODE_CMD_RELAY_TOGGLE:
										{
											printf("字节协议--继电器翻转命令\r\n");
											bsp_RelayToggle( USART_RX_BUF[ FRAME_DATA_FIELD_OFFSET + 2 ] );
											status = 
											bsp_IsRelayOn( RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] );//查询对应继电器状态
											if(status==0)
											{
												//更新继电器状态
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_OFF;
											}
											else if(status==1)
											{
												//更新继电器状态
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;
											}
											//计算校验位并更新
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//发送反馈信息
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										//查询命令
										case FUN_CODE_CMD_RELAY_QUERY:
										{
											printf("字节协议--继电器查询命令\r\n");
											status = 
											bsp_IsRelayOn( RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] );//查询对应继电器状态
											if(status==0)
											{
												//更新继电器状态
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_OFF;
											}
											else if(status==1)
											{
												//更新继电器状态
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3] = FUN_CODE_RSP_RELAY_ON;
											}
											//计算校验位并更新
											RELAY_RSP_BYTE[8] = 
											CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
											//发送反馈信息
											USART1_Send_Data(RELAY_RSP_BYTE,9);
											printf("\r\n");
										}break;
										default:break;
									}
								}
								else
								{
								//继电器号参数错误
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
		Uart0_STA_Clr();//串口0状态清除
	}
}

/*
*********************************************************************************************************
*	函 数 名：CheckSum
*	功能说明：和校验
*	形    参：	u8* buf 数组首地址地址
*				u16 len 需要校验的字节长度
*	返 回 值：u8 sum 累加和
*********************************************************************************************************
*/
uint8_t CheckSum(uint8_t *buf,uint16_t len)   //  第5位  第一个数据位，len  4
{
	
    uint8_t i;
	uint8_t sum=0;
	for(i=0;i<len;i++)
		sum+=buf[i];
	if(sum<10)return sum;
	else if(sum>10)return sum%10;
	 
	return sum;
		
}

