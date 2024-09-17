/*
*********************************************************************************************************
*
*	ģ������ : �ж�����ģ��
*	�ļ����� : bsp_exti.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-3-12   SDZ  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
#include "bsp_exti.h"
///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
void KEY_IO_Init(void);
void EXTI_KEY_Init(void);
///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyEXTI
*	����˵��: �����жϰ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKeyEXTI(void)
{
	KEY_IO_Init();
	EXTI_KEY_Init();
}
/*
*********************************************************************************************************
*	�� �� ��: KEY_IO_Init
*	����˵��: ���ð�����Ӧ��GPIO
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void KEY_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��һ������GPIOʱ��*/
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	/*�ر�jtag��ʹ��SWD��������SWDģʽ����*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/*�ڶ������������еİ���GPIO*/
	GPIO_InitStructure.GPIO_Pin = WKUP_GPIO_PIN;   /*WKUP�˿�*/
	GPIO_InitStructure.GPIO_Mode = WKUP_GPIO_MODE; /*WKUP�˿�ģʽ*/
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure); /*��ʼ��WKUP*/
	
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;   /*KEY0�˿�*/
	GPIO_InitStructure.GPIO_Mode = KEY0_GPIO_MODE; /*KEY0�˿�ģʽ*/
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure); /*��ʼ��KEY0*/
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;   /*KEY1�˿�*/
	GPIO_InitStructure.GPIO_Mode = KEY1_GPIO_MODE; /*KEY1�˿�ģʽ*/
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure); /*��ʼ��KEY1*/
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;   /*KEY2�˿�*/
	GPIO_InitStructure.GPIO_Mode = KEY2_GPIO_MODE; /*KEY2�˿�ģʽ*/
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure); /*��ʼ��KEY2*/
}
/*
*********************************************************************************************************
*	�� �� ��: EXTI_KEY_Init
*	����˵��: �����еİ������ó��ⲿ�жϴ�����ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void EXTI_KEY_Init(void)
{
	//1.����ṹ��
	
	//����NVIC�ṹ��
	NVIC_InitTypeDef NVIC_Initstructure;
	//����EXTI�ṹ��
	EXTI_InitTypeDef EXTI_Initstructure;
	
	//2.��ʱ��
	
	//AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//3.�ж���ӳ��
	//WEKP(PA0)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	//K2(PE2)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	//K1(PE3)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	//K0(PE4)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	
	//WKUP(PA0)--------------
	//4.1�ⲿ�жϵĽṹ��
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line0;					//�ж���EXTI_Line0
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ģʽ���ж�
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Rising;		//������ʽ������
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//ʹ��
	//4.2�ⲿ�жϽṹ��д��Ĵ���
	EXTI_Init(&EXTI_Initstructure);								//д��Ĵ�������
	//K2(PE2)--------------
	//4.1�ⲿ�жϵĽṹ��
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line2;					//�ж���EXTI_Line2
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ģʽ���ж�
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//������ʽ�½���
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//ʹ��
	//4.2�ⲿ�жϽṹ��д��Ĵ���
	EXTI_Init(&EXTI_Initstructure);								//д��Ĵ�������
	//K1(PE3)--------------
	//4.1�ⲿ�жϵĽṹ��
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line3;					//�ж���EXTI_Line3
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ģʽ���ж�
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//������ʽ�½���
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//ʹ��
	//4.2�ⲿ�жϽṹ��д��Ĵ���
	EXTI_Init(&EXTI_Initstructure);								//д��Ĵ�������
	//K0(PE4)--------------
	//4.1�ⲿ�жϵĽṹ��
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line4;					//�ж���EXTI_Line4
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ģʽ���ж�
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//������ʽ�½���
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//ʹ��
	//4.2�ⲿ�жϽṹ��д��Ĵ���
	EXTI_Init(&EXTI_Initstructure);								//д��Ĵ�������
	
	
	//------------------------------------------------------------------------------------------
	//WKUP(PA0)--------------
	//5.1�ⲿ�жϵĽṹ��
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI0_IRQn;   			//�жϺ�IRQN
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��
	//5.2NVIC_Init()д��Ĵ���
	NVIC_Init(&NVIC_Initstructure);								//д��Ĵ�������-----
	//K0(PE4)--------------
	//5.1�ⲿ�жϵĽṹ��
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI4_IRQn;   			//�жϺ�IRQN
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��
	//5.2NVIC_Init()д��Ĵ���
	NVIC_Init(&NVIC_Initstructure);								//д��Ĵ�������
	//K2(PE2)--------------
	//5.1�ⲿ�жϵĽṹ��
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI2_IRQn;   			//�жϱ��
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��
	//5.2NVIC_Init()д��Ĵ���
	NVIC_Init(&NVIC_Initstructure);								//д��Ĵ�������
	//K1(PE3)--------------
	//5.1�ⲿ�жϵĽṹ��
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI3_IRQn;   			//�жϱ��
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��
	//5.2NVIC_Init()д��Ĵ���
	NVIC_Init(&NVIC_Initstructure);								//д��Ĵ�������
}

/*
*********************************************************************************************************
*	�� �� ��: EXTI0_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: �жϲ�����־����
*********************************************************************************************************
*/
//KEUP
void EXTI0_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //�����ж��߱�־
	{
		if(WKUP == WKUP_ACTIVE_LEVEL)
		{
			bsp_BeepToggle();
		}
		EXTI_ClearITPendingBit(EXTI_Line0);			//����ж����жϱ�־
	}
}

/*
*********************************************************************************************************
*	�� �� ��: EXTI4_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: �жϲ�����־����
*********************************************************************************************************
*/
//KEY0
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //�����ж��߱�־
	{
		if(KEY0 == KEY0_ACTIVE_LEVEL)
		{
			bsp_LedToggle(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);			//����ж����жϱ�־
	}
}
/*
*********************************************************************************************************
*	�� �� ��: EXTI3_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: �жϲ�����־����
*********************************************************************************************************
*/
//KEY1
void EXTI3_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //�����ж��߱�־
	{
		if(KEY1 == KEY1_ACTIVE_LEVEL)//����Ч��ƽ
		{
			bsp_LedToggle(2);
		}		
		EXTI_ClearITPendingBit(EXTI_Line3);			//����ж����жϱ�־
	}
}
/*
*********************************************************************************************************
*	�� �� ��: EXTI2_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: �жϲ�����־����
*********************************************************************************************************
*/
//KEY2
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //�����ж��߱�־
	{
		if(KEY2 == KEY2_ACTIVE_LEVEL)//����Ч��ƽ
		{
			bsp_LedToggle(1);
			bsp_LedToggle(2);
		}
		EXTI_ClearITPendingBit(EXTI_Line2);			//����ж����жϱ�־
	}
}


