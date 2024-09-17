/*
*********************************************************************************************************
*
*	ģ������ : UART1����ģ��
*	�ļ����� : bsp_uart1.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-3-20 	�Ե�  ��ʽ����
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
uint8_t	USART_RX_BUF[USART_REC_LEN] = {0};	//����1�Ļ�������
uint8_t	ReceiveState = 0;					//����״̬�Ƿ���������1֡��1�ǽ�����1֡
uint16_t RxCounter = 0;						//���ճ���
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ���ڳ�ʼ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t baud)
{
	//1.����ṹ��
	//GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//USART
	USART_InitTypeDef USART_InitStructure;
	
	
	#if EN_USART1_RX
	//NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	//2.��ʱ��
	//��GPIOAʱ��,USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//3.�˿ڳ�ʼ
	//TX,PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//RX,PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.���ڲ���	����
	USART_InitStructure.USART_BaudRate = baud;										//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ-ȫ˫��
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��
	USART_InitStructure.USART_StopBits =USART_StopBits_1;							//1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//���ݳ���8λ
	USART_Init(USART1,&USART_InitStructure);
	#if EN_USART1_RX
	//5.�ж�����	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//����1�жϺ�							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//��Ӧ
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);									//�򿪽����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);									//�򿪿����ж�
	#endif
	
	//6.����1ʹ��
	USART_Cmd(USART1,ENABLE);														//ʹ�ܴ���1
	
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler
*	����˵��: USART1�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USART1_IRQHandler(void)														//����1�жϷ������
{
	uint8_t Res = Res;
	
	//�ж��Ƿ���������ж�		
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)							//�����жϣ����յ�1���ֽڵ����ݣ�
	{
		//�����ݴ��뻺����
		USART_RX_BUF[RxCounter++]=USART1->DR;										//�ѽ��յ����ֽڱ���
	}
	
	//�ж��Ƿ���������ж�
	if(USART_GetITStatus(USART1,USART_IT_IDLE) ==  SET)							//�����жϣ����յ�1֡���ݣ�
	{
		//�����ȶ�״̬�Ĵ���SR���ٶ����ݼĴ���DR���������
		Res = USART1->SR;															//��SR�Ĵ���
		Res = USART1->DR;															//��DR�Ĵ������ȶ�SR���ٶ�DR������Ϊ�����IDLE�ж�
		ReceiveState = 1;															//��ǽ���״̬��λ����ʾ1֡���ݽ������
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: Uart0_STA_Clr
*	����˵��: ����0״̬����������������״̬�ͳ��ȱ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart0_STA_Clr(void)
{
	RxCounter = 0;//����BUF��������
	ReceiveState = 0;//����״̬����
}

//��Ӧ��printf֧�ֺ���������
#if 1
#pragma import(__use_no_semihosting)
/*��׼�����֧�ֺ���*/
struct __FILE
{
	int handle;
};
FILE __stdout;

/*����_sys_exit()�Ա���ʹ�ð�����ģʽ*/
void _sys_exit(int x)
{
	x = x;
}
/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/int fputc(int ch,FILE *f)
{
	while((USART1->SR&0x40)==0){};//ѭ�����ͣ�ֱ���������
		USART1->DR = (u8)ch;
		return ch;
}

#endif
//----------------------------------------------------------------------------------------------------------

/*
*********************************************************************************************************
*	�� �� ��: USART1_Send_Data
*	����˵��: USART1����len���֡�
*	��    ��: buf:�������׵�ַ
len:���͵��ֽ��� 0~255
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		USART_SendData(USART1,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}


///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
