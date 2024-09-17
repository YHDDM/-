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
*		V1.0    2024-04-07 lawrence  ��ʽ����
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

//__IO uint16_t tpad_default_val = 0; 

/* volatile �޶�������������ÿ�η���status����ʱ�����ڴ��ж�ȡ���µ�ֵ��
������ʹ�üĴ����еĻ���ֵ����������ȷ�������ڷ���status����ʱ�ܹ�������µ�ֵ���������������Ľ��*/

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
static void TPAD_TIM_GPIO_Config(void);
static void TPAD_TIM_Config(void);

static void bsp_TpadTimInit(void);
static void bsp_TpadReset(void);

static uint16_t TPAD_Get_Val(void);
static uint16_t	TPAD_Get_MaxVal(uint8_t n);


///////////////////////////////////////
/* ����ʵ���� */
/*tpad ����ֵ*/
vu16 tpad_default_val = 0;
/*
*********************************************************************************************************
*	�� �� ��: TPAD_Get_Val
*	����˵��: ���ݴ���������ȡ���ʱ�䣬�����ʱ����ֱ�ӷ��ض�ʱ���ļ���ֵ
*	��    ��: ��
*	�� �� ֵ: ���ݰ�����粶�񵽸ߵ�ƽ�ļ�����ֵ
*********************************************************************************************************
*/
static uint16_t TPAD_Get_Val(void)
{
	bsp_TpadReset();//ÿ�β����ʱ�򣬱����ȸ�λ�ŵ�
	 //�ȴ�����������
	while(TIM_GetFlagStatus(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx) == RESET)
	{
		//һ����ʱ�䶼����ms�������ڣ����ٻᳬ����ʱ�������ʱֵ
		if(TIM_GetCounter(TIMx_CAP_TIM) > TPAD_ARR_MAX_VAL - 500)
		{
			//��ʱ�ˣ�ֱ�ӷ���CNT��ֵ
			return TIM_GetCounter(TIMx_CAP_TIM);
		}
	};
	return TIMx_CAP_TIM_GetCapturex_FUN(TIMx_CAP_TIM);
}

/*
*********************************************************************************************************
*	�� �� ��: TPAD_Get_MaxVal
*	����˵��: ���ݴ���������ȡ���ʱ�䣬������Σ���ȡ���ֵ
*	��    ��: n������������
*	�� �� ֵ: ���ݰ�����粶�񵽸ߵ�ƽ�ļ�����ֵ��n�β��������ֵ��
*********************************************************************************************************
*/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp = 0;
	uint16_t res = 0;
	
	while(n--)
	{
		temp = TPAD_Get_Val();//�õ�һ��ֵ
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
*	��    ��: n ������������
*	�� �� ֵ: 0 û�а���  1�а���
*********************************************************************************************************
*/
uint8_t bsp_TpadScan(void)
{
	static uint8_t keyen = 0;//0�����Կ�ʼ���	>0�������ܿ�ʼ���
	uint8_t res = 0;
	uint8_t sample = 3;		 //Ĭ�ϲ�������Ϊ3��
	uint16_t rval;
	
	rval = TPAD_Get_MaxVal(sample);
	
	/*����tpad_default_val+TPAD_GATE_VAL����Ч*/
	
	if(rval > (tpad_default_val+TPAD_GATE_VAL))
	{
		if(keyen == 0)
		{
			res = 1;//keyen==0��Ч
		}
		keyen = 3;	//ֻҪҪ��3�β��ܰ�����Ч
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
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);		//ʹ��TIMxʱ��
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);	//ʹ�ܶ�Ӧ���ݴ�������˿�ʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;			//���ݴ����˿�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�˿��ٶ� 50MHz
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);		//��ʼ���������˿�
}
/*
*********************************************************************************************************
*	�� �� ��: TPAD_TIM_Config
*	����˵��: ���ݴ������� ��ʱ��  ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TPAD_TIM_Config(void)
{
	//�ṹ������
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//����ṹ��
	TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	//�򿪶�ʱ��ʱ��
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);

	//������ʱ������
	TIM_TimeBaseStructure.TIM_Period = TPAD_ARR_MAX_VAL;			//ARR�����Զ���װ�ؼĴ���
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_CAP_TIM_PSC;			//PSC����ʱ��Ԥ��Ƶ�Ĵ���
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//����ʱ�ӷָTDTS = Tck_tim�����ﲻ��ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				//�ظ���������ֵ���߼���û�õ�����������
	TIM_TimeBaseInit(TIMx_CAP_TIM,&TIM_TimeBaseStructure);			//��ʼ��TIMx
	
	//�����������
	TIM_ICInitStructure.TIM_Channel = TIMx_CAP_TIM_CHANNEL_x;		//�������벶���ͨ��
	TIM_ICInitStructure.TIM_ICFilter = 0x03;						//8���˲�
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
	TIM_ICInitStructure.TIM_ICPrescaler =TIM_ICPSC_DIV1;			//��Ƶ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ֱ��ӳ��
	TIM_ICInit(TIMx_CAP_TIM,&TIM_ICInitStructure);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIMx_CAP_TIM,ENABLE);
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadTimInit
*	����˵��: ���ݴ�������  ��ʱ��  ���ų�ʼ������
*	��    ��: ��
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
*	����˵��: ���� TIM �õ�RCC �Ĵ���
*	��    ��: ��
*	�� �� ֵ: TIM ����ʱ����
*********************************************************************************************************
*/
static void bsp_TpadReset(void)
{
	//����ṹ��
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	//��GPIOʱ��
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	
	//�򿪶�ʱ����ʱ��
	//TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	//�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(TIMx_CAP_TIM_CH_PORT,TIMx_CAP_TIM_CH_PIN);//ͨ���������������Ϊ0���ŵ�
	
	delay_ms(5);//�ŵ�ȴ�
	
	TIM_SetCounter(TIMx_CAP_TIM,0);//���㣬�ȴ������ز���
	//TIM_ClearFlag(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx);//�����־CC2
	TIM_ClearITPendingBit(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx | TIMx_CAP_TIM_IT_UPDATE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	//��ʼ���
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_TpadInit	��ʼ����������
*	����˵��: ��ÿ��ص�ʱ����������ȡֵ
*	��    ��: ��
*	�� �� ֵ: 0����ʼ���ɹ���1��ʼ��ʧ��
*********************************************************************************************************
*/
uint8_t bsp_TpadInit(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	bsp_TpadTimInit();
	
	for(i=0 ; i<10 ; i++)/*����ȡ10��*/
	{
		buf[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	for(i = 0; i < 9; i++)//����
	{
		for( j = i+1;j<10;j++)
		{
			if(buf[i] > buf[j])//��������
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
		temp += buf[i];	//ȡ�м��6�����ݽ���ƽ��
	}
	
	tpad_default_val = temp /6;
	
	printf("tpad_default_val:%d\r\n",tpad_default_val);
	printf("\r\n\r\n");
	
	if(tpad_default_val > TPAD_ARR_MAX_VAL / 2)
	{
		return 1;//��ʼ���������� TPAD_ARR_MAX_VAL/2����ֵ��������1
	}
	return 0;
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
