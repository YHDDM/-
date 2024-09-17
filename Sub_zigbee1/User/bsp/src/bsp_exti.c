/*
*********************************************************************************************************
*
*	模块名称 : 中断驱动模块
*	文件名称 : bsp_exti.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-3-12   SDZ  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
#include "bsp_exti.h"
///////////////////////////////////////
/* 变量定义区 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void KEY_IO_Init(void);
void EXTI_KEY_Init(void);
///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyEXTI
*	功能说明: 配置中断按键
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKeyEXTI(void)
{
	KEY_IO_Init();
	EXTI_KEY_Init();
}
/*
*********************************************************************************************************
*	函 数 名: KEY_IO_Init
*	功能说明: 配置按键对应的GPIO
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void KEY_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*第一步：打开GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	/*关闭jtag，使能SWD，可以用SWD模式调试*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/*第二步：配置所有的按键GPIO*/
	GPIO_InitStructure.GPIO_Pin = WKUP_GPIO_PIN;   /*WKUP端口*/
	GPIO_InitStructure.GPIO_Mode = WKUP_GPIO_MODE; /*WKUP端口模式*/
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure); /*初始化WKUP*/
	
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;   /*KEY0端口*/
	GPIO_InitStructure.GPIO_Mode = KEY0_GPIO_MODE; /*KEY0端口模式*/
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure); /*初始化KEY0*/
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;   /*KEY1端口*/
	GPIO_InitStructure.GPIO_Mode = KEY1_GPIO_MODE; /*KEY1端口模式*/
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure); /*初始化KEY1*/
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;   /*KEY2端口*/
	GPIO_InitStructure.GPIO_Mode = KEY2_GPIO_MODE; /*KEY2端口模式*/
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure); /*初始化KEY2*/
}
/*
*********************************************************************************************************
*	函 数 名: EXTI_KEY_Init
*	功能说明: 将所有的按键配置成外部中断触发方式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI_KEY_Init(void)
{
	//1.定义结构体
	
	//定义NVIC结构体
	NVIC_InitTypeDef NVIC_Initstructure;
	//定义EXTI结构体
	EXTI_InitTypeDef EXTI_Initstructure;
	
	//2.打开时钟
	
	//AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//3.中断线映射
	//WEKP(PA0)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	//K2(PE2)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	//K1(PE3)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	//K0(PE4)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	
	//WKUP(PA0)--------------
	//4.1外部中断的结构体
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line0;					//中断线EXTI_Line0
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//模式是中断
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Rising;		//触发方式上升沿
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//使能
	//4.2外部中断结构体写入寄存器
	EXTI_Init(&EXTI_Initstructure);								//写入寄存器配置
	//K2(PE2)--------------
	//4.1外部中断的结构体
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line2;					//中断线EXTI_Line2
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//模式是中断
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//触发方式下降沿
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//使能
	//4.2外部中断结构体写入寄存器
	EXTI_Init(&EXTI_Initstructure);								//写入寄存器配置
	//K1(PE3)--------------
	//4.1外部中断的结构体
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line3;					//中断线EXTI_Line3
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//模式是中断
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//触发方式下降沿
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//使能
	//4.2外部中断结构体写入寄存器
	EXTI_Init(&EXTI_Initstructure);								//写入寄存器配置
	//K0(PE4)--------------
	//4.1外部中断的结构体
	//EXTI
	EXTI_Initstructure.EXTI_Line = EXTI_Line4;					//中断线EXTI_Line4
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;			//模式是中断
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;		//触发方式下降沿
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;					//使能
	//4.2外部中断结构体写入寄存器
	EXTI_Init(&EXTI_Initstructure);								//写入寄存器配置
	
	
	//------------------------------------------------------------------------------------------
	//WKUP(PA0)--------------
	//5.1外部中断的结构体
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI0_IRQn;   			//中断号IRQN
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//响应优先级2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//使能
	//5.2NVIC_Init()写入寄存器
	NVIC_Init(&NVIC_Initstructure);								//写入寄存器配置-----
	//K0(PE4)--------------
	//5.1外部中断的结构体
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI4_IRQn;   			//中断号IRQN
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//响应优先级2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//使能
	//5.2NVIC_Init()写入寄存器
	NVIC_Init(&NVIC_Initstructure);								//写入寄存器配置
	//K2(PE2)--------------
	//5.1外部中断的结构体
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI2_IRQn;   			//中断标号
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//响应优先级2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//使能
	//5.2NVIC_Init()写入寄存器
	NVIC_Init(&NVIC_Initstructure);								//写入寄存器配置
	//K1(PE3)--------------
	//5.1外部中断的结构体
	//NVIC
	NVIC_Initstructure.NVIC_IRQChannel = EXTI3_IRQn;   			//中断标号
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 2;			//响应优先级2
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;				//使能
	//5.2NVIC_Init()写入寄存器
	NVIC_Init(&NVIC_Initstructure);								//写入寄存器配置
}

/*
*********************************************************************************************************
*	函 数 名: EXTI0_IRQHandler
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 中断产生标志变量
*********************************************************************************************************
*/
//KEUP
void EXTI0_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //产生中断线标志
	{
		if(WKUP == WKUP_ACTIVE_LEVEL)
		{
			bsp_BeepToggle();
		}
		EXTI_ClearITPendingBit(EXTI_Line0);			//清除中断线中断标志
	}
}

/*
*********************************************************************************************************
*	函 数 名: EXTI4_IRQHandler
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 中断产生标志变量
*********************************************************************************************************
*/
//KEY0
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //产生中断线标志
	{
		if(KEY0 == KEY0_ACTIVE_LEVEL)
		{
			bsp_LedToggle(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);			//清除中断线中断标志
	}
}
/*
*********************************************************************************************************
*	函 数 名: EXTI3_IRQHandler
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 中断产生标志变量
*********************************************************************************************************
*/
//KEY1
void EXTI3_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //产生中断线标志
	{
		if(KEY1 == KEY1_ACTIVE_LEVEL)//是有效电平
		{
			bsp_LedToggle(2);
		}		
		EXTI_ClearITPendingBit(EXTI_Line3);			//清除中断线中断标志
	}
}
/*
*********************************************************************************************************
*	函 数 名: EXTI2_IRQHandler
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 中断产生标志变量
*********************************************************************************************************
*/
//KEY2
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //产生中断线标志
	{
		if(KEY2 == KEY2_ACTIVE_LEVEL)//是有效电平
		{
			bsp_LedToggle(1);
			bsp_LedToggle(2);
		}
		EXTI_ClearITPendingBit(EXTI_Line2);			//清除中断线中断标志
	}
}


