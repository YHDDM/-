/*
*********************************************************************************************************
*
*	模块名称 : xx驱动模块
*	文件名称 : bsp_xxx.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    20xx-xx-xx waroyal  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 变量定义区 */
const uint8_t RELAY_ON_CMD[] = "relay_on_";
const uint8_t RELAY_OFF_CMD[] = "relay_off_";
const uint8_t RELAY_TOGGLE_CMD[] = "relay_toggle_";
const uint8_t RELAY_STATE_CMD[] = "relay_state_";
const uint8_t HELP_CMD[] = "help";
uint8_t RELAY_ON_RSP[] = "relay_X_state_on";
uint8_t RELAY_OFF_RSP[] = "relay_X_state_off";

//uint8_t RELAY_BSP_BYTE[9]={0};
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t CheckSum(uint8_t *buf,uint16_t len)
{
	uint16_t i;
	uint8_t sum = 0;
	
	for(i = 0;i < len;i++)
	{
		sum += buf[i];
	}
	
	return sum;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void usart2_cmd(void)
{
	//定义一个变量 暂存帧数据长度
	//定义一个变量 暂存继电器状态
	
	//1.判断命令是否正确，长度是否正确
	//2.判断命令的参数是否正确（如本例中的继电器序号）
	//3.按照协议解析对应的字节，完成相应的功能
	//4.注意清除串口1的接收状态和帧数据长度
	
	
	u16 len = 0;//长度暂存
	uint8_t status = 0;//状态暂存
	
	if(ReceiveState)
	{
		//bsp_LedToggle(2);
		len = RxCounter;
		
		//判断帧头是否正确
//		if((USART_RX_BUF[0] == FRAME_HEADER_BYTE_1st) && (USART_RX_BUF[1] == FRAME_HEADER_BYTE_2nd))
//		{
		uint8_t RELAY_RSP_BYTE[10];//暂存一整条指令的数组
		
		if((USART_RX_BUF[0]==FRAME_HEADER_BYTE_1st)&&(USART_RX_BUF[1]==FRAME_HEADER_BYTE_2nd)&&(USART_RX_BUF[2]==FRAME_HEADER_BYTE_3rd))
		{
			//printf("字节协议--帧头正确\r\n");
			if(USART_RX_BUF[3]==0x03)
			{
				if(USART_RX_BUF[len-1]==CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))
			{
				//printf("字节协议--校验和正确\r\n");
				if(USART_RX_BUF[4]==(len-FRAME_CONST_BYTE_LEN))
				{
					//printf("字节协议--数据长度正确\r\n");
					switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET])//数据的第一个命令（用于确定是上位机给下位机发的命令（0x01），还是下位机给上位机发的命令（0x02））
					{
						case FUN_CODE_CMD:
						{
							switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+1])//数据的第二个命令（用于确定是什么设备 0x01是继电器）
							{		
								case FUN_CODE_DEVICE_RELAY:
								{
									if(((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])>0)&&((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])<5))//数据的第三个命令（用于区分四个不同的继电器）
									{
										RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2];
										switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3])//数据的第四个命令（用于设备的功能指令）
										{
											case FUN_CODE_CMD_RELAY_ON://继电器闭合命令
											{
												//printf("字节协议--继电器闭合命令\r\n");
												bsp_RelayOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_ON;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_RELAY_OFF://继电器断开命令
											{
												//printf("字节协议--继电器断开命令\r\n");
												bsp_RelayOff(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_OFF;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_RELAY_TOGGLE://继电器翻转命令
											{
												//printf("字节协议--继电器翻转命令\r\n");
												bsp_RelayToggle(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												bsp_RelayOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												if(status==0)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_OFF;
												}else if(status==1)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_ON;
												}
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_RELAY_QUERY://继电器查询命令
											{
												//printf("字节协议--继电器查询命令\r\n");
												status = bsp_IsRelayOn(RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2]);
												if(status==0)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_OFF;
												}else if(status==1)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_ON;
												}
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											default:
												break;
										}
									}
									else
									{
										
									}
								}break;
								
								case FUN_CODE_DECEIVE_FAN:
								{
									if(((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])>0)&&((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])<3))//数据的第三个命令（用于区分四个不同的继电器）
									{
										RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2];
										switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3])//数据的第四个命令（用于设备的功能指令）
										{
											case FUN_CODE_CMD_FAN_ON://风扇开启命令
											{
												//printf("字节协议--风扇开启命令\r\n");
												bsp_FanOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_ON;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_FAN_OFF://风扇关闭命令
											{
												//printf("字节协议--风扇关闭命令\r\n");
												bsp_FanOff(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_OFF;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_FAN_TOGGLE://风扇翻转命令
											{
												//printf("字节协议--风扇翻转命令\r\n");
												bsp_FanToggle(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												bsp_FanOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												if(status==0)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_OFF;
												}else if(status==1)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_ON;
												}
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_FAN_QUERY://风扇查询命令
											{
												//printf("字节协议--风扇查询命令\r\n");
												status = bsp_IsFanOn(RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2]);
												if(status==0)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_OFF;
												}else if(status==1)
												{
													RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_ON;
												}
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											default:
												break;
										}
									}
									else
									{
										
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
			
		}
//		else if(memcmp(USART_RX_BUF,RELAY_ON_CMD,9)==0&&(len == 10))
//		{
//			if((USART_RX_BUF[9]>'0')&&(USART_RX_BUF[9]<'5'))
//			{
//				bsp_RelayOn((USART_RX_BUF[9]-'0'));
//				RELAY_ON_RSP[6] = USART_RX_BUF[9];
//			//	printf("%s\r\n",RELAY_ON_RSP);
//			}
//		}
//		else if((memcmp(USART_RX_BUF,RELAY_OFF_CMD,10)==0)&&(len==11))
//		{
//			if((USART_RX_BUF[10]>'0')&&(USART_RX_BUF[10]<'5'))
//			{
//				bsp_RelayOff((USART_RX_BUF[10]-'0'));
//				RELAY_OFF_RSP[6] = USART_RX_BUF[10];
//			//	printf("%s\r\n",RELAY_OFF_RSP);
//			}
//		}
//		else if((memcmp(USART_RX_BUF,RELAY_TOGGLE_CMD,13)==0)&&(len==14))
//		{
//			if((USART_RX_BUF[13]>'0')&&(USART_RX_BUF[13]<'5'))
//			{
//				bsp_RelayToggle((USART_RX_BUF[13]-'0'));
//				status = bsp_IsRelayOn(USART_RX_BUF[13]-'0');
//				switch(status)
//				{
//					case 0:
//					{
//						RELAY_OFF_RSP[6] = USART_RX_BUF[13];
//						//printf("%s\r\n",RELAY_OFF_RSP);
//					}break;
//					case 1:
//					{
//						RELAY_ON_RSP[6] = USART_RX_BUF[13];
//						//printf("%s\r\n",RELAY_ON_RSP);
//					}break;
//					default:break;
//				}
//			}
//		}
//		else if((memcmp(USART_RX_BUF,RELAY_TOGGLE_CMD,12)==0)&&(len==13))
//		{
//			if((USART_RX_BUF[12]>'0')&&(USART_RX_BUF[12]<'5'))
//			{
//				status = bsp_IsRelayOn(USART_RX_BUF[12]-'0');
//				switch(status)
//				{
//					case 0:
//					{
//						RELAY_OFF_RSP[6] = USART_RX_BUF[12];
//						//printf("%s\r\n",RELAY_OFF_RSP);
//					}break;
//					case 1:
//					{
//						RELAY_ON_RSP[6] = USART_RX_BUF[12];
//						//printf("%s\r\n",RELAY_ON_RSP);
//					}break;
//					default:break;
//				}
//			}
//		}
		Uart0_STA_Clr();//串口0状态清除
	
	//}
}
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

