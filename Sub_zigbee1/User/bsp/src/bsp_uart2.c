/*
*********************************************************************************************************
*
*	ģ������ : UART2����ģ��
*	�ļ����� : bsp_uart2.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-6-20	�Ե�  ��ʽ����
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
#ifdef EN_UART2_RX							//���ʹ���˽���

uint8_t UART2_RX_BUF[UART2_BUF_LEN];		//���ջ�����
uint8_t UART2_RX_CNT = 0;						//���յ������ݳ���
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: USART2_IRQHandler
*	����˵��: USART2�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_IRQHandler(void)														//����2�жϷ������
{
	uint8_t res;
	
	//�ж��Ƿ���������ж�		
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)							//�����жϣ����յ�1���ֽڵ����ݣ�
	{
		res = USART_ReceiveData(USART2);//��ȡ���յ�������
		//�����ݴ��뻺����
		if(UART2_RX_CNT < UART2_BUF_LEN)
		{
			UART2_RX_BUF[UART2_RX_CNT] = res;//��¼���յ���ֵ
			UART2_RX_CNT++;
		}
	}
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart2
*	����˵��: UART2��ʼ��
*	��    ��: bound������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart2(uint32_t bound)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	
	//3.�˿ڳ�ʼ
	//TX,PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//RX,PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);							//��λUART5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);							//ֹͣ��λ
	
	#ifdef EN_UART2_RX																//���ʹ���˽���
	
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						    //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;							    //��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	
	USART_Init(USART2,&USART_InitStructure);											//��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//����2�жϺ�							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//��Ӧ
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);										//�򿪽����ж�
	USART_Cmd(USART2,ENABLE);														//ʹ�ܴ���								
	#endif												
	
}
/*
*********************************************************************************************************
*	�� �� ��: UART2_Send_Data
*	����˵��: UART2����len���֡�
*	��    ��: buf:�������׵�ַ
len:���͵��ֽ��� ��Ϊ�˺ͱ�����Ľ���ƥ�䣬���ｨ�鲻Ҫ����64���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART2_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	UART2_RX_CNT = 0;
}
/*
*********************************************************************************************************
*	�� �� ��: UART2_Receive_Data
*	����˵��: UART2��ѯ���յ�������
*	��    �Σ�buf�����ջ����׵�ַ
			  len:���������ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART2_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART2_RX_CNT;
	uint8_t i=0;
	
	*len = 0;			//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms����������10msû�н��յ�һ�����ݣ�����Ϊ���ս���
	if(rxlen==UART2_RX_CNT && rxlen)//���յ������ݣ��ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i] = UART2_RX_BUF[i];
		}
		*len = UART2_RX_CNT;
		UART2_RX_CNT = 0;//����
	}
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
