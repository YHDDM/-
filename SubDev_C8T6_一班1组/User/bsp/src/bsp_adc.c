/*
*********************************************************************************************************
*
*	模块名称 : ADC模块驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-4-10 付燕华l  正式发布
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
 static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime);
 static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times);


///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAdc
*	功能说明: 初始化ADC的完整流程
*	形    参：ADCx:ADC1,ADC2,ADC3
*			  ADC_Channel:ADC_Channel_1~17哪个通道 
*			  ADC_SampleTime:采样时间 
*	返 回 值: 无
*********************************************************************************************************
*/

static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime)
{
	//定义结构体
	ADC_InitTypeDef ADC_InitStructure;
	//使能时钟
	if(ADCx == ADC1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		ADC_DeInit(ADC1);
	}
	else if(ADCx == ADC2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
		ADC_DeInit(ADC2);
	}
	else if(ADCx == ADC3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
		ADC_DeInit(ADC3);
	}
	//结构体赋值
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//单次转换
	ADC_InitStructure.ADC_DataAlign =ADC_DataAlign_Right ;				//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;//软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//独立通道
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//单次采样
	ADC_Init(ADCx,&ADC_InitStructure);
	
	//规则采样通道设置(ADC_NbrOfChannel是几就写几次)
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime);
	
	//使能ADC
	ADC_Cmd(ADCx,ENABLE);
	
	//校准复位+等待复位完成
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	//启动校准+等待校准完成
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
}
/*
*********************************************************************************************************
*	函 数 名: ADC_GetVal
*	功能说明: 获取启动ADC，获取ADC转换值
*	形    参：ADCx ：ADC1~3
*	返 回 值:  转换结果 
*********************************************************************************************************
*/

static uint16_t ADC_GetVal(ADC_TypeDef* ADCx)
{
	//软件启动采样
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	//等待转换完成
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC)==RESET)
	{};
	//获取ADC采样值,返回
	return ADC_GetConversionValue(ADCx);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc_Average
*	功能说明: 
*	形    参：ADCx:1~3
*			  times 采样时间
*	返 回 值: 无
*********************************************************************************************************
*/

static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=ADC_GetVal(ADCx);
	}
	return temp_val/times;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAdcPotentiometer
*	功能说明:  bsp_InitAdcPotentiometer参数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_InitAdcPotentiometer(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RES_POT_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = RES_POT_GPIO_PIN;
	GPIO_Init(RES_POT_GPIO_PORT,&GPIO_InitStructure);
	/* 配置RES_POT的ADC参数 */
	bsp_InitAdc(RES_POT_ADC_X,RES_POT_ADC_CHANNEL,RES_POT_ADC_SAMPLETIME);
	
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc_Potentiometer
*	功能说明: 得到电位器电压值。
*	形    参：无
*	返 回 值: 返回值:电位器电压值，均值采样
*********************************************************************************************************
*/

uint16_t Get_Adc_Potentiometer(void)
{
	uint16_t adcx;
	adcx = Get_Adc_Average(RES_POT_ADC_X,SAMP_COUNT);	/*SAMP_COUNT次取平均*/
	return adcx;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_IintAdcTemp
*	功能说明:  配置AdcTemp参数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_InitAdcTemp(void)
{
	/* 配置内部温度传感器的ADC参数 */ 
	bsp_InitAdc(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55Cycles5);
	/* 使能内部温度传感器的通道 */ 
	ADC_TempSensorVrefintCmd(ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Temprate
*	功能说明: 得到温度值。
*	形    参：无
*	返 回 值: 返回值:温度值(扩大了100倍,单位:℃.) 
*********************************************************************************************************
*/

short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
	double temperate;
		
	adcx = Get_Adc_Average(ADC1,SAMP_COUNT);	//SAMP_COUNT次取平均
	temperate = (float)adcx*(3.3/4096);			//电压值 
	temperate=(1.43-temperate)/0.0043+25;		//转换为温度值 
	result=temperate*=100;						//扩大100倍. 
	return result;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAdcLsens
*	功能说明: bsp_InitAdcLsens参数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_InitAdcLsens(void) 
{ 
	 GPIO_InitTypeDef GPIO_InitStructure; 
	  
	 RCC_APB2PeriphClockCmd(LSENS_GPIO_CLK, ENABLE); 
	  
	 /* 配置RES_POT为模拟输入 */ 
	 GPIO_InitStructure.GPIO_Pin = LSENS_GPIO_PIN; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	 GPIO_Init(LSENS_GPIO_PORT, &GPIO_InitStructure); 
	  
	 /* 配置LSENSE的ADC参数 */ 
	 bsp_InitAdc(LSENS_ADC_X,LSENS_ADC_CHANNEL,LSENS_ADC_SAMPLETIME); 
  
} 
/*
*********************************************************************************************************
*	函 数 名: Lsens_Get_Val
*	功能说明: 读取Light Sens的值 
*	形    参：无
*	返 回 值:  0~100:0,最暗;100,最亮  
*********************************************************************************************************
*/

uint8_t Lsens_Get_Val(void)
{
	u32 temp_val=0; 
	temp_val = Get_Adc_Average(LSENS_ADC_X,LSENS_READ_TIMES);  //LSENS_READ_TIMES次取平均 
	  
	if(temp_val>4000) 
	{ 
	  temp_val=4000; 
	} 
	return (uint8_t)(100-(temp_val/40));
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
