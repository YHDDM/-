/*
*********************************************************************************************************
*
*	ģ������ : uart����ģ��
*	�ļ����� : bsp_uart.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-20 ���໪  ��ʽ����
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
uint8_t 	USART_RX_BUF[USART_REC_LEN] = {0};	//����1 �Ľ��ջ�������
uint8_t 	ReceiveState = 0;				//����״̬���Ƿ���һ֡
uint16_t	RxCounter = 0;					//���ճ���
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ����1��ʼ��
*	��    �Σ�uint32_t baud ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t baud)
{
	//1.����ṹ��
	//GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//USART
	USART_InitTypeDef	USART_InitStructure;
	
	//NVIC
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//2.��ʱ��
	//��GPIOʱ�ӣ�USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//3.�˿ڲ�������
	//дTX,PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��RX, �˿�PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.���ڲ�������
	USART_InitStructure.USART_BaudRate = baud;											//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ--ȫ˫��
	USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//���ݳ���8λ
	USART_Init(USART1,&USART_InitStructure);
	
	//5.�жϲ�������
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//����1�жϺ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//�򿪽����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);			//�򿪿����ж�	
	
	//6.����1ʹ��
	USART_Cmd(USART1,ENABLE);								//ʹ�ܴ���1
	
}
/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler
*	����˵��: �жϷ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t Res = Res;
	//�Ƿ���������ж�
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//�����ݴ��뻺����//�Զ�����ж�
		USART_RX_BUF[RxCounter++] = USART1->DR;
	}
	//�ж��Ƿ��ǿ����ж�
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
	{
		//�����ȶ�ȡ״̬�Ĵ���SR,�ٶ����ݼĴ���DR���������IDLE�ж�
		Res = USART1->SR;
		Res = USART1->DR;
		ReceiveState = 1;	//��ʾ������һ֡����
	}
}
/*
*********************************************************************************************************
*	�� �� ��: Uart0_STA_Clr
*	����˵��: �������״̬�ͳ��ȱ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart0_STA_Clr(void)
{
	RxCounter = 0;
	ReceiveState = 0;
}

//��Ӧ��printf֧�ֺ���������
//����һ�´��룬֧��printf������������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
/*��׼����Ҫ֧�ֺ���*/
struct __FILE
{
	int handle;
};
FILE __stdout;

/*����_sys_ezit()�Ա���ʹ�ð�����ģʽ*/
void _sys_exit(int x)
{
	x= x;
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��:�ض���pufc��������������ʹ��printf�����Ӵ���1��ӡ��� 
*	��    �Σ�int ch,FILE *f
*	�� �� ֵ: ch
*********************************************************************************************************
*/

int fputc(int ch,FILE *f)
{
	while((USART1->SR & 0X40) == 0){};//ѭ�����ͣ�ֱ���������
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: USART1_Send_Data
*	����˵��: USART1����len����
*	��    �Σ�buf���������׵�ַ
				len�����͵��ֽ��� 0~255
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)== RESET);
		USART_SendData(USART1,buf[t]);
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void usart1_cmd_receive()
{
	uint8_t len;
//	uint8_t t;
	
	//����һ��������
	if(ReceiveState)
	{
		len = RxCounter;
//		for(t=0;t<len;t++)
//		{
//		printf("%x",USART_RX_BUF[t]);
//		}
	
		//�����ķ���
		USART1_Send_Data(USART_RX_BUF,RxCounter);
		Uart0_STA_Clr();
	}
}



///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
