/*
*********************************************************************************************************
*
*	ģ������ : ����4����ģ��
*	�ļ����� : bsp_uart4.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-07-01  �Ե�  ��ʽ����
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
uint8_t		USART4_RX_BUF[UART4_BUF_LEN]={0};					//���ջ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
uint8_t		ReceiveState4=0;									//����״̬���,1ʱ��������һ֡
uint16_t	RxCounter4=0;										//���ճ���
uint8_t 	relay_1,relay_2,relay_3,relay_4,fan_1,fan_2,vib,pir;//��ȡ���ư��֡����
float 		temp,humid,light,co2,pm25;							//��ȡ���ư�����
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
* �� �� ��: UART4_IRQHandler
* ����˵��: ���� 4 �жϡ�
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
	uint8_t res;
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //���յ�����
	{	
		res =USART_ReceiveData(UART4); //��ȡ���յ�������
		if(RxCounter4<UART4_BUF_LEN)
		{
			USART4_RX_BUF[RxCounter4]=res; //��¼���յ���ֵ
			RxCounter4++; //������������ 1
		}
		ReceiveState4=1;//��ǽ���״̬��λ����ʾ������1֡����
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Uart4_STA_Clr
*	����˵��: ����4״̬�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart4_STA_Clr(void)
{
	RxCounter4=0;
	ReceiveState4=0;
}
/*
*********************************************************************************************************
* �� �� ��: bsp_InitUart4
* ����˵��: ��ʼ�� CPU �� UART4 ����Ӳ���豸��
* �� �Σ�uint32_t bound:������
* �� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart4(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ�� GPIOC ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ�� UART4 ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);//��λ���� 4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);//ֹͣ��λ
	
#ifdef EN_UART4_RX //���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); ; //��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //ʹ�ܴ��� 4 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
	
	USART_Cmd(UART4, ENABLE); //ʹ�ܴ���
#endif
}

/*
*********************************************************************************************************
* �� �� ��: UART4_Send_Data
* ����˵��: UART4 ���� len ���ֽ�
* �� �Σ� buf:�������׵�ַ
* len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� 64 ���ֽ�)
* �� �� ֵ: ��
*********************************************************************************************************
*/
void USART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
		USART_SendData(UART4,buf[t]);
	}
	
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}
/*
*********************************************************************************************************
*	�� �� ��: usart4_cmd_receive
*	����˵��: �����Ľ��գ�����һ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void usart4_cmd_receive()
{	
	//�����Ľ�������
	if(ReceiveState4)
	{
		
		USART1_Send_Data(USART4_RX_BUF,RxCounter4);

		if(USART4_RX_BUF[8] == 0x22 && USART4_RX_BUF[9] == 0x00) //Ϊ�����������豸
		{
			if(USART4_RX_BUF[10] == 0x01 && USART4_RX_BUF[11] == 0x02)//�¶ȴ�������������
			{
				*((char *)(&temp)) 	   = USART4_RX_BUF[15];
				*((char *)(&temp) + 1) = USART4_RX_BUF[14];
				*((char *)(&temp) + 2) = USART4_RX_BUF[13];
				*((char *)(&temp) + 3) = USART4_RX_BUF[12];					
			}
			
			if(USART4_RX_BUF[18] == 0x02 && USART4_RX_BUF[19] == 0x02)//ʪ�ȴ���������
			{
				
				*((char *)(&humid))     = USART4_RX_BUF[23];
				*((char *)(&humid) + 1) = USART4_RX_BUF[22];
				*((char *)(&humid) + 2) = USART4_RX_BUF[21];
				*((char *)(&humid) + 3) = USART4_RX_BUF[20];
			}
			
			if(USART4_RX_BUF[26] == 0x04 && USART4_RX_BUF[27] == 0x02)//���մ���������
			{					
				*((char *)(&light))     = USART4_RX_BUF[31];
				*((char *)(&light) + 1) = USART4_RX_BUF[30];
				*((char *)(&light) + 2) = USART4_RX_BUF[29];
				*((char *)(&light) + 3) = USART4_RX_BUF[28];
			}
			

			if(USART4_RX_BUF[34] == 0x05 && USART4_RX_BUF[35] == 0x02)//������̼����������
			{
				*((char *)(&co2))     = USART4_RX_BUF[39];
				*((char *)(&co2) + 1) = USART4_RX_BUF[38];
				*((char *)(&co2) + 2) = USART4_RX_BUF[37];
				*((char *)(&co2) + 3) = USART4_RX_BUF[36];				
			}	

			if(USART4_RX_BUF[42] == 0x06 && USART4_RX_BUF[43] == 0x02)//PM2.5
			{
				*((char *)(&pm25))     = USART4_RX_BUF[47];
				*((char *)(&pm25) + 1) = USART4_RX_BUF[46];
				*((char *)(&pm25) + 2) = USART4_RX_BUF[45];
				*((char *)(&pm25) + 3) = USART4_RX_BUF[44];	
			}
			
			if(USART4_RX_BUF[50] == 0x03)//��
			{
				if(USART4_RX_BUF[52] == 0x3F || USART4_RX_BUF[53] == 0x3F || USART4_RX_BUF[54] == 0x3F || USART4_RX_BUF[55] == 0x3F)
				{
					vib = 1;//��
				}	
				else 
				{
					vib = 0;
				}						
			}		
			if(USART4_RX_BUF[58] == 0x07)//����
			{
				if(USART4_RX_BUF[60] == 0x3F)
					pir = 1;
				else
					pir = 0;
			}						
		}	
		Uart4_STA_Clr();
	}
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
