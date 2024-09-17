/*
*********************************************************************************************************
*
*	ģ������ : ����2����ģ��
*	�ļ����� : bsp_uart2.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-07-01 	�Ե�  ��ʽ����
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
uint8_t		USART2_RX_BUF[UART2_BUF_LEN]={0};		//���ջ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
uint8_t		ReceiveState2=0;						//����״̬���,1ʱ��������һ֡
uint16_t	RxCounter2=0;							//���ճ���

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
* �� �� ��: UART2_IRQHandler
* ����˵��: ���� 2 �жϡ�
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_IRQHandler(void)
{
	uint8_t res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	
		res =USART_ReceiveData(USART2); //��ȡ���յ�������
		if(RxCounter2<UART2_BUF_LEN)
		{
			USART2_RX_BUF[RxCounter2]=res; //��¼���յ���ֵ
			RxCounter2++; //������������ 1
		}
		ReceiveState2=1;//��ǽ���״̬��λ����ʾ������1֡����
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Uart2_STA_Clr
*	����˵��: ����2״̬�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart2_STA_Clr(void)
{
	RxCounter2=0;
	ReceiveState2=0;
}
/*
*********************************************************************************************************
* �� �� ��: bsp_InitUart2
* ����˵��: ��ʼ�� CPU �� UART2 ����Ӳ���豸��
* �� �Σ�uint32_t bound:������
* �� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart2(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ�� GPIOC ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ�� USART2 ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC10 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PC11 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ���� 2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
	
#ifdef EN_UART2_RX //���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(USART2, &USART_InitStructure); ; //��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	
	USART_Cmd(USART2, ENABLE); //ʹ�ܴ���
#endif
}

/*
*********************************************************************************************************
* �� �� ��: USART2_Send_Data
* ����˵��: USART2 ���� len ���ֽ�
* �� �Σ� buf:�������׵�ַ
* len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� 64 ���ֽ�)
* �� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
/*
*********************************************************************************************************
*	�� �� ��: usart2_cmd_receive()
*	����˵��: /���ڶ����գ�����һ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void usart2_cmd_receive(void)
{
	//���ڶ���������
	if(ReceiveState2)
	{
		USART1_Send_Data(USART2_RX_BUF,RxCounter2);
		Uart2_STA_Clr();
	}
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
