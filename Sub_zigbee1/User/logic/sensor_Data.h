/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : .h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _SENSOR_DATA_H_
#define _SENSOR_DATA_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
#include "usart1_cmd.h"
///////////////////////////////////////
/* 宏定义区域 */
//数据帧，即功能码，表示数据上报
#define FUN_CODE_DATA				0x03

//单个传感器的数据长度
#define SENSOR_SINGLE_DATA_LEN		8

#define DATA_LEN_OFFSET				4//数据长度偏移地址
#define FRAME_TYPE_OFFSET			5//帧类型偏移地址

#define DATA_TYPE_OFFSET			6//数据帧的数据类型偏移地址
#define SENSOR_DATA_LEN_OFFSET		7//传感器数据长度  偏移地址
#define SENSOR_DATA_START_OFFSET	8//第一个传感器首地址	偏移地址

#define SENSOR_TEMP_CODE			0x01//温度传感器
#define SENSOR_HUMI_CODE			0x02//湿度传感器
#define SENSOR_SHAKE_CODE			0X03//震动传感器
#define SENSOR_LM_CODE				0x04//光照传感器
#define SENSOR_CO2_CODE				0x05//CO2传感器
#define	SENSOR_PM25_CODE			0x06//PM2.5传感器
#define SENSOR_RED_CODE			    0X07//红外线传感器

#define ENVIRONMENT_SENSOR_TYPE_CODE_MSB	0x22//环境传感器
#define ENVIRONMENT_SENSOR_TYPE_CODE_LSB	0x00//

#define	SENSOR_STATUS_PRE					0x00//传感器预热
#define SENSOR_STATUS_NG					0x01//传感器数值异常
#define	SENSOR_STATUS_RUN					0x02//传感器正常运行

#define DATA_TYPE_SENSOR					0x01//传感器数据
///////////////////////////////////////
/* 外部变量申明区 */
//注意x[3]为大端，x[0]为小端
typedef union
{
	float f;
	uint8_t x[4];
}_sensorData;//传感器数据 float 与 4字节 相互变换 使用联合体

///////////////////////////////////////
/* 函数申明区 */
void sensor_Upload(float temperature,float humidity,float lux,uint16_t co2,uint16_t pm2_5,uint8_t shake,uint8_t red);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
