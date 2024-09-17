/*
*********************************************************************************************************
*
*	ģ������ : �����������ģ��
*	�ļ����� : bsp_28byj48.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.2    2024-07-03 	Lawrence  ��ʽ����
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
static uint8_t _index = 0;
static uint8_t test[9]={1,5,2,6,3,7,4,8,1};

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSM28BYJ48
*	����˵��: GPIO��ʱ�ӳ�ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSM28BYJ48(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_ALL_28BYJ48,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//A��
	GPIO_InitStructure.GPIO_Pin = A_GPIO_PIN;
	GPIO_Init(A_GPIO_PORT,&GPIO_InitStructure);
	//B��
	GPIO_InitStructure.GPIO_Pin = B_GPIO_PIN;
	GPIO_Init(B_GPIO_PORT,&GPIO_InitStructure);
	//C��
	GPIO_InitStructure.GPIO_Pin = C_GPIO_PIN;
	GPIO_Init(C_GPIO_PORT,&GPIO_InitStructure);
	//D��
	GPIO_InitStructure.GPIO_Pin = D_GPIO_PIN;
	GPIO_Init(D_GPIO_PORT,&GPIO_InitStructure);
	
	//����Ĭ�Ϲر�
	GPIO_ResetBits(A_GPIO_PORT,A_GPIO_PIN);
	GPIO_ResetBits(B_GPIO_PORT,B_GPIO_PIN);
	GPIO_ResetBits(C_GPIO_PORT,C_GPIO_PIN);
	GPIO_ResetBits(D_GPIO_PORT,D_GPIO_PIN);

}

/*
*********************************************************************************************************
*	�� �� ��: motPowerOn
*	����˵��: ��ת
*	��    �Σ�uint8_t _ucMode
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void motPowerOn(uint8_t _ucMode)
{
	if(_ucMode == SM_OFF)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}
	else if(_ucMode == SM_A)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}
	else if(_ucMode == SM_B)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_C)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_D)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 1;
	}else if(_ucMode == SM_AB)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_BC)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_CD)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 1;
	}else if(_ucMode == SM_DA)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 1;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: motPowerOn2
*	����˵��: ��ת
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void motPowerOn2(uint8_t _ucMode)
{
	if(_ucMode == SM_OFF)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_DA)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}else if(_ucMode == SM_D)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_CD)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_C)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_BC)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_B)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_AB)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}else if(_ucMode == SM_A)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: test_fun
*	����˵��: �������
*	��    �Σ���=uint8_t _ucStop ����ֹͣ,uint8_t _ucDir ����ת,uint8_t _ucSpeed ת�����1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void test_fun(uint8_t _ucStop,uint8_t _ucDir,uint8_t _ucSpeed)
{
	if(_ucStop == 0)
	{
		
		switch(_ucDir)
		{
			case 0://��ת
			{
				if(bsp_CheckTimer(3))
				{
					motPowerOn(test[_index++]);
					if(_index == 9) _index = 0;
					bsp_StartTimer(3,_ucSpeed);
				}
				
			}break;
			
			case 1://��ת
			{
				if(bsp_CheckTimer(3))
				{
					if(_index <= 0) _index = 8;
					motPowerOn(test[_index--]);
					bsp_StartTimer(3,_ucSpeed);
				}
			}break;
			
			case 2:
			{
					motPowerOn(0);
			}break;
		}

	}

}


///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
