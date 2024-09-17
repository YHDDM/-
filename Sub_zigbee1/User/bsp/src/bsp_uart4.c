/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_uart4.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-03   SDZ  ��ʽ����
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
#ifdef EN_UART4_RX		//���ʹ���˽���
uint8_t  UART4_RX_BUF[UART4_BUF_LEN];    //���ܻ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з������ܻ������飩
//���յ������ݵĳ���
uint8_t UART4_RX_CNT = 0;	
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: UART4_IRQHandler
*	����˵��: UART4�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    �Σ���
*	�� �� ֵ: ��
*   ע    �⣺USARTx->SR �ܱ���Ī������Ĵ���
*********************************************************************************************************
*/
void UART4_IRQHandler(void) //����4�жϷ���������
{
	uint8_t res;
	//�Ƿ��ǽ����ж�
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)	//���յ�����
	{
		res = USART_ReceiveData(UART4);
		if(UART4_RX_CNT<UART4_BUF_LEN)
		{
			UART4_RX_BUF[UART4_RX_CNT] = res;
			UART4_RX_CNT++;
		}
	}
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart4
*	����˵��: ��ʼ��CPU��UART4����Ӳ���豸
*	��    �Σ�uint32_t baud ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart4(uint32_t bound)
{
	/*����ṹ��*/
	//GPIO
	GPIO_InitTypeDef      GPIO_Instructure;
	//NVIC
	NVIC_InitTypeDef      NVIC_InitStructure;
	//USART
	USART_InitTypeDef     USART_InitStructure;
	/*��ʱ��*/
	/*��GPIOʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��GPIOCʱ��
	/*��UART4ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ��
	
	/*�˿ڳ�ʼ*/
	/*����UART Tx PA9*/
	GPIO_Instructure.GPIO_Pin = GPIO_Pin_10;//PC10
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_Init(GPIOC,&GPIO_Instructure);
	//RX,PA10
	GPIO_Instructure.GPIO_Pin = GPIO_Pin_11;//PC11
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOC,&GPIO_Instructure);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);
	
#ifdef EN_UART4_RX
	//���ڲ�������
	USART_InitStructure.USART_BaudRate = bound;                                         //������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                    //�շ�ģʽ---ȫ˫��
	USART_InitStructure.USART_Parity = USART_Parity_No;                                //����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                             //1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        //���ݳ���8λ
	USART_Init(UART4,&USART_InitStructure);
	
	//�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;            //����1�жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);  //�򿪽����ж�
	
	//����4ʹ��
	USART_Cmd(UART4,ENABLE);//ʹ�ܴ���1
#endif
}
/*
*********************************************************************************************************
*	�� �� ��: UART4_Send_Data
*	����˵��: UART4����len���ֽ�
*	��    �Σ�buf����ȥ�׵�ַ
			  len���͵��ֽ���(���鲻Ҫ����64���ֽ�)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++)//ѭ����������
	{
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
		USART_SendData(UART4,buf[t]);
	}
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
	UART4_RX_CNT = 0;
}
/*
*********************************************************************************************************
*	�� �� ��: UART4_Receive_Data
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART4_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART4_RX_CNT;
	uint8_t i = 0;
	*len = 0;
	delay_ms(10);
	if(rxlen==UART4_RX_CNT&&rxlen)
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i] = UART4_RX_BUF[i];
		}
		*len = UART4_RX_CNT;
		UART4_RX_CNT = 0;
	}
}

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
