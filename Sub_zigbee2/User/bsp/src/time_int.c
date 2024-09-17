/*
*********************************************************************************************************
*
*	模块名称 : 基本定时器_01驱动模块
*	文件名称 : time_int.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-29 waroyal  正式发布
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
*	功能说明: 根据 TIM 得到RCC寄存器
*	形    参: 无
*	返 回 值: TIM 外设时钟名
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIMx)
{
	uint32_t rcc =0 ;
	
	/*
		APB1 定时器有 TM2~TM7
		APB2 定时器有 TM1、TM8
	*/
	
	//高级定时器类 TM1、TIM8
	if(TIMx == TIM1)
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	else if(TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	
	//通用定时器 TM2~TM5
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
	
	//基本定时器类 TIM7、TIM6
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
*	函 数 名: bsp_GetRCCofGPIO
*	功能说明: 通过GPIO得到RCC寄存器
*	形    参: GPIO_TypeDef* GPIOx
*	返 回 值: 时钟参数（GPIOx: GPIOA ~ GPIOG）
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
*	函 数 名: bsp_ConfigTimGpio
*	功能说明: 初始化引脚及时钟使能
*	形    参: 
			(GPIO_TypeDef* GPIOx: GPIOA ~ GPIOG)
			(GPIO_PinX: 0~15)
			(TIMx:TIM1~TIM8)
*	返 回 值:无 
*********************************************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_PinX,TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx),ENABLE);
	
	//使能TIM（定时器）时钟
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);
	}
	
	//结构体赋值
	GPIO_InitStructure.GPIO_Pin = GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIMOutPWM
*	功能说明: 设置PWM输出、引脚、定时器、通道、频率、占空比
*	形    参: _ucChannel：1~4
			 _ulDutyCycle：扩大100倍，5000代表50%
			 
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t _ucChannel,uint32_t _ulFreq,uint32_t _ulDutyCycle)
{
	//定义定时器结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//定义比较输出结构体
	TIM_OCInitTypeDef 		TIM_OCInitStructure;
	
	//ARR
	uint16_t usPeriod;
	//PSC
	uint16_t usPrescaler;
	//时钟频率输入
	uint32_t uiTIMxCLK;
	
	
	//初始化时钟、引脚等
	bsp_ConfigTimGpio(GPIOx,GPIO_Pin,TIMx);
	
	//时钟频率赋值
	uiTIMxCLK = SystemCoreClock;
	
	//计算ARR和PSC
		if( _ulFreq < 100) //频率小于100Hz
	{
		usPrescaler = 10000 - 1;							//PSC分频比 = 10000
		usPeriod = (uiTIMxCLK / 10000)/ _ulFreq - 1;		//ARR自动重装的值
	}
	else if(_ulFreq < 3000)//频率大于等于100Hz 小于 3KHz
	{
		usPrescaler = 100 -1;								//PSC分频比 = 100
		usPeriod = (uiTIMxCLK / 100) / _ulFreq - 1;			//ARR自动重装的值	
	}
	else
	{
		usPrescaler = 1 - 1;								//PSC分频比 = 1
		usPeriod = uiTIMxCLK / _ulFreq - 1;					//ARR自动重装的值
	}
	
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR设置自动重装载寄存器
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC设置时钟预分频寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割，TDTS = Tck_tim，这里不做时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数器的值，高级的没用到，不用设置
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);				//初始化TIMx
	
	//使能ARR影子重装载寄存器
	TIM_ARRPreloadConfig(TIMx,ENABLE);							//使能ARR重装载影子寄存器
	
	//使能定时器
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能信号输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	 //设置有效电平为高电平
	TIM_OCInitStructure.TIM_Pulse = (_ulDutyCycle * usPeriod ) / 10000;	//设置CCR的值
	
	
	//高级定时器
	TIM_OCInitStructure.TIM_OCIdleState	=	TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity	=	TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	//设置比较寄存器
	if(_ucChannel == 1)
	{
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);//使能预设置
		TIM_OC1Init(TIMx,&TIM_OCInitStructure);
	}
	else if(_ucChannel == 2)
	{
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);//使能预设置
		TIM_OC2Init(TIMx,&TIM_OCInitStructure);
		
	}
	else if(_ucChannel == 3)
	{
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);//使能预设置
		TIM_OC3Init(TIMx,&TIM_OCInitStructure);
		
	}
	else if(_ucChannel == 4)
	{
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);//使能预设置
		TIM_OC4Init(TIMx,&TIM_OCInitStructure);
		
	}

	//使能定时器
	TIM_Cmd(TIMx,ENABLE);
	
	//只有高级定时器才需要使能PWM主输出
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
			TIM_CtrlPWMOutputs(TIMx,ENABLE);
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIMforInt
*	功能说明: 设置定时器的定时频率(Hz)，设置抢占和响应优先级
*	形    参: 
			TIMx：TIM1~8
			_ulFreq：频率 Hz
			_PreemptionPriority：抢占优先级值0~3（中断分组2下）
