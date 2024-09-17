/*
*********************************************************************************************************
*
*	ģ������ : ִ����Э������ģ��
*	�ļ����� : uart1_cmd.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-07-04 waroyal  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
///////////////////////////////////////
/* ���������� */
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: usart1_cmd
*	����˵��: ����ִ�����������ݴ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void usart1_cmd(void)
{
	//����һ������ �ݴ�֡���ݳ���
	//����һ������ �ݴ�̵���״̬
	
	//1.�ж������Ƿ���ȷ�������Ƿ���ȷ
	//2.�ж�����Ĳ����Ƿ���ȷ���籾���еļ̵�����ţ�
	//3.����Э�������Ӧ���ֽڣ������Ӧ�Ĺ���
	//4.ע���������1�Ľ���״̬��֡���ݳ���
	
	u16 len = 0;//�����ݴ�
	uint8_t status = 0;//״̬�ݴ�
	
	if(ReceiveState)
	{
		len = RxCounter;
		if((USART_RX_BUF[0]==FRAME_HEADER_BYTE_1st)&&(USART_RX_BUF[1]==FRAME_HEADER_BYTE_2nd)&&(USART_RX_BUF[2]==FRAME_HEADER_BYTE_3rd))
		{
			uint8_t RELAY_RSP_BYTE[10];//�ݴ�һ����ָ�������
			//printf("�ֽ�Э��--֡ͷ��ȷ\r\n");
			if(USART_RX_BUF[len-1]==CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))
			{
				//printf("�ֽ�Э��--У�����ȷ\r\n");
				if(USART_RX_BUF[4]==(len-FRAME_CONST_BYTE_LEN))
				{
					//printf("�ֽ�Э��--���ݳ�����ȷ\r\n");
					switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET])//���ݵĵ�һ���������ȷ������λ������λ���������0x01����������λ������λ���������0x02����
					{
						case FUN_CODE_CMD:
						{
							switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+1])//���ݵĵڶ����������ȷ����ʲô�豸 0x01�Ǽ̵�����
							{		
								case FUN_CODE_DEVICE_RELAY:
								{
									if(((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])>0)&&((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])<5))//���ݵĵ�����������������ĸ���ͬ�ļ̵�����
									{
										RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2];
										switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3])//���ݵĵ��ĸ���������豸�Ĺ���ָ�
										{
											case FUN_CODE_CMD_RELAY_ON://�̵����պ�����
											{
												//printf("�ֽ�Э��--�̵����պ�����\r\n");
												bsp_RelayOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_ON;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_RELAY_OFF://�̵����Ͽ�����
											{
												//printf("�ֽ�Э��--�̵����Ͽ�����\r\n");
												bsp_RelayOff(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_RELAY_OFF;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_RELAY_TOGGLE://�̵�����ת����
											{
												//printf("�ֽ�Э��--�̵�����ת����\r\n");
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
											case FUN_CODE_CMD_RELAY_QUERY://�̵�����ѯ����
											{
												//printf("�ֽ�Э��--�̵�����ѯ����\r\n");
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
									if(((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])>0)&&((USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2])<3))//���ݵĵ�����������������ĸ���ͬ�ļ̵�����
									{
										RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+2] = USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2];
										switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3])//���ݵĵ��ĸ���������豸�Ĺ���ָ�
										{
											case FUN_CODE_CMD_FAN_ON://���ȿ�������
											{
												//printf("�ֽ�Э��--���ȿ�������\r\n");
												bsp_FanOn(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_ON;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_FAN_OFF://���ȹر�����
											{
												//printf("�ֽ�Э��--���ȹر�����\r\n");
												bsp_FanOff(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
												RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET+3]=FUN_CODE_RSP_FAN_OFF;
												RELAY_RSP_BYTE[9] = CheckSum(&RELAY_RSP_BYTE[FRAME_DATA_FIELD_OFFSET],4);
												USART1_Send_Data(RELAY_RSP_BYTE,10);
												//printf("\r\n");
											}break;
											case FUN_CODE_CMD_FAN_TOGGLE://���ȷ�ת����
											{
												//printf("�ֽ�Э��--���ȷ�ת����\r\n");
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
											case FUN_CODE_CMD_FAN_QUERY://���Ȳ�ѯ����
											{
												//printf("�ֽ�Э��--���Ȳ�ѯ����\r\n");
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
		Uart0_STA_Clr();//����0״̬���
	}
}
///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

