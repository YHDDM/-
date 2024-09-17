/*
*********************************************************************************************************
*
*	ģ������ : xx����ģ��
*	�ļ����� : bsp_xxx.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    20xx-xx-xx waroyal  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
#include "sys.h"
///////////////////////////////////////
/* ���������� */
//�趨��������Ϣ�ϱ��Ļ���
uint8_t SENSOR_DATA_BYTE[248] = 
{
	FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,//֡ͷ��3���ֽڣ�
	FRAME_DECEIVE_TYPE2,/*FRAME_DECEIVE_TYPE2ΪZigbee�豸ID*/
	0x3b,//���������ȣ�1���ֽڣ�
	0x03,//����Ϊ���ݣ�1���ֽڣ�
	0x01,//Ϊ���������ݣ�1���ֽڣ�
	0x38,//Ϊ���������ݳ��ȣ�1���ֽڣ�
	//...//��������������8*N���ֽڣ���N���Ϊ30��
	//- //У�飨1���ֽڣ�
};

_sensorData Sensor_Data_ToFloat;//����������ת��Ϊ������
_sensorData Sensor_Data_To4Byte; //����������ת��Ϊ4�ֽ� �ڴ�


///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sensor_Upload(float temperature,float humidity,float lux,uint16_t co2,uint16_t pm2_5,uint8_t shake,uint8_t red)
{
	uint8_t i = 0;//ѭ������
	//����һ���¶ȴ�����
	uint8_t sensor_Temp_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_TEMP_CODE,
		SENSOR_STATUS_RUN,
		0,
		0,
		0,
		0
	};
	
	//����һ��ʪ�ȴ�����
	uint8_t sensor_Humi_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_HUMI_CODE,
		SENSOR_STATUS_RUN,
		0,
		0,
		0,
		0
	};
	
	//����һ�¹��մ�����
		uint8_t sensor_Light_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_LM_CODE,
		SENSOR_STATUS_RUN,
		0,
		0,
		0,
		0
	};	
		
	//����һ�¶�����̼������
		uint8_t sensor_Co2_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_CO2_CODE,
		SENSOR_STATUS_RUN,
		0,
		0,
		0,
		0
	};	
	//����һ��PM2.5������
		uint8_t sensor_PM2_5_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_PM25_CODE,
		SENSOR_STATUS_RUN,
		0,
		0,
		0,
		0
	};	
		
	//����һ���𶯴�����
		uint8_t sensor_shake_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_SHAKE_CODE,
		0,
		0,
		0,
		0,
		0
	};
		
	//����һ�º��⴫����
		uint8_t sensor_red_buf[8] = {
		ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
		ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
		SENSOR_RED_CODE,
		0,
		0,
		0,
		0,
		0
	};
	
//		if(USART_RX_BUF[3]!=FRAME_DECEIVE_TYPE2)//����ӻ�Zigbeeû���յ�������ִֹͣ��
//		{
//			return ;
//		}
//	
	//��ֵ��ע��Թ�����ĸ�������ֵ
	Sensor_Data_To4Byte.f = temperature;//�¶���ֵ���
	//ע�⣬�ڴ���������С��=ģʽ�洢�ģ������Ҫת��Ϊ���ģʽ
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = humidity;//ʪ����ֵ���
	
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = lux;//������ֵ���
	
		for(i=0;i<4;i++)
	{
		sensor_Light_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = co2;//co2��ֵ���
	
		for(i=0;i<4;i++)
	{
		sensor_Co2_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = pm2_5;//pm2.5��ֵ���
	
		for(i=0;i<4;i++)
	{
		sensor_PM2_5_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = shake;//�𶯴��������ݴ��
	
		for(i=0;i<4;i++)
	{
		sensor_shake_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = red;//���⴫�������ݴ��
	
		for(i=0;i<4;i++)
	{
		sensor_red_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	
	//ƫ�Ƶ�ַ 0x04,���Ķ�ǰΪ0x03����zigbeeռ���ˣ�����һλ��
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 59;//(1 + 1 + 1 + 7 * 8)����43�ֽ�
	//ƫ�Ƶ�ַ 0x05
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA;//����֡
	//ƫ�Ƶ�ַ 0x06
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR;//����������
	//ƫ�Ƶ�ַ 0x07
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 56;//����7*8==56���ֽ�
	//ƫ�Ƶ�ַ 0x08
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 0  * SENSOR_SINGLE_DATA_LEN))],sensor_Temp_buf,8);//�����¶�����
	//ƫ�Ƶ�ַ 0x0F
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 1 * SENSOR_SINGLE_DATA_LEN))],sensor_Humi_buf,8);//����ʪ��
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN))],sensor_Light_buf,8);//��������ǿ��

	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 3 * SENSOR_SINGLE_DATA_LEN))],sensor_Co2_buf,8);//����������̼
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 4 * SENSOR_SINGLE_DATA_LEN))],sensor_PM2_5_buf,8);//����pm2.5
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 5 * SENSOR_SINGLE_DATA_LEN))],sensor_shake_buf,8);//�����𶯴���������
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN))],sensor_red_buf,8);//����������⴫��������

	//����У���
	SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + (7 * SENSOR_SINGLE_DATA_LEN))] = CheckSum(&SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET]);//����У����
	
	USART1_Send_Data(SENSOR_DATA_BYTE,65);//64�������� 3��֡ͷ��+ 1 (ZigBee) + 1 (�����ֽ�) + 59�����������ȣ� + 1��У��λ��
}


///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
