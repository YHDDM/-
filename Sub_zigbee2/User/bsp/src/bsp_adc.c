/*
*********************************************************************************************************
*
*	模块名称 : 数模转换驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-04-10 Lawrence  正式发布
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
*	函 数 名: bsp_InitAdc
*	功能说明:  ADCx(ADC1 - ADC3)  ，  ADC_Channel(0-17) ， ADC_SampleTime(1.5周期自己补全)
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
	//定义结构体
	ADC_InitTypeDef 	ADC_InitStructure;
	
	//使能时钟
	if(ADCx == ADC1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	//使能ADC1通道时钟
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC 复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束
		//ADC_DeInit(ADC1);//等价于上面两个
	}
	else if(ADCx == ADC2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);	//使能ADC2通道时钟
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC 复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);//复位结束
		//ADC_DeInit(ADC2);//等价于上面两个
	}
	else if(ADCx == ADC3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);	//使能ADC3通道时钟
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC 复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);//复位结束
		//ADC_DeInit(ADC3);//等价于上面两个
	}
	
	//结构体赋值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC工作模#式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//右对齐模式  12位
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//顺序进行规则转换的ADC通道数目				
	ADC_Init(ADCx,&ADC_InitStructure);									//ADCx初始化寄存器
	
	//规则采样通道设置
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime);
	
	//使能ADC
	ADC_Cmd(ADCx,ENABLE);
	//校准复位+等待复位完成
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));							//等待复位校准结束
	//启动校准
	ADC_StartCalibration(ADCx);
	//等待校准完成
	while(ADC_GetCalibrationStatus(ADCx));								//等待校准结束
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc
*	功能说明: 获取ADC值。这里仅以规则通道为例，仅供函数内部调用
*	形    参: ADC_TypeDef* ADCx  ,具体哪个ADC
*	返 回 值: 转换的结果
*********************************************************************************************************
*/
static uint16_t Get_Adc(ADC_TypeDef* ADCx)
{
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);								//使能指定的ADCx的软件启动功能
	
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) == RESET);				//等待转换结束
	
	return ADC_GetConversionValue(ADCx);								//返回最近一次ADCx规则组的转换结果
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc_Average
*	功能说明: 获得ADC值，这里仅以规则通道为例
*	形    参: ADC_TypeDef* ADCx 具体是哪个ADC ，uint8_t times多次采样求平均值
*	返 回 值: 无
*********************************************************************************************************
*/
static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times)
{
	u32 temp_val = 0;
	u8 t;
	
	for(t = 0;t < times;t++)
	{
		temp_val += Get_Adc(ADCx);
	}
	return temp_val/times;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAdcPotentiometer
*	功能说明: 初始化电位器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAdcPotentiometer(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RES_POT_GPIO_CLK,ENABLE);
	
	/*配置RES_POT 位模拟输入*/
	GPIO_InitStructure.GPIO_Pin = RES_POT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(RES_POT_GPIO_PORT,&GPIO_InitStructure);
	
	/*配置RES_POT的ADC参数*/
	bsp_InitAdc(RES_POT_ADC_X,RES_POT_ADC_CHANNEL,RES_POT_ADC_SAMPLETIME);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc_Potentiometer
*	功能说明: 得到电位器的电压值
*	形    参: 无
*	返 回 值: 返回值电位器电压值，均值采样
*********************************************************************************************************
*/
uint16_t Get_Adc_Potentiometer(void)
{
	uint16_t adcx;
	adcx = Get_Adc_Average(RES_POT_ADC_X,SAMP_COUNT);			//SAMP_COUNT次取平均
	return adcx;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAdcTemp
*	功能说明: 配置 AdcTemp 参数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAdcTemp(void)
{
	/*配置内部温度传感器的ADC参数*/
	bsp_InitAdc(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55Cycles5);
	
	/*使能内部温度传感器的通道*/
	ADC_TempSensorVrefintCmd(ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Temprate
*	功能说明: 获取内部温度传感器温度值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
	double temperate;
	
	adcx = Get_Adc_Average(ADC1,SAMP_COUNT);		//SAMP_COUNT次平均值
	temperate = (float) adcx* (3.3 / 4096);			//电压值
	temperate = (1.43 - temperate) / 0.0043 + 25;	//转换为温度值
	result = temperate *= 100;						//扩大100倍
	return result;
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
