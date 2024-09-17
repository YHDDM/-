/*
*********************************************************************************************************
*
*	ģ������ : ������ʱ��_01����ģ��
*	�ļ����� : time_int.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-29 waroyal  ��ʽ����
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

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRCCofTIM
*	����˵��: ���� TIM �õ�RCC�Ĵ���
*	��    ��: ��
*	�� �� ֵ: TIM ����ʱ����
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIMx)
{
	uint32_t rcc =0 ;
	
	/*
		APB1 ��ʱ���� TM2~TM7
		APB2 ��ʱ���� TM1��TM8
	*/
	
	//�߼���ʱ���� TM1��TIM8
	if(TIMx == TIM1)
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	else if(TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	
	//ͨ�ö�ʱ�� TM2~TM5
	else if(TIMx == TIM2)
	{
		rcc = RCC_APB1Periph_TIM2;
	}
	else if(TIMx == TIM3)
	{
		rcc = RCC_APB1Periph_TIM3;
	}
	else if(TIMx == TIM4)
	{
		rcc = RCC_APB1Periph_TIM4;
	}
	else if(TIMx == TIM5)
	{
		rcc = RCC_APB1Periph_TIM5;		
	}
	
	//������ʱ���� TIM7��TIM6
	else if(TIMx == TIM7)
	{
		rcc = RCC_APB1Periph_TIM7;				
	}
	else if(TIMx == TIM6)
	{
		rcc = RCC_APB1Periph_TIM6;
	}
	return rcc;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRCCofGPIO
*	����˵��: ͨ��GPIO�õ�RCC�Ĵ���
*	��    ��: GPIO_TypeDef* GPIOx
*	�� �� ֵ: ʱ�Ӳ�����GPIOx: GPIOA ~ GPIOG��
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx)
{
	uint32_t rcc = 0;
	if(GPIOx == GPIOA)
	{
		rcc = RCC_APB2Periph_GPIOA;
	}
	else if(GPIOx == GPIOB)
	{
		rcc = RCC_APB2Periph_GPIOB;
	}
	else if(GPIOx == GPIOC)
	{
		rcc = RCC_APB2Periph_GPIOC;
	}
	else if(GPIOx == GPIOD)
	{
		rcc = RCC_APB2Periph_GPIOD;
	}
	else if(GPIOx == GPIOE)
	{
		rcc = RCC_APB2Periph_GPIOE;
	}
	else if(GPIOx == GPIOF)
	{
		rcc = RCC_APB2Periph_GPIOF;
	}
	else if(GPIOx == GPIOG)
	{
		rcc = RCC_APB2Periph_GPIOG;
	}
	return rcc;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_ConfigTimGpio
*	����˵��: ��ʼ�����ż�ʱ��ʹ��
*	��    ��: 
			(GPIO_TypeDef* GPIOx: GPIOA ~ GPIOG)
			(GPIO_PinX: 0~15)
			(TIMx:TIM1~TIM8)
*	�� �� ֵ:�� 
*********************************************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_PinX,TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx),ENABLE);
	
	//ʹ��TIM����ʱ����ʱ��
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	//�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMOutPWM
*	����˵��: ����PWM��������š���ʱ����ͨ����Ƶ�ʡ�ռ�ձ�
*	��    ��: _ucChannel��1~4
			 _ulDutyCycle������100����5000����50%
			 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel,uint32_t _ulFreq,uint32_t _ulDutyCycle)
{
	//���嶨ʱ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//����Ƚ�����ṹ��
	TIM_OCInitTypeDef 		TIM_OCInitStructure;
	
	//ARR
	uint16_t usPeriod;
	//PSC
	uint16_t usPrescaler;
	//ʱ��Ƶ������
	uint32_t uiTIMxCLK;
	
	
	//��ʼ��ʱ�ӡ����ŵ�
	bsp_ConfigTimGpio(GPIOx,GPIO_Pin,TIMx);
	
	//ʱ��Ƶ�ʸ�ֵ
	uiTIMxCLK = SystemCoreClock;
	
	//����ARR��PSC
		if( _ulFreq < 100) //Ƶ��С��100Hz
	{
		usPrescaler = 10000 - 1;							//PSC��Ƶ�� = 10000
		usPeriod = (uiTIMxCLK / 10000)/ _ulFreq - 1;		//ARR�Զ���װ��ֵ
	}
	else if(_ulFreq < 3000)//Ƶ�ʴ��ڵ���100Hz С�� 3KHz
	{
		usPrescaler = 100 -1;								//PSC��Ƶ�� = 100
		usPeriod = (uiTIMxCLK / 100) / _ulFreq - 1;			//ARR�Զ���װ��ֵ	
	}
	else
	{
		usPrescaler = 1 - 1;								//PSC��Ƶ�� = 1
		usPeriod = uiTIMxCLK / _ulFreq - 1;					//ARR�Զ���װ��ֵ
	}
	
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR�����Զ���װ�ؼĴ���
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC����ʱ��Ԥ��Ƶ�Ĵ���
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָTDTS = Tck_tim�����ﲻ��ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ���������ֵ���߼���û�õ�����������
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);				//��ʼ��TIMx
	
	//ʹ��ARRӰ����װ�ؼĴ���
	TIM_ARRPreloadConfig(TIMx,ENABLE);							//ʹ��ARR��װ��Ӱ�ӼĴ���
	
	//ʹ�ܶ�ʱ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ���ź����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	 //������Ч��ƽΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_Pulse = (_ulDutyCycle * usPeriod ) / 10000;	//����CCR��ֵ
	
	
	//�߼���ʱ��
	TIM_OCInitStructure.TIM_OCIdleState	=	TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity	=	TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	//���ñȽϼĴ���
	if(_ucChannel == 1)
	{
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);//ʹ��Ԥ����
		TIM_OC1Init(TIMx,&TIM_OCInitStructure);
	}
	else if(_ucChannel == 2)
	{
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);//ʹ��Ԥ����
		TIM_OC2Init(TIMx,&TIM_OCInitStructure);
		
	}
	else if(_ucChannel == 3)
	{
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);//ʹ��Ԥ����
		TIM_OC3Init(TIMx,&TIM_OCInitStructure);
		
	}
	else if(_ucChannel == 4)
	{
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);//ʹ��Ԥ����
		TIM_OC4Init(TIMx,&TIM_OCInitStructure);
		
	}

	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIMx,ENABLE);
	
	//ֻ�и߼���ʱ������Ҫʹ��PWM�����
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
			TIM_CtrlPWMOutputs(TIMx,ENABLE);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMforInt
