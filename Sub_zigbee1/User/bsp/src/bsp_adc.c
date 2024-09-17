/*
*********************************************************************************************************
*
*	ģ������ : ��ģת������ģ��
*	�ļ����� : bsp_adc.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-04-10 Lawrence  ��ʽ����
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
*	�� �� ��: bsp_InitAdc
*	����˵��:  ADCx(ADC1 - ADC3)  ��  ADC_Channel(0-17) �� ADC_SampleTime(1.5�����Լ���ȫ)
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
	//����ṹ��
	ADC_InitTypeDef 	ADC_InitStructure;
	
	//ʹ��ʱ��
	if(ADCx == ADC1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	//ʹ��ADC1ͨ��ʱ��
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC ��λ
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����
		//ADC_DeInit(ADC1);//�ȼ�����������
	}
	else if(ADCx == ADC2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);	//ʹ��ADC2ͨ��ʱ��
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC ��λ
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);//��λ����
		//ADC_DeInit(ADC2);//�ȼ�����������
	}
	else if(ADCx == ADC3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);	//ʹ��ADC3ͨ��ʱ��
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC ��λ
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);//��λ����
		//ADC_DeInit(ADC3);//�ȼ�����������
	}
	
	//�ṹ�帳ֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC����ģ#ʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ������ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//�Ҷ���ģʽ  12λ
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//˳����й���ת����ADCͨ����Ŀ				
	ADC_Init(ADCx,&ADC_InitStructure);									//ADCx��ʼ���Ĵ���
	
	//�������ͨ������
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime);
	
	//ʹ��ADC
	ADC_Cmd(ADCx,ENABLE);
	//У׼��λ+�ȴ���λ���
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));							//�ȴ���λУ׼����
	//����У׼
	ADC_StartCalibration(ADCx);
	//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx));								//�ȴ�У׼����
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Adc
*	����˵��: ��ȡADCֵ��������Թ���ͨ��Ϊ�������������ڲ�����
*	��    ��: ADC_TypeDef* ADCx  ,�����ĸ�ADC
*	�� �� ֵ: ת���Ľ��
*********************************************************************************************************
*/
static uint16_t Get_Adc(ADC_TypeDef* ADCx)
{
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);								//ʹ��ָ����ADCx�������������
	
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) == RESET);				//�ȴ�ת������
	
	return ADC_GetConversionValue(ADCx);								//�������һ��ADCx�������ת�����
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Adc_Average
*	����˵��: ���ADCֵ��������Թ���ͨ��Ϊ��
*	��    ��: ADC_TypeDef* ADCx �������ĸ�ADC ��uint8_t times��β�����ƽ��ֵ
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_InitAdcPotentiometer
*	����˵��: ��ʼ����λ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitAdcPotentiometer(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RES_POT_GPIO_CLK,ENABLE);
	
	/*����RES_POT λģ������*/
	GPIO_InitStructure.GPIO_Pin = RES_POT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(RES_POT_GPIO_PORT,&GPIO_InitStructure);
	
	/*����RES_POT��ADC����*/
	bsp_InitAdc(RES_POT_ADC_X,RES_POT_ADC_CHANNEL,RES_POT_ADC_SAMPLETIME);
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Adc_Potentiometer
*	����˵��: �õ���λ���ĵ�ѹֵ
*	��    ��: ��
*	�� �� ֵ: ����ֵ��λ����ѹֵ����ֵ����
*********************************************************************************************************
*/
uint16_t Get_Adc_Potentiometer(void)
{
	uint16_t adcx;
	adcx = Get_Adc_Average(RES_POT_ADC_X,SAMP_COUNT);			//SAMP_COUNT��ȡƽ��
	return adcx;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAdcTemp
*	����˵��: ���� AdcTemp ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitAdcTemp(void)
{
	/*�����ڲ��¶ȴ�������ADC����*/
	bsp_InitAdc(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55Cycles5);
	
	/*ʹ���ڲ��¶ȴ�������ͨ��*/
	ADC_TempSensorVrefintCmd(ENABLE);
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Temprate
*	����˵��: ��ȡ�ڲ��¶ȴ������¶�ֵ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
	double temperate;
	
	adcx = Get_Adc_Average(ADC1,SAMP_COUNT);		//SAMP_COUNT��ƽ��ֵ
	temperate = (float) adcx* (3.3 / 4096);			//��ѹֵ
	temperate = (1.43 - temperate) / 0.0043 + 25;	//ת��Ϊ�¶�ֵ
	result = temperate *= 100;						//����100��
	return result;
}
///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
