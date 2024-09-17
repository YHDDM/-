/*
*********************************************************************************************************
*
*	ģ������ : ADCģ��ģ��
*	�ļ����� : bsp_adc.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-04-10	���໪
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
///////////////////////////////////////
/* �궨������ */
#define RES_POT_GPIO_PIN		GPIO_Pin_7					//RES_POT ���ź� 
#define RES_POT_PIN_ID			7							//RES_POT ������� 
#define RES_POT_GPIO_PORT		GPIOF						//RES_POT �˿ں�
#define RES_POT_GPIO_CLK		RCC_APB2Periph_GPIOF		//RES_POT ʱ�� 
#define RES_POT_ADC_X			ADC3						//RES_POT �� ADCx 
#define RES_POT_ADC_CHANNEL		ADC_Channel_5				//RES_POT �� ADC ����ͨ��
#define RES_POT_ADC_SAMPLETIME	ADC_SampleTime_55Cycles5	//RES_POT �� ADC ����ʱ�� 

#define SAMP_COUNT				20							//������������ʾ����������ƽ��ֵ



#define LSENS_GPIO_PIN			GPIO_Pin_8					//RES_POT ���ź� 
#define LSENS_PIN_ID			8							//RES_POT ������� 
#define LSENS_GPIO_PORT			GPIOF						//RES_POT �˿ں�
#define LSENS_GPIO_CLK			RCC_APB2Periph_GPIOF		//RES_POT ʱ�� 
#define LSENS_ADC_X				ADC3						//RES_POT �� ADCx 
#define LSENS_ADC_CHANNEL		ADC_Channel_6				//RES_POT �� ADC ����ͨ��
#define LSENS_ADC_SAMPLETIME	ADC_SampleTime_55Cycles5	//RES_POT �� ADC ����ʱ�� 

#define LSENS_READ_TIMES			10							//������������ʾ����������ƽ��ֵ

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

void bsp_InitAdcPotentiometer(void) ;
uint16_t Get_Adc_Potentiometer(void);

void bsp_InitAdcTemp(void);
short Get_Temprate(void);

uint8_t Lsens_Get_Val(void);
void bsp_InitAdcLsens(void) ;

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
