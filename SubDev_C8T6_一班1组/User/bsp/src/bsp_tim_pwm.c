/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��PWM�������ģ��
*	�ļ����� : bsp_tim_pwm.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-29 ���໪  ��ʽ����
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
*	����˵��: ͨ�����붨ʱ������ȡʱ��
*	��    �Σ�TIM_TypeDef* TIMx:TIM1~TIM8
*	�� �� ֵ: rcc�� TIM����ʱ���� 
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofTIM( TIM_TypeDef* TIMx)
{
	uint32_t rcc=0;
	//�߼���ʱ���� TIM1
	if(TIMx == TIM1 )
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	//TIM8
	else if(TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	
	//ͨ�ö�ʱ���� TIM2
	else if(TIMx == TIM2)
	{
		rcc = RCC_APB1Periph_TIM2;
	}
	//TIM3
	else if(TIMx == TIM3)
	{
		rcc = RCC_APB1Periph_TIM3;
	}
	//TIM4
	else if(TIMx == TIM4)
	{
		rcc = RCC_APB1Periph_TIM4;
	}
	//TIM5
	else if(TIMx == TIM5)
	{
		rcc = RCC_APB1Periph_TIM5;
	}
	
	//������ʱ�� TIM7
	else if(TIMx == TIM7)
	{
		rcc = RCC_APB1Periph_TIM7;
	}
	//TIM6
	else if(TIMx == TIM6)
	{
		rcc = RCC_APB1Periph_TIM6;
	}
	return rcc;

}


/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRCCofGPIO
*	����˵��: ͨ��GPIO�Ĳ�����ȡ��Ӧ��ʱ�Ӳ���
*	��    �Σ�GPIOx��GPIOA~GPIOG
*	�� �� ֵ: rcc:ʱ�Ӳ���
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofGPIO( GPIO_TypeDef* GPIOx)
{
	uint32_t rcc = 0;
	if(GPIOx == GPIOA)
	{
		rcc = RCC_APB2Periph_GPIOA;
	}else if(GPIOx == GPIOB)
	{
		rcc = RCC_APB2Periph_GPIOB;
	}else if(GPIOx == GPIOC)
	{
		rcc = RCC_APB2Periph_GPIOC;
	}else if(GPIOx == GPIOD)
	{
		rcc = RCC_APB2Periph_GPIOD;
	}else if(GPIOx == GPIOE)
	{
		rcc = RCC_APB2Periph_GPIOE;
	}else if(GPIOx == GPIOF)
	{
		rcc = RCC_APB2Periph_GPIOF;
	}else if(GPIOx == GPIOG)
	{
		rcc = RCC_APB2Periph_GPIOG;
	}
	return rcc;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ�����ż�ʱ��ʹ��
*	��    �Σ�GPIOx��GPIOA~GPIOG
*			  GPIO_PinX��GPIO_Pin 0~15
*		      TIMx:TIM1~TIM8
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinX,TIM_TypeDef* TIMx)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx),ENABLE);
	//ʹ�ܶ�ʱ��ʱ��
	if( ( TIMx == TIM1 ) || ( TIMx == TIM8 ) )
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	//�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin= GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOx,&GPIO_InitStructure);				//��ʼ��GPIO
}