_			SubPriority：抢占优先级值0~3（中断分组2下）
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIMforInt(TIM_TypeDef* TIMx,uint32_t _ulFreq,uint8_t _PreemptionPriority,uint8_t _SubPriority)
{
	//定义定时器结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//ARR
	uint16_t usPeriod;
	//PSC
	uint16_t usPrescaler;
	//时钟频率输入
	uint32_t uiTIMxCLK;
	
	//使能TIM时钟
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);//APB2总线下
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx),ENABLE);//APB1总线下
	}
	
	//如果定时频率为0
	if(_ulFreq == 0 )
	{
		//定时器关闭
		TIM_Cmd(TIMx,DISABLE);//失能定时输出
		//关闭TIM定时更新中断(Update)
		{
			//定义结构体
			NVIC_InitTypeDef NVIC_InitStructure;
			//定义中断号
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
			
			//NVIC中断优先级设置
			NVIC_InitStructure.NVIC_IRQChannel = irq;									//中断号
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//响应优先级
			NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;							//失能
			NVIC_Init(&NVIC_InitStructure);												//初始化NVIC寄存器
		}
		return;
	}		
	if((TIMx == TIM1) || (TIMx == TIM8))
	{
		//APB2定时器
		uiTIMxCLK = SystemCoreClock;
	}
	else
	{
		//APB1定时器
		uiTIMxCLK = SystemCoreClock; //APB1 clock = SystemCoreClock / 2;
	}
	
	if( _ulFreq < 100) //频率小于100Hz
	{
		usPrescaler = 10000 - 1;							//PSC分频比 = 10000
		usPeriod = (uiTIMxCLK / 10000)/ _ulFreq - 1;		//ARR自动重装的值
	}
	else if(_ulFreq < 3000)//频率大于等于100Hz 小于 3KHz
	{
		usPrescaler = 100 -1;								//PSC分频比 = 100
		usPeriod = (uiTIMxCLK / 100) / _ulFreq - 1;			//ARR自动重装的值	
	}
	else
	{
		usPrescaler = 1 - 1;								//PSC分频比 = 1
		usPeriod = uiTIMxCLK / _ulFreq - 1;					//ARR自动重装的值
	}
	
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = usPeriod;				//ARR设置自动重装载寄存器
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;			//PSC设置时钟预分频寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割，TDTS = Tck_tim，这里不做时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数器的值，高级的没用到，不用设置
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);				//初始化TIMx
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);							//使能ARR重装载影子寄存器
	
	//TIM Interrupts enable
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);					//使能TIMx更新中断
	//TIM counter enable
	TIM_Cmd(TIMx,ENABLE);										//使能定时器TIMx
	
	//配置TIM定时器更新中断(Update)
	{
		//定义结构体
		NVIC_InitTypeDef NVIC_InitStructure;
		//定义中断号
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

		//NVIC中断优先级设置
		NVIC_InitStructure.NVIC_IRQChannel = irq;									//中断号
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;	//抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;				//响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//失能
		NVIC_Init(&NVIC_InitStructure);												//初始化NVIC寄存器
	}
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
