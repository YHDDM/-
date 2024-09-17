/*
*********************************************************************************************************
*
*	ģ������ : UART5����ģ��
*	�ļ����� : bsp_uart5.c
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
#ifdef EN_UART5_RX							//���ʹ���˽���

uint8_t UART5_RX_BUF[UART5_BUF_LEN];		//���ջ�����
uint8_t UART5_RX_CNT = 0;						//
static uint8_t ReceiveState5 = 0;				//����״̬���Ƿ���һ֡
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: USART5_IRQHandler
*	����˵��: USART5�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART5_IRQHandler(void)														//����5�жϷ������
{
	uint8_t res;
	
	//�ж��Ƿ���������ж�		
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET)							//�����жϣ����յ�1���ֽڵ����ݣ�
	{
		res = USART_ReceiveData(UART5);//��ȡ���յ�������
		//�����ݴ��뻺����
		if(UART5_RX_CNT < UART5_BUF_LEN)
		{
			UART5_RX_BUF[UART5_RX_CNT] = res;//��¼���յ���ֵ
			UART5_RX_CNT++;
		}
	}
	
		//�ж��Ƿ��ǿ����ж�
	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET)
	{
		//�����ȶ�ȡ״̬�Ĵ���SR,�ٶ����ݼĴ���DR���������IDLE�ж�
		res = UART5->SR;
		res = UART5->DR;
		ReceiveState5 = 1;	//��ʾ������һ֡����
	}
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: Uart5_STA_Clr
*	����˵��: �������״̬�ͳ��ȱ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart5_STA_Clr(void)
{
	UART5_RX_CNT = 0;
	ReceiveState5 = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart5
*	����˵��: UART5��ʼ��
*	��    ��: bound������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart5(uint32_t bound)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��UART5ʱ��
	
	
	//3.�˿ڳ�ʼ
		//TX,PC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//RX,PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	

	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5,ENABLE);//��λUART5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5,DISABLE);//ֹͣ��λ
	
	#ifdef EN_UART5_RX									//���ʹ���˽���
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(UART5,&USART_InitStructure);//��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//ʹ�ܴ���5�ж�								//����1�жϺ�							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//��Ӧ
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);									//�򿪽����ж�
	USART_Cmd(UART5,ENABLE);	//ʹ�ܴ���								
	#endif												
	
}
/*
*********************************************************************************************************
*	�� �� ��: UART5_Send_Data
*	����˵��: UART5����len���֡�
*	��    ��: buf:�������׵�ַ
len:���͵��ֽ��� ��Ϊ�˺ͱ�����Ľ���ƥ�䣬���ｨ�鲻Ҫ����64���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART5_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
		USART_SendData(UART5,buf[t]);
	}
	
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
	UART5_RX_CNT = 0;

	
}
///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