/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMforInt
*	����˵��: ���ö�ʱ���Ķ�ʱƵ�ʣ�Hz������ռ���ȼ�����Ӧ���ȼ�
*	��    �Σ�TIMx��TIM1~TIM8
*			  _ulFreq��Ƶ�� Hz
*			  _PreemptionPriority,_SubPriority:���ȼ�ȡֵ��0~3���жϷ���2��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMforInt(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority,uint8_t _SubPriority)
{
	//���嶨ʱ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	uint16_t usPeriod;		//ARR
	uint16_t usPrescaler;	//PSC
	uint32_t uiTIMxCLK;		//ʱ��Ƶ������
	
	//��ʱ�ӣ�ѡ���Ӧ��ʱ�ӣ�
	if( (TIMx == TIM1) || (TIMx == TIM8))
	{
		//APB2������
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}else
	{
		//APB1������
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	if ((TIMx == TIM1) || (TIMx == TIM8)) 
	 { 
	  /* APB2 ��ʱ�� */ 
	  uiTIMxCLK = SystemCoreClock; 
	 } 
	 else /* APB1 ��ʱ�� .  */ 
	 { 
	  uiTIMxCLK = SystemCoreClock;  // APB1 clock = SystemCoreClock / 2; 
	 } 
	//Ƶ��Ϊ0
	if(_ulFreq == 0)
	{
		//��ʱ���ر�
		TIM_Cmd(TIMx,DISABLE);
		{
			/* �ر�TIM��ʱ�����ж�*/
			uint8_t irq = 0;
			//����ṹ��
			NVIC_InitTypeDef NVIC_InitStructure;
			if(TIMx == TIM1)
			{
				irq = TIM1_UP_IRQn;
			}
			else if(TIMx == TIM8)
			{
				irq = TIM8_UP_IRQn;
			}
			else if(TIMx == TIM7)
			{
				irq = TIM7_IRQn;
			}
			else if(TIMx == TIM2)
			{
				irq = TIM2_IRQn;
			}
			else if(TIMx == TIM3)
			{
				irq = TIM3_IRQn;
			}
			else if(TIMx == TIM4)
			{
				irq = TIM4_IRQn;
			}
			else if(TIMx == TIM5)
			{
				irq = TIM5_IRQn;
			}
			else if(TIMx == TIM6)
			{
				irq = TIM6_IRQn;
			}
			
			NVIC_InitStructure.NVIC_IRQChannel= irq;									//�жϺ�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//��ռ
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//��Ӧ
			NVIC_InitStructure.NVIC_IRQChannelCmd= DISABLE;								//ʧ��
			NVIC_Init(&NVIC_InitStructure);  	
		}
		return;
	}
	
	
	//����Ƶ������
	uiTIMxCLK = SystemCoreClock;
	
	if(_ulFreq < 100)
	{
		usPrescaler = 10000-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	else if( _ulFreq < 3000)
	{
		usPrescaler = 100-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	else 
	{
		usPrescaler = 1-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//��������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�������ظ����������߼�
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//ʹ��ARR��װ��Ӱ�ӼĴ���
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	//ʹ�ܸ����ж�
	TIM_ITConfig(TIMx , TIM_IT_Update,ENABLE);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIMx,ENABLE);
	
	
	/* ����TIM��ʱ�����ж� (Update) */
	{
		uint8_t irq = 0;
		//����ṹ��
		NVIC_InitTypeDef NVIC_InitStructure;
		if(TIMx == TIM1)
		{
			irq = TIM1_UP_IRQn;
		}
		else if(TIMx == TIM8)
		{
			irq = TIM8_UP_IRQn;
		}
		else if(TIMx == TIM7)
		{
			irq = TIM7_IRQn;
		}
		else if(TIMx == TIM2)
		{
			irq = TIM2_IRQn;
		}
		else if(TIMx == TIM3)
		{
			irq = TIM3_IRQn;
		}
		else if(TIMx == TIM4)
		{
			irq = TIM4_IRQn;
		}
		else if(TIMx == TIM5)
		{
			irq = TIM5_IRQn;
		}
		else if(TIMx == TIM6)
		{
			irq = TIM6_IRQn;
		}
		
		NVIC_InitStructure.NVIC_IRQChannel= irq;									//�жϺ�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//��ռ
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//��Ӧ
		NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;								//ʹ��
		NVIC_Init(&NVIC_InitStructure); 	
	}		
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMOutPWM
*	����˵��: �������������PWM�źŵ�Ƶ�ʺ�ռ�ձ�.  ��Ƶ��Ϊ0������ռ��Ϊ0ʱ���رն�ʱ����GPIO���0�� 
*     ��Ƶ��Ϊ0��ռ�ձ�Ϊ100%ʱ��GPIO���1.
*	��    �Σ�GPIOx, GPIO_Pin, TIMx ͬ�⺯���ṹ�� 
*    		 _ucChannel:ͨ��1~4 
*   		  _ulFreq : PWM�ź�Ƶ�ʣ���λHz  (ʵ�ʲ��ԣ�������Ƶ��Ϊ 72M / 4 = 18M��. 0 ��ʾ��ֹ��� 
*    		 _ulDutyCycle : PWM�ź�ռ�ձȣ���λ�����֮һ����5000����ʾ50.00%��ռ�ձ� (����100��)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel, 
  uint32_t _ulFreq, uint32_t _ulDutyCycle)
{
	//���嶨ʱ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//����Ƚ�����ṹ��
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	
	uint16_t usPeriod;		//ARR
	uint16_t usPrescaler;	//PSC
	uint32_t uiTIMxCLK;		//ʱ��Ƶ������
	
	//��ʼ��ʱ��,���ŵ�
	bsp_ConfigTimGpio(GPIOx,GPIO_Pin,TIMx);
	//ʱ��Ƶ�ʸ�ֵ
	uiTIMxCLK = SystemCoreClock;
	
	//����ARR��PSC��ֵ��ɶ�ʱ���ĳ�ʼ����
	if(_ulFreq < 100)
	{
		usPrescaler = 10000-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	else if( _ulFreq < 3000)
	{
		usPrescaler = 100-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	else 
	{
		usPrescaler = 1-1;									//PSC
		usPeriod = uiTIMxCLK / _ulFreq / (usPrescaler + 1) - 1;	//ARR
	}
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//��������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�������ظ����������߼�
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//ʹ��ARR��װ��Ӱ�ӼĴ���
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	//���ñȽϼĴ���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ���ź����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//��Ч��ƽ�Ǹߵ�ƽ
	TIM_OCInitStructure.TIM_Pulse = (usPeriod*_ulDutyCycle/10000);	//����CCR��ֵ				
	//�߼���ʱ��TIM1��TIM8���õ�ʱ����
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	if(_ucChannel == 1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);     		 /* TIMxͨ��1��ʼ�� */ 
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* ͨ��1ʹ��Ԥװ�ؼĴ��� */
	}else if(_ucChannel == 2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);    		  /* TIMxͨ��2��ʼ�� */ 
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* ͨ��2ʹ��Ԥװ�ؼĴ��� */ 
	 } 
	 else if (_ucChannel == 3) 
	 { 
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);     		 /* TIMxͨ��3��ʼ�� */ 
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* ͨ��3ʹ��Ԥװ�ؼĴ��� */ 
	 } 
	 else if (_ucChannel == 4) 
	 { 
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);     		 /* TIMxͨ��4��ʼ�� */ 
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* ͨ��4ʹ��Ԥװ�ؼĴ��� */ 
	 } 
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);      				 /* ʹ��ARRԤװ�ؼĴ��� */ 
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIMx,ENABLE);
	
	//ֻ�и߼���ʱ������Ҫʹ��PWM�����
	if((TIMx == TIM1)||(TIMx == TIM9))
	{
		TIM_CtrlPWMOutputs(TIMx,ENABLE);
	}
}




///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
