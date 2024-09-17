/*
*********************************************************************************************************
*
*	ģ������ : UART3����ģ��
*	�ļ����� : bsp_uart3.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-6-20 	�Ե�  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
#include "sys.h"
///////////////////////////////////////
/* ���������� */
#ifdef EN_UART3_RX							//���ʹ���˽���

uint8_t UART3_RX_BUF[UART3_BUF_LEN];		//���ջ�����
uint8_t UART3_RX_CNT = 0;						//���յ������ݳ���
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: USART3_IRQHandler
*	����˵��: USART3�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_IRQHandler(void)														//����1�жϷ������
{
	uint8_t res;
	
	//�ж��Ƿ���������ж�		
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)							//�����жϣ����յ�1���ֽڵ����ݣ�
	{
		res = USART_ReceiveData(USART3);//��ȡ���յ�������
		//�����ݴ��뻺����
		if(UART3_RX_CNT < UART3_BUF_LEN)
		{
			UART3_RX_BUF[UART3_RX_CNT] = res;//��¼���յ���ֵ
			UART3_RX_CNT++;
		}
	}
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart3
*	����˵��: UART3��ʼ��
*	��    ��: bound������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart3(uint32_t bound)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��UART4ʱ��
	
	
	//3.�˿ڳ�ʼ
	//TX,PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//RX,PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);							//��λUSART3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);							//ֹͣ��λ
	
	#ifdef EN_UART3_RX																//���ʹ���˽���
	
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						    //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;							    //��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	
	USART_Init(USART3,&USART_InitStructure);											//��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//����5�жϺ�							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//ʹ�ܴ���5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//��Ӧ
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);										//�򿪽����ж�
	USART_Cmd(USART3,ENABLE);														//ʹ�ܴ���								
	#endif												
	
}
/*
*********************************************************************************************************
*	�� �� ��: UART3_Send_Data
*	����˵��: UART3����len���֡�
*	��    ��: buf:�������׵�ַ
len:���͵��ֽ��� ��Ϊ�˺ͱ�����Ľ���ƥ�䣬���ｨ�鲻Ҫ����64���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART3_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
	UART3_RX_CNT = 0;
}
/*
*********************************************************************************************************
*	�� �� ��: UART3_Receive_Data
*	����˵��: UART3��ѯ���յ�������
*	��    �Σ�buf�����ջ����׵�ַ
			  len:���������ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART3_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART3_RX_CNT;
	uint8_t i=0;
	
	*len = 0;			//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms����������10msû�н��յ�һ�����ݣ�����Ϊ���ս���
	if(rxlen==UART3_RX_CNT && rxlen)//���յ������ݣ��ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i] = UART3_RX_BUF[i];
		}
		*len = UART3_RX_CNT;
		UART3_RX_CNT = 0;//����
	}
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
