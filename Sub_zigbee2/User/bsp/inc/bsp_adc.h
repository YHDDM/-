/*
*********************************************************************************************************
*
*	模块名称 : 数模驱动模块
*	文件名称 : bsp_adc.h
*	说    明 : 内部温度测量
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-04-10	Lawrence
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */
#define RES_POT_GPIO_PIN 				GPIO_Pin_7					//RES_POT 引脚号
#define RES_POT_PIN_ID					7							//RES_POT 引脚序号
#define RES_POT_GPIO_PORT				GPIOF						//RES_POT 端口号
#define RES_POT_GPIO_CLK				RCC_APB2Periph_GPIOF		//RES_POT 时钟
#define RES_POT_ADC_X					ADC3						//RES_POT 的 ADCx
#define RES_POT_ADC_CHANNEL				ADC_Channel_5				//RES_POT 的 ADC 输入通道
#define RES_POT_ADC_SAMPLETIME			ADC_SampleTime_55Cycles5	//RES_POT 的 ADC采样时间

#define SAMP_COUNT						20							//样本个数，表示采样数据求平均值

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t ADC_SampleTime);
static uint16_t Get_Adc(ADC_TypeDef* ADCx);
static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times);

/*供外部使用*/
void bsp_InitAdcPotentiometer(void);
uint16_t Get_Adc_Potentiometer(void);
void bsp_InitAdcTemp(void);
short Get_Temprate(void);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
