/*
*********************************************************************************************************
*
*	模块名称 : xx驱动模块
*	文件名称 : bsp_xxx.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    20xx-xx-xx waroyal  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
#include "sys.h"
///////////////////////////////////////
/* 变量定义区 */
//设定传感器信息上报的缓存
uint8_t SENSOR_DATA_BYTE[248] = 
{
	FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,//帧头（3个字节）
	FRAME_DECEIVE_TYPE2,/*FRAME_DECEIVE_TYPE2为Zigbee设备ID*/
	0x3b,//数据区长度（1个字节）
	0x03,//类型为数据（1个字节）
	0x01,//为传感器数据（1个字节）
	0x38,//为传感器数据长度（1个字节）
	//...//传感器数据区（8*N个字节）（N最大为30）
	//- //校验（1个字节）
};

_sensorData Sensor_Data_ToFloat;//传感器数据转换为浮点数
_sensorData Sensor_Data_To4Byte; //传感器数据转换为4字节 内存


///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sensor_Upload(float temperature,float humidity,float lux,uint16_t co2,uint16_t pm2_5,uint8_t shake,uint8_t red)
{
	uint8_t i = 0;//循环数据
	//定义一下温度传感器
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
	
	//定义一下湿度传感器
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
	
	//定义一下光照传感器
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
		
	//定义一下二氧化碳传感器
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
	//定义一下PM2.5传感器
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
		
	//定义一下震动传感器
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
		
	//定义一下红外传感器
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
	
//		if(USART_RX_BUF[3]!=FRAME_DECEIVE_TYPE2)//如果从机Zigbee没有收到数据则停止执行
//		{
//			return ;
//		}
//	
	//赋值，注意对共用体的浮点数赋值
	Sensor_Data_To4Byte.f = temperature;//温度数值存放
	//注意，内存中数组是小端=模式存储的，因此需要转换为大端模式
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = humidity;//湿度数值存放
	
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = lux;//光照数值存放
	
		for(i=0;i<4;i++)
	{
		sensor_Light_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = co2;//co2数值存放
	
		for(i=0;i<4;i++)
	{
		sensor_Co2_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = pm2_5;//pm2.5数值存放
	
		for(i=0;i<4;i++)
	{
		sensor_PM2_5_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = shake;//震动传感器数据存放
	
		for(i=0;i<4;i++)
	{
		sensor_shake_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	Sensor_Data_To4Byte.f = red;//红外传感器数据存放
	
		for(i=0;i<4;i++)
	{
		sensor_red_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];		
	}
	
	
	//偏移地址 0x04,（改动前为0x03，因被zigbee占用了，后移一位）
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 59;//(1 + 1 + 1 + 7 * 8)长度43字节
	//偏移地址 0x05
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA;//数据帧
	//偏移地址 0x06
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR;//传感器数据
	//偏移地址 0x07
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 56;//长度7*8==56个字节
	//偏移地址 0x08
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 0  * SENSOR_SINGLE_DATA_LEN))],sensor_Temp_buf,8);//拷贝温度数据
	//偏移地址 0x0F
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 1 * SENSOR_SINGLE_DATA_LEN))],sensor_Humi_buf,8);//拷贝湿度
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN))],sensor_Light_buf,8);//拷贝光照强度

	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 3 * SENSOR_SINGLE_DATA_LEN))],sensor_Co2_buf,8);//拷贝二氧化碳
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 4 * SENSOR_SINGLE_DATA_LEN))],sensor_PM2_5_buf,8);//拷贝pm2.5
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 5 * SENSOR_SINGLE_DATA_LEN))],sensor_shake_buf,8);//拷贝震动传感器数据
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN))],sensor_red_buf,8);//拷贝人体红外传感器数据

	//计算校验和
	SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + (7 * SENSOR_SINGLE_DATA_LEN))] = CheckSum(&SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET]);//计算校验码
	
	USART1_Send_Data(SENSOR_DATA_BYTE,65);//64发送数据 3（帧头）+ 1 (ZigBee) + 1 (长度字节) + 59（数据区长度） + 1（校验位）
}


///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
