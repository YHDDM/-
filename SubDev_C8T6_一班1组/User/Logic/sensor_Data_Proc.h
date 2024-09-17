#ifndef __SENSOR_DATA_PROC_H_
#define __SENSOR_DATA_PROC_H_

///////////////////////////
//头文件
#include "bsp.h"
#include "usart1_cmd.h"
////////////////////
//变量区

//数据区
#define FRAME_DECEIVE_TYPE1			0x01//主机
#define FRAME_DECEIVE_TYPE2			0x02//ZigBee

#define FUN_CODE_DATA	0x03

//单个传感器的数据长度
#define SENSOR_SINGLE_DATA_LEN		8	

#define	DATA_LEN_OFFSET				4	//数据长度偏移地址
#define	FRAME_TYPE_OFFSET			5	//帧类型偏移地址
	
#define DATA_TYPE_OFFSET			6	//数据帧的数据类型	偏移地址
#define	SENSOR_DATA_LEN_OFFSET		7	//传感器数据长度	偏移地址
#define	SENSOR_DATA_START_OFFSET	8	//第一个传感器数据首地址	偏移地址

#define SENSOR_TEMP_CODE	0x01	//温度传感器
#define	SENSOR_HUMI_CODE	0x02	//湿度传感器
#define SENSOR_SHAKE_CODE	0x03	//震动传感器
#define	SENSOR_LM_CODE		0x04	//光照传感器
#define	SENSOR_CO2_CODE		0x05	//CO2传感器
#define	SENSOR_PM25_CODE	0x06	//PM2.5传感器
#define SENSOR_RED_CODE		0x07	//人体红外传感器

#define	ENVIRONMENT_SENSOR_TYPE_CODE_MSB	0x22//环境传感器
#define	ENVIRONMENT_SENSOR_TYPE_CODE_LSB	0x00//固定标志

#define	SENSOR_STATUS_PRE	0x00//传感器预热
#define	SENSOR_STATUS_NG	0x01//传感器数值异常
#define	SENSOR_STATUS_RUN 	0x02//传感器正常运行

#define	DATA_TYPE_SENSOR	0x01//传感器数据

//////////////////////////////////

//注意x[3]为大端，x[0]为小端
typedef union
{
	float f;
	uint8_t x[4];
}_sensorData;//传感器数据 float与4字节 相互变换

///////////////////
//函数声明区

//字节形式传输温湿度信息
void sensor_Upload(float temperature,float humidity,float lm,uint16_t co2,uint16_t pm2_5,uint8_t shake,uint8_t red);
#endif













