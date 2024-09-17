/*
*********************************************************************************************************
*
*	ģ������ : xx����ģ��
*	�ļ����� : bsp_xxx.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    20xx-xx-xx waroyal  ��ʽ����
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

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitRelay(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY,ENABLE);
	//�ڽṹ�帳ֵ֮ǰ���ر��ĸ��̵���
	bsp_RelayOff(1);
	bsp_RelayOff(2);
	bsp_RelayOff(3);
	bsp_RelayOff(4);
	//�ṹ�帳ֵ+��ʼ���ṹ��
	//��Ϊͨ������������ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;
	GPIO_Init(RELAY1_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;
	GPIO_Init(RELAY2_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;
	GPIO_Init(RELAY3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;
	GPIO_Init(RELAY4_GPIO_PORT,&GPIO_InitStructure);


}
/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayOn
*	����˵��: �̵�����
*	��    �Σ�uint8_t _no  �̵���ѡ�����_no:1~4
*	�� �� ֵ: 1��ʾ�պϣ�0��ʾ����
*********************************************************************************************************
*/
uint8_t bsp_IsRelayOn(uint8_t _no)
{
	if(_no == 1)//RELAY1
	{
			if((RELAY1_GPIO_PORT->ODR & RELAY1_GPIO_PIN) == RELAY1_GPIO_PIN)
		{
			return 1;
		}
			return 0;
		
	}else if(_no == 2)//RELAY2
	{
			if((RELAY2_GPIO_PORT->ODR & RELAY2_GPIO_PIN) == RELAY2_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}else if(_no == 3)//RELAY3
	{
			if((RELAY3_GPIO_PORT->ODR & RELAY3_GPIO_PIN) == RELAY3_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}else if(_no == 4)//RELAY4
	{
			if((RELAY4_GPIO_PORT->ODR & RELAY4_GPIO_PIN) == RELAY4_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}
	
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayOff
*	����˵��: �̵�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayOff(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1 = 0;
	}
	else if(_no == 2)
	{
		RELAY2 = 0;
	}
	else if(_no == 3)
	{
		RELAY3 = 0;
	}
	else if(_no == 4)
	{
		RELAY4 = 0;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayOn
*	����˵��: �̵�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayOn(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1 = 1;
	}
	else if(_no == 2)
	{
		RELAY2 = 1;
	}
	else if(_no == 3)
	{
		RELAY3 = 1;
	}
	else if(_no == 4)
	{
		RELAY4 = 1;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayToggle
*	����˵��: �̵���״̬��ת
*	��    �Σ�uint8_t _no �̵���ѡ�� _no:1~4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RelayToggle(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1_GPIO_PORT->ODR ^= RELAY1_GPIO_PIN;
	}else if(_no == 2)
	{
		RELAY2_GPIO_PORT->ODR ^= RELAY2_GPIO_PIN;
	}else if(_no == 3)
	{
		RELAY3_GPIO_PORT->ODR ^= RELAY3_GPIO_PIN;
	}else if(_no == 4)
	{
		RELAY4_GPIO_PORT->ODR ^= RELAY4_GPIO_PIN;
	}
}


///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
