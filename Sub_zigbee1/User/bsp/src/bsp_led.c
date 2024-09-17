/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ�ƣ�����½�����ʹ�ã��������ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-08-02 waroyal  ��ʽ����
*
*	Copyright (C), 2014-2019, �����׵���
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/* ��GPIOʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);
//	
//	GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */
//	
//	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
//	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
	
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_ALL_LED,ENABLE);
	
	//3.�ṹ�帳ֵ
	//LED0
	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//LED0��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //LED0ģʽ����Ϊ�������ģʽ
	
	//4.����GPIO_Init()�������г�ʼ��
	//LDE0
	GPIO_Init(LED0_GPIO_PORT,&GPIO_InitStructure);
	
	//LDE1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);

	
	//5.�˿ڳ�ʼ��ƽ����
	//λ��
//	LED0 = 1;
//	LED1 = 1;
	//�⺯��
	GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);

	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: LED����
*	��    ��: uint8_t _no: 1~2;
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if( _no == 1)
	{
		//λ��
		LED0 = 0;
	}else if( _no == 2)
	{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN); //�⺯������
	}
}

void bsp_LedOff(uint8_t _no)
{
	if( _no == 1)
	{
		LED0 = 1;
	}else if( _no == 2)
	{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	}
}



/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		LED0_GPIO_PORT->ODR ^= LED0_GPIO_PIN;
	}
	else if(_no == 2)
	{
		LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
	}
}



/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
