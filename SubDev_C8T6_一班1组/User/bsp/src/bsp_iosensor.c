/*
*********************************************************************************************************
*
*	模块名称 : IO驱动模块
*	文件名称 : bsp_iosensor.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-05-29 付燕华  正式发布
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
uint8_t VIBRATE_OUT_flag = 0; //震动传感器触发标志 
uint8_t PIR_OUT_flag = 0;  //人体红外传感器触发标志
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void SENSOR_IO_Init(void); //IO初始化 
void EXTI_Sensor_Init(void);//中断初始化
///////////////////////////////////////
/* 函数实体区 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitIoSensor
*	功能说明: 高低电平型传感器初始化函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitIoSensor(void) 
{ 
	SENSOR_IO_Init(); 
	EXTI_Sensor_Init(); 
}
/*
*********************************************************************************************************
*	函 数 名: SENSOR_IO_Init
*	功能说明: 高低电平型传感器初始化函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void SENSOR_IO_Init(void) //IO初始化 
{  
	GPIO_InitTypeDef GPIO_InitStructure; 
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //使能复用功能时钟 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试 
  
	RCC_APB2PeriphClockCmd(VIBRATE_OUT_GPIO_CLK,ENABLE); //使能VIBRATE_OUT时钟 
	//初始化 VIBRATE_OUT    
	GPIO_InitStructure.GPIO_Pin  = VIBRATE_OUT_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = VIBRATE_OUT_GPIO_MODE; //设置成输入，默认上拉    
	GPIO_Init(VIBRATE_OUT_GPIO_PORT, &GPIO_InitStructure); //初始化VIBRATE 


	RCC_APB2PeriphClockCmd(PIR_OUT_GPIO_CLK,ENABLE);  //使能PIR_OUT时钟 
	//初始化 PIR_OUT    
	GPIO_InitStructure.GPIO_Pin  = PIR_OUT_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = PIR_OUT_GPIO_MODE;  //设置成输入，默认下拉    
	GPIO_Init(PIR_OUT_GPIO_PORT, &GPIO_InitStructure);  //初始化PIR 
} 

/*
*********************************************************************************************************
*	函 数 名: EXTI_Sensor_Init 
*	功能说明: 高低电平型传感器外部中断初始化函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI_Sensor_Init(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，使能SWD，可以用SWD模式调试 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  		//使能复用功能时钟 
	
	//VIBRATE中断线以及中断初始化配置  
	GPIO_EXTILineConfig(VIBRATE_OUT_PORT_SRC,VIBRATE_OUT_PIN_SRC); 
	
	EXTI_InitStructure.EXTI_Line = VIBRATE_OUT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
	EXTI_InitStructure.EXTI_Trigger = VIBRATE_OUT_EXTI_TRIG; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//////////////////////////////
	
	NVIC_InitStructure.NVIC_IRQChannel = VIBRATE_OUT_EXTI_IRQN; 	//使能VIBRATE所在的外部中断通道 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  	//抢占优先级2  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;  			//子优先级2  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  				//使能外部中断通道 
	NVIC_Init(&NVIC_InitStructure);     	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	//PIR中断线以及中断初始化配置  
	GPIO_EXTILineConfig(PIR_OUT_PORT_SRC,PIR_OUT_PIN_SRC);  
	
	EXTI_InitStructure.EXTI_Line = PIR_OUT_EXTI_LINE; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
	EXTI_InitStructure.EXTI_Trigger = PIR_OUT_EXTI_TRIG;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
/////////////////////////////////////////////////////////////
	NVIC_InitStructure.NVIC_IRQChannel = PIR_OUT_EXTI_IRQN;  //使能GAS所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //抢占优先级2  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;   //子优先级2  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能外部中断通道 
	NVIC_Init(&NVIC_InitStructure);     	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

}

/*
*********************************************************************************************************
*	函 数 名: EXTI15_10_IRQHandler 
*	功能说明: 外部中断15-10服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI15_10_IRQHandler(void) 
{
	if(EXTI_GetITStatus(VIBRATE_OUT_EXTI_LINE)!=RESET)
	{
		if(VIBRATE_OUT==VIBRATE_OUT_ACTIVE_LEVEL) //VIBRATE_OUT动作为低电平
		{
			VIBRATE_OUT_flag=1;	//标志置位，便于主函数轮询 
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(VIBRATE_OUT_EXTI_LINE);
	}
//	if(EXTI_GetITStatus(PIR_OUT_EXTI_LINE)!=RESET)
//	{
//		if(PIR_OUT==PIR_OUT_ACTIVE_LEVEL)//PIR_OUT动作为高电平 
//		{
//			PIR_OUT_flag=1;//标志置位，便于主函数轮询 
//		}
//		//中断处理，自行添加
//		EXTI_ClearITPendingBit(PIR_OUT_EXTI_LINE); 
//	}
}


/*
*********************************************************************************************************
*	函 数 名: EXTI15_10_IRQHandler 
*	功能说明: 外部中断15-10服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI3_IRQHandler(void) 
{
//	if(EXTI_GetITStatus(VIBRATE_OUT_EXTI_LINE)!=RESET)
//	{
//		if(VIBRATE_OUT==VIBRATE_OUT_ACTIVE_LEVEL) //VIBRATE_OUT动作为低电平
//		{
//			VIBRATE_OUT_flag=1;	//标志置位，便于主函数轮询 
//		}
//		//中断处理，自行添加
//		EXTI_ClearITPendingBit(VIBRATE_OUT_EXTI_LINE);
//	}
	if(EXTI_GetITStatus(PIR_OUT_EXTI_LINE)!=RESET)
	{
		if(PIR_OUT==PIR_OUT_ACTIVE_LEVEL)//PIR_OUT动作为高电平 
		{
			PIR_OUT_flag=1;//标志置位，便于主函数轮询 
		}
		//中断处理，自行添加
		EXTI_ClearITPendingBit(PIR_OUT_EXTI_LINE); 
	}
}


///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