*	����˵��: ���ö�ʱ���Ķ�ʱƵ��(Hz)��������ռ����Ӧ���ȼ�
*	��    ��: 
			TIMx��TIM1~8
			_ulFreq��Ƶ�� Hz
			_PreemptionPriority����ռ���ȼ�ֵ0~3���жϷ���2�£�
_			SubPriority����ռ���ȼ�ֵ0~3���жϷ���2�£�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMforInt(TIM_TypeDef* TIMx,uint32_t _ulFreq,uint8_t _PreemptionPriority,uint8_t _SubPriority)
{
	//���嶨ʱ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//ARR
	uint16_t usPeriod;
	//PSC
	uint16_t usPrescaler;
	//ʱ��Ƶ������
	uint32_t uiTIMxCLK;
	
	//ʹ��TIMʱ��
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);//APB2������
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);//APB1������
	}
	
	//�����ʱƵ��Ϊ0
	if(_ulFreq == 0 )
	{
		//��ʱ���ر�
		TIM_Cmd(TIMx,DISABLE);//ʧ�ܶ�ʱ���
		//�ر�TIM��ʱ�����ж�(Update)
		{
			//����ṹ��
			NVIC_InitTypeDef NVIC_InitStructure;
			//�����жϺ�
			uint8_t irq = 0;
			
			if(TIMx == TIM1)
				irq = TIM1_UP_IRQn;
			else if(TIMx == TIM2)
				irq = TIM2_IRQn;
			else if(TIMx == TIM3)
				irq = TIM3_IRQn;
			else if(TIMx == TIM4)
				irq = TIM4_IRQn;
			else if(TIMx == TIM5)
				irq = TIM5_IRQn;
			else if(TIMx == TIM6)
				irq = TIM6_IRQn;
			else if(TIMx == TIM7)
				irq = TIM7_IRQn;
			else if(TIMx == TIM8)
				irq = TIM8_UP_IRQn;
			
			//NVIC�ж����ȼ�����
			NVIC_InitStructure.NVIC_IRQChannel = irq;									//�жϺ�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//��Ӧ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;							//ʧ��
			NVIC_Init(&NVIC_InitStructure);												//��ʼ��NVIC�Ĵ���
		}
		return;
	}		
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		//APB2��ʱ��
		uiTIMxCLK = SystemCoreClock;
	}
	else
	{
		//APB1��ʱ��
		uiTIMxCLK = SystemCoreClock; //APB1 clock = SystemCoreClock / 2;
	}
	
	if( _ulFreq < 100) //Ƶ��С��100Hz
	{
		usPrescaler = 10000 - 1;							//PSC��Ƶ�� = 10000
		usPeriod = (uiTIMxCLK / 10000)/ _ulFreq - 1;		//ARR�Զ���װ��ֵ
	}
	else if(_ulFreq < 3000)//Ƶ�ʴ��ڵ���100Hz С�� 3KHz
	{
		usPrescaler = 100 -1;								//PSC��Ƶ�� = 100
		usPeriod = (uiTIMxCLK / 100) / _ulFreq - 1;			//ARR�Զ���װ��ֵ	
	}
	else
	{
		usPrescaler = 1 - 1;								//PSC��Ƶ�� = 1
		usPeriod = uiTIMxCLK / _ulFreq - 1;					//ARR�Զ���װ��ֵ
	}
	
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR�����Զ���װ�ؼĴ���
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC����ʱ��Ԥ��Ƶ�Ĵ���
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָTDTS = Tck_tim�����ﲻ��ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ���������ֵ���߼���û�õ�����������
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);				//��ʼ��TIMx
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);							//ʹ��ARR��װ��Ӱ�ӼĴ���
	
	//TIM Interrupts enable
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);					//ʹ��TIMx�����ж�
	//TIM counter enable
	TIM_Cmd(TIMx,ENABLE);										//ʹ�ܶ�ʱ��TIMx
	
	//����TIM��ʱ�������ж�(Update)
	{
		//����ṹ��
		NVIC_InitTypeDef NVIC_InitStructure;
		//�����жϺ�
		uint8_t irq = 0;
		
		if(TIMx == TIM1)
			irq = TIM1_UP_IRQn;
		else if(TIMx == TIM2)
			irq = TIM2_IRQn;
		else if(TIMx == TIM3)
			irq = TIM3_IRQn;
		else if(TIMx == TIM4)
			irq = TIM4_IRQn;
		else if(TIMx == TIM5)
			irq = TIM5_IRQn;
		else if(TIMx == TIM6)
			irq = TIM6_IRQn;
		else if(TIMx == TIM7)
			irq = TIM7_IRQn;
		else if(TIMx == TIM8)
			irq = TIM8_UP_IRQn;

		//NVIC�ж����ȼ�����
		NVIC_InitStructure.NVIC_IRQChannel = irq;									//�жϺ�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//��Ӧ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʧ��
		NVIC_Init(&NVIC_InitStructure);												//��ʼ��NVIC�Ĵ���
	}
}
///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
