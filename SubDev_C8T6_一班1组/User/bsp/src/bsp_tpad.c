/*
*********************************************************************************************************
*
*	ģ������ : ���ݰ�������ģ��
*	�ļ����� : bsp_tpad.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-04-07 ���໪  ��ʽ����
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
__IO uint16_t tpad_default_val = 0;		//__IO�޶���ֹ�������Ż��˱���

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
static void TPAD_TIM_GPIO_Config(void);
static void TPAD_TIM_Config(void);

static void bsp_TpadTimInit(void);
static void bsp_TpadReset(void);

static uint16_t Tpad_Get_val(void);
static uint16_t TPAD_Get_MaxVal( uint8_t n );

///////////////////////////////////////
/* ����ʵ���� */



/*
*********************************************************************************************************
*	�� �� ��: Tpad_Get_val
*	����˵��: ���ݴ���������ȡ���ʱ��,�����ʱ����ֱ�ӷ��ض�ʱ���ļ���ֵ 
*	��    �Σ���
*	�� �� ֵ: ���ݰ�����粶�񵽸ߵ�ƽ�ļ�����ֵ 
*********************************************************************************************************
*/
static uint16_t Tpad_Get_val(void)
{
	bsp_TpadReset();	//ÿ�β���ǰ����Ҫ��λ
	
	while(TIM_GetFlagStatus(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx)==RESET)
	{
		//����ĳһʱ�̣���ʱ����CNT��ֵ����
		if(TIM_GetCounter(TIMx_CAP_TIM)>(TPAD_ARR_MAX_VAL - 500))
		{
			return TIM_GetCounter(TIMx_CAP_TIM);
		}		
	}
	return TIMx_CAP_TIM_GetCapturex_FUN(TIMx_CAP_TIM);	//���ز���ֵ
}
/*
*********************************************************************************************************
*	�� �� ��: TPAD_Get_MaxVal
*	����˵��: ���ݴ���������ȡ���ʱ��,������Σ���ȡ���ֵ
*	��    �Σ�n ������������
*	�� �� ֵ: ���ݰ�����粶�񵽸ߵ�ƽ�ļ�����ֵ��n�β��������ֵ�� 
*********************************************************************************************************
*/
static uint16_t TPAD_Get_MaxVal( uint8_t n )
{
	uint16_t temp = 0;
	uint16_t res =0;
	while(n--)
	{
		temp = Tpad_Get_val();	//�õ�һ��ֵ
		if(temp > res)
		{
			res = temp;
		}
	};
	return res;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadScan
*	����˵��: ɨ�败������
*	��    �Σ�n ������������
*	�� �� ֵ: 0,û�а���;1,�а���; 
*********************************************************************************************************
*/
uint8_t bsp_TpadScan(void)
{
	static uint8_t keyen = 0; //0�����Կ�ʼ��⣻>0�������ܿ�ʼ���
	uint8_t res = 0;
	uint8_t sample = 3;			//Ĭ�ϲ���3��
	uint16_t rval;
	
	rval = TPAD_Get_MaxVal(sample);
	
	if(rval > (tpad_default_val+TPAD_GATE_VAL))
	{
		if(keyen == 0)
		{
			res =1;	//keyen == 0,��Ч
		}
		keyen = 3;		 /* ����Ҫ�ٹ�3��֮����ܰ�����Ч */
	}
	if(keyen)
	{
		keyen--;
	}
	return res;
	
}
/*
*********************************************************************************************************
*	�� �� ��: TPAD_TIM_GPIO_Config
*	����˵��: ���ݴ������� GPIO ��ʼ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);		 /* ʹ��TIMxʱ�� */ 
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);	 /* ʹ�ܶ�Ӧ���ݴ�������˿�ʱ�� */
	
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;			/* ���ݴ����˿����� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		/* �˿��ٶ�50MHz */ 
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;			/* �������� */ 
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);		/* ��ʼ�������˿� */
}
/*
*********************************************************************************************************
*	�� �� ��: TPAD_TIM_Config
*	����˵��: ���ݴ������� ��ʱ�� ��ʼ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TPAD_TIM_Config(void)
{
	//�ṹ������
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef			TIM_ICInitStructure;

	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	//������ʱ������
	TIM_TimeBaseStructure.TIM_Period = TPAD_ARR_MAX_VAL;				//ARR
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_CAP_TIM_PSC;			//PSC
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//��������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�������ظ����������߼�
	
	TIM_TimeBaseInit(TIMx_CAP_TIM,&TIM_TimeBaseStructure);
	
	
	TIM_ICInitStructure.TIM_Channel = TIMx_CAP_TIM_CHANNEL_x;//����ͨ��
	TIM_ICInitStructure.TIM_ICFilter = 0x03;//Ӳ���˲� 8��N
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//����Ƶ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//
	TIM_ICInit(TIMx_CAP_TIM,&TIM_ICInitStructure);
	
	/* ʹ�ܼ����� */ 
	TIM_Cmd( TIMx_CAP_TIM,ENABLE);
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadTimInit
*	����˵��: ���ݴ������� ��ʱ�� ���ų�ʼ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_TpadTimInit(void)
{
	TPAD_TIM_GPIO_Config(); 
	TPAD_TIM_Config(); 
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadReset
*	����˵��: ����TIM �õ�RCC�Ĵ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_TpadReset(void)
{
	//����ṹ��
	GPIO_InitTypeDef		GPIO_InitStructure;
	//��GPIOʱ��
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	//�򿪶�ʱ��ʱ��
	//TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(TIMx_CAP_TIM_CH_PORT,TIMx_CAP_TIM_CH_PIN);		//ͨ���������������Ϊ0���ŵ�
	
	delay_ms( 5 );
	
	TIM_SetCounter(TIMx_CAP_TIM, 0);								//����ȴ������ز���
	
	//TIM_ClearFlag(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx);				//�����ʶCC2
	TIM_ClearITPendingBit ( TIMx_CAP_TIM, TIMx_CAP_TIM_IT_CCx | TIMx_CAP_TIM_IT_UPDATE ); //����жϱ�־ 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadInit
*	����˵��: ��ÿ��ص�ʱ����������ȡֵ.
*	��    �Σ���
*	�� �� ֵ:  0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
*********************************************************************************************************
*/
uint8_t bsp_TpadInit(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	bsp_TpadTimInit();
	
	for(i=0;i<10;i++)			 /* ������ȡ10�� */
	{
		buf[i] = Tpad_Get_val();
		delay_ms(10);
	}
	for(i = 0;i<9;i++)			 /* ���� */ 
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])	 /* �������� */
			{
				temp = buf[i]; 
                buf[i] = buf[j]; 
                buf[j] = temp;
			}
		}
	}
	temp = 0;
	for(i=2;i<8;i++)
	{
		temp+=buf[i];		 /* ȡ�м��6�����ݽ���ƽ�� */ 
	}
	tpad_default_val=temp/6;
	
	printf("tpad_default_val:%delay_ms\r\n",tpad_default_val);
	printf("\r\n\r\n");
	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
	{
		return 1;		 /* ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������! */ 
	}
	return 0;
}


///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
