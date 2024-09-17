/*
*********************************************************************************************************
*
*	模块名称 : 定时器PWM输出驱动模块
*	文件名称 : bsp_tim_pwm.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-29 付燕华  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 变量定义区 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_GetRCCofTIM
*	功能说明: 通过输入定时器名获取时钟
*	形    参：TIM_TypeDef* TIMx:TIM1~TIM8
*	返 回 值: rcc： TIM外设时钟名 
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofTIM( TIM_TypeDef* TIMx)
{
	uint32_t rcc=0;
	//高级定时器类 TIM1
	if(TIMx == TIM1 )
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	//TIM8
	else if(TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	
	//通用定时器类 TIM2
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
	
	//基本定时器 TIM7
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
*	函 数 名: bsp_GetRCCofGPIO
*	功能说明: 通过GPIO的参数获取对应的时钟参数
*	形    参：GPIOx：GPIOA~GPIOG
*	返 回 值: rcc:时钟参数
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
*	函 数 名: 
*	功能说明: 初始化引脚及时钟使能
*	形    参：GPIOx：GPIOA~GPIOG
*			  GPIO_PinX：GPIO_Pin 0~15
*		      TIMx:TIM1~TIM8
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinX,TIM_TypeDef* TIMx)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx),ENABLE);
	//使能定时器时钟
	if( ( TIMx == TIM1 ) || ( TIMx == TIM8 ) )
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	//结构体赋值
	GPIO_InitStructure.GPIO_Pin= GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOx,&GPIO_InitStructure);				//初始化GPIO
}



/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIMforInt
*	功能说明: 设置定时器的定时频率（Hz），抢占优先级和响应优先级
*	形    参：TIMx：TIM1~TIM8
*			  _ulFreq：频率 Hz
*			  _PreemptionPriority,_SubPriority:优先级取值（0~3）中断分租2下
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIMforInt(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority,uint8_t _SubPriority)
{
	//定义定时器结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	uint16_t usPeriod;		//ARR
	uint16_t usPrescaler;	//PSC
	uint32_t uiTIMxCLK;		//时钟频率输入
	
	//打开时钟（选择对应的时钟）
	if( (TIMx == TIM1) || (TIMx == TIM8))
	{
		//APB2总线下
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}else
	{
		//APB1总线下
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	if ((TIMx == TIM1) || (TIMx == TIM8)) 
	 { 
	  /* APB2 定时器 */ 
	  uiTIMxCLK = SystemCoreClock; 
	 } 
	 else /* APB1 定时器 .  */ 
	 { 
	  uiTIMxCLK = SystemCoreClock;  // APB1 clock = SystemCoreClock / 2; 
	 } 
	//频率为0
	if(_ulFreq == 0)
	{
		//定时器关闭
		TIM_Cmd(TIMx,DISABLE);
		{
			/* 关闭TIM定时更新中断*/
			uint8_t irq = 0;
			//定义结构体
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
			
			NVIC_InitStructure.NVIC_IRQChannel= irq;									//中断号
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//抢占
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//响应
			NVIC_InitStructure.NVIC_IRQChannelCmd= DISABLE;								//失能
			NVIC_Init(&NVIC_InitStructure);  	
		}
		return;
	}
	
	
	//输入频率设置
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
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上奇数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//不做时钟分割
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//不适用重复计数器，高级
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//使能ARR重装载影子寄存器
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	//使能更新中断
	TIM_ITConfig(TIMx , TIM_IT_Update,ENABLE);
	
	//使能定时器
	TIM_Cmd(TIMx,ENABLE);
	
	
	/* 配置TIM定时更新中断 (Update) */
	{
		uint8_t irq = 0;
		//定义结构体
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
		
		NVIC_InitStructure.NVIC_IRQChannel= irq;									//中断号
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//抢占
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//响应
		NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;								//使能
		NVIC_Init(&NVIC_InitStructure); 	
	}		
}
/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIMOutPWM
*	功能说明: 设置引脚输出的PWM信号的频率和占空比.  当频率为0，并且占空为0时，关闭定时器，GPIO输出0； 
*     当频率为0，占空比为100%时，GPIO输出1.
*	形    参：GPIOx, GPIO_Pin, TIMx 同库函数结构体 
*    		 _ucChannel:通道1~4 
*   		  _ulFreq : PWM信号频率，单位Hz  (实际测试，最大输出频率为 72M / 4 = 18M）. 0 表示禁止输出 
*    		 _ulDutyCycle : PWM信号占空比，单位：万分之一。如5000，表示50.00%的占空比 (扩大100倍)
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel, 
  uint32_t _ulFreq, uint32_t _ulDutyCycle)
{
	//定义定时器结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//定义比较输出结构体
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	
	uint16_t usPeriod;		//ARR
	uint16_t usPrescaler;	//PSC
	uint32_t uiTIMxCLK;		//时钟频率输入
	
	//初始化时钟,引脚等
	bsp_ConfigTimGpio(GPIOx,GPIO_Pin,TIMx);
	//时钟频率赋值
	uiTIMxCLK = SystemCoreClock;
	
	//计算ARR和PSC的值完成定时器的初始设置
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
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上奇数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//不做时钟分割
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//不适用重复计数器，高级
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//使能ARR重装载影子寄存器
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	//设置比较寄存器
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能信号输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//有效电平是高电平
	TIM_OCInitStructure.TIM_Pulse = (usPeriod*_ulDutyCycle/10000);	//设置CCR的值				
	//高级定时器TIM1和TIM8设置的时候用
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	if(_ucChannel == 1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);     		 /* TIMx通道1初始化 */ 
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* 通道1使能预装载寄存器 */
	}else if(_ucChannel == 2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);    		  /* TIMx通道2初始化 */ 
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* 通道2使能预装载寄存器 */ 
	 } 
	 else if (_ucChannel == 3) 
	 { 
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);     		 /* TIMx通道3初始化 */ 
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* 通道3使能预装载寄存器 */ 
	 } 
	 else if (_ucChannel == 4) 
	 { 
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);     		 /* TIMx通道4初始化 */ 
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);    /* 通道4使能预装载寄存器 */ 
	 } 
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);      				 /* 使能ARR预装载寄存器 */ 
	//使能定时器
	TIM_Cmd(TIMx,ENABLE);
	
	//只有高级定时器才需要使能PWM主输出
	if((TIMx == TIM1)||(TIMx == TIM9))
	{
		TIM_CtrlPWMOutputs(TIMx,ENABLE);
	}
}




///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
