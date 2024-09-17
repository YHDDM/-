/*
*********************************************************************************************************
*
*	ģ������ : ������������ģ��
*	�ļ����� : bsp_Key.c
*	��    �� : V1.1
*	˵    �� : ɨ�������������������˲����ƣ����а���FIFO�����Լ�������¼���
				��1����������
				��2����������
				��3��������
				��4������ʱ�Զ�����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-06 	�Ե�  ��ʽ����
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
static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* ����FIFO�������ṹ�� */
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);
///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
*********************************************************************************************************
*/
/* Ϊ������6���¼���WKUP�������£�KEY0�������£�KEY1�������£�KEY2�������£�WKUP��KEY0ͬʱ���£�KEY0��KEY1ͬʱ���� */
static uint8_t IsKeyDown1(void)
{
	if(( WKUP == WKUP_ACTIVE_LEVEL ) && ( KEY0 != KEY0_ACTIVE_LEVEL )
		&& ( KEY1 != KEY1_ACTIVE_LEVEL ) && ( KEY2 != KEY2_ACTIVE_LEVEL ))
		return 1;
	else
		return 0;
}

static uint8_t IsKeyDown2(void)
{
	if(( WKUP != WKUP_ACTIVE_LEVEL ) && ( KEY0 == KEY0_ACTIVE_LEVEL )
		&& ( KEY1 != KEY1_ACTIVE_LEVEL ) && ( KEY2 != KEY2_ACTIVE_LEVEL ))
		return 1;
	else	
		return 0;
}

static uint8_t IsKeyDown3(void)
{
	if(( WKUP != WKUP_ACTIVE_LEVEL ) && ( KEY0 != KEY0_ACTIVE_LEVEL )
		&& ( KEY1 == KEY1_ACTIVE_LEVEL ) && ( KEY2 != KEY2_ACTIVE_LEVEL ))
		return 1;
	else	
		return 0;
}

static uint8_t IsKeyDown4(void)
{
	if(( WKUP != WKUP_ACTIVE_LEVEL ) && ( KEY0 != KEY0_ACTIVE_LEVEL )
		&& ( KEY1 != KEY1_ACTIVE_LEVEL ) && ( KEY2 == KEY2_ACTIVE_LEVEL ))
		return 1;
	else	
		return 0;
}

static uint8_t IsKeyDown5(void)
{
	if(( WKUP == WKUP_ACTIVE_LEVEL ) && ( KEY0 == KEY0_ACTIVE_LEVEL )
		&& ( KEY1 != KEY1_ACTIVE_LEVEL ) && ( KEY2 != KEY2_ACTIVE_LEVEL ))
		return 1;
	else	
		return 0;
}

static uint8_t IsKeyDown6(void)
{
	if(( WKUP != WKUP_ACTIVE_LEVEL ) && ( KEY0 == KEY0_ACTIVE_LEVEL )
		&& ( KEY1 == KEY1_ACTIVE_LEVEL ) && ( KEY2 != KEY2_ACTIVE_LEVEL ))
		return 1;
	else	
		return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ���������ú�����bsp_Init()����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();			/* ��ʼ���������� */
	bsp_InitKeyHard();			/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ������
*	��    ��:  KeyCode �� ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;
	
	if(++s_tKey.Write >= KEY_FIFO_SIZE )
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;
	
	if(s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
		
	{
		ret = s_tKey.Buf[s_tKey.Read];
		
		if(++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Get2Key
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ��
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;
	
	if(s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];
		
		if(++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 =  0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��: _ucKeyID : ����ID,��0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    ��: 	_ucKeyID : ����ID,��0��ʼ
				_LongTime : �����¼�ʱ��
				_RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/*����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;	/*�����������ٶȣ�0��ʾ��֧������*/
	s_tBtn[_ucKeyID].RepeatCount = 0;				/*����������*/
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	//WKUP-------------------------------------------------------------------------------------
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin = WKUP_GPIO_PIN;	//WKUP����
	GPIO_InitStructure.GPIO_Mode = WKUP_GPIO_MODE;	//WKUP�˿�ģʽ
	//4.����GPIO_Init
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure);	//WKUP��ʼ��
	
	//KEY0-------------------------------------------------------------------------------------
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;	//KEY0����
	GPIO_InitStructure.GPIO_Mode = KEY0_GPIO_MODE;	//KEY0�˿�ģʽ
	//4.����GPIO_Init
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure);	//KEY0��ʼ��
	
	//KEY1-------------------------------------------------------------------------------------
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;	//KEY1����
	GPIO_InitStructure.GPIO_Mode = KEY1_GPIO_MODE;	//KEY1�˿�ģʽ
	//4.����GPIO_Init
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);	//KEY1��ʼ��
	
	//KEY2-------------------------------------------------------------------------------------
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;	//KEY2����
	GPIO_InitStructure.GPIO_Mode = KEY2_GPIO_MODE;	//KEY2�˿�ģʽ
	//4.����GPIO_Init
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);	//KEY2��ʼ��

}


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;
	
	/*�԰���FIFO��дָ������*/
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;
	
	/*��ÿ�������ṹ���Ա������һ��ȱʡֵ*/
	for(i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;				/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;			/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
//		s_tBtn[i].KeyCodeDown = 3 * i + 1;			/* �������µļ�ֵ���� */
//		s_tBtn[i].KeyCodeUp		= 3 * i + 2;			/* ��������ļ�ֵ���� */
//		s_tBtn[i].KeyCodeLong	= 3 * i + 3;			/* �������������µļ�ֱ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ������ʱ�� */
	}
	
	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	/* ���磬����ϣ������1���³���1����Զ��ط���ͬ��ֵ */
//	s_tBtn[KID_K1].LongTime = 100;
//	s_tBtn[KID_K1].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
	
	
	/* �жϰ������µĺ��� */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	
	/* ��ϼ� */
	s_tBtn[4].IsKeyDownFunc = IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc = IsKeyDown6;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��: �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	/* 
		���û�г�ʼ�������������򱨴�
		if (s_tBtn[i].IsKeyDownFunc == 0)
			{
				printf("Fault");
			}
	*/
	pBtn = &s_tBtn[i];
	if(pBtn->IsKeyDownFunc())
	{
		if(pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if(pBtn->State == 0)
			{
				pBtn->State = 1;
				
				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}
			
			if(pBtn->LongTime > 0)
			{
				if(pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if(++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ��Ű���FIFO */
					bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if(pBtn->RepeatSpeed > 0)
					{
						if(++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	
	else
		
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if(pBtn->State == 1)
			{
				pBtn->State = 0;
				
				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}
		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;
	
	for(i = 0;i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}


/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
