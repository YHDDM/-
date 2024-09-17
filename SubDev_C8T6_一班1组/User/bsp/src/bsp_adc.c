/*
*********************************************************************************************************
*
*	ģ������ : ADCģ������ģ��
*	�ļ����� : bsp_adc.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-4-10 ���໪l  ��ʽ����
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
 static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime);
 static uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t times);


///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAdc
*	����˵��: ��ʼ��ADC����������
*	��    �Σ�ADCx:ADC1,ADC2,ADC3
*			  ADC_Channel:ADC_Channel_1~17�ĸ�ͨ�� 
*			  ADC_SampleTime:����ʱ�� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void bsp_InitAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime)
{
	//����ṹ��
	ADC_InitTypeDef ADC_InitStructure;
	//ʹ��ʱ��
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
	//�ṹ�帳ֵ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//����ת��
	ADC_InitStructure.ADC_DataAlign =ADC_DataAlign_Right ;				//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;//�������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//����ͨ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//���β���
	ADC_Init(ADCx,&ADC_InitStructure);
	
	//�������ͨ������(ADC_NbrOfChannel�Ǽ���д����)
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime);
	
	//ʹ��ADC
	ADC_Cmd(ADCx,ENABLE);
	
	//У׼��λ+�ȴ���λ���
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	//����У׼+�ȴ�У׼���
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
}
/*
*********************************************************************************************************
*	�� �� ��: ADC_GetVal
*	����˵��: ��ȡ����ADC����ȡADCת��ֵ
*	��    �Σ�ADCx ��ADC1~3
*	�� �� ֵ:  ת����� 
*********************************************************************************************************
*/

static uint16_t ADC_GetVal(ADC_TypeDef* ADCx)
{
	//�����������
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	//�ȴ�ת�����
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC)==RESET)
	{};
	//��ȡADC����ֵ,����
	return ADC_GetConversionValue(ADCx);
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Adc_Average
*	����˵��: 
*	��    �Σ�ADCx:1~3
*			  times ����ʱ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_InitAdcPotentiometer
*	����˵��:  bsp_InitAdcPotentiometer������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitAdcPotentiometer(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RES_POT_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = RES_POT_GPIO_PIN;
	GPIO_Init(RES_POT_GPIO_PORT,&GPIO_InitStructure);
	/* ����RES_POT��ADC���� */
	bsp_InitAdc(RES_POT_ADC_X,RES_POT_ADC_CHANNEL,RES_POT_ADC_SAMPLETIME);
	
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Adc_Potentiometer
*	����˵��: �õ���λ����ѹֵ��
*	��    �Σ���
*	�� �� ֵ: ����ֵ:��λ����ѹֵ����ֵ����
*********************************************************************************************************
*/

uint16_t Get_Adc_Potentiometer(void)
{
	uint16_t adcx;
	adcx = Get_Adc_Average(RES_POT_ADC_X,SAMP_COUNT);	/*SAMP_COUNT��ȡƽ��*/
	return adcx;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_IintAdcTemp
*	����˵��:  ����AdcTemp������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitAdcTemp(void)
{
	/* �����ڲ��¶ȴ�������ADC���� */ 
	bsp_InitAdc(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55Cycles5);
	/* ʹ���ڲ��¶ȴ�������ͨ�� */ 
	ADC_TempSensorVrefintCmd(ENABLE);
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Temprate
*	����˵��: �õ��¶�ֵ��
*	��    �Σ���
*	�� �� ֵ: ����ֵ:�¶�ֵ(������100��,��λ:��.) 
*********************************************************************************************************
*/

short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
	double temperate;
		
	adcx = Get_Adc_Average(ADC1,SAMP_COUNT);	//SAMP_COUNT��ȡƽ��
	temperate = (float)adcx*(3.3/4096);			//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;		//ת��Ϊ�¶�ֵ 
	result=temperate*=100;						//����100��. 
	return result;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAdcLsens
*	����˵��: bsp_InitAdcLsens����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitAdcLsens(void) 
{ 
	 GPIO_InitTypeDef GPIO_InitStructure; 
	  
	 RCC_APB2PeriphClockCmd(LSENS_GPIO_CLK, ENABLE); 
	  
	 /* ����RES_POTΪģ������ */ 
	 GPIO_InitStructure.GPIO_Pin = LSENS_GPIO_PIN; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	 GPIO_Init(LSENS_GPIO_PORT, &GPIO_InitStructure); 
	  
	 /* ����LSENSE��ADC���� */ 
	 bsp_InitAdc(LSENS_ADC_X,LSENS_ADC_CHANNEL,LSENS_ADC_SAMPLETIME); 
  
} 
/*
*********************************************************************************************************
*	�� �� ��: Lsens_Get_Val
*	����˵��: ��ȡLight Sens��ֵ 
*	��    �Σ���
*	�� �� ֵ:  0~100:0,�;100,����  
*********************************************************************************************************
*/

uint8_t Lsens_Get_Val(void)
{
	u32 temp_val=0; 
	temp_val = Get_Adc_Average(LSENS_ADC_X,LSENS_READ_TIMES);  //LSENS_READ_TIMES��ȡƽ�� 
	  
	if(temp_val>4000) 
	{ 
	  temp_val=4000; 
	} 
	return (uint8_t)(100-(temp_val/40));
}
///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
