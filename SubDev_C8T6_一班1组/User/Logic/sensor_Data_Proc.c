#include "sensor_Data_Proc.h"
#include "bsp_uart.h"
//设定传感器信息上报的缓存
uint8_t	SENSOR_DATA_BYTE[248]=
{
	FRAME_HEADER_BYTE_1st,FRAME_HEADER_BYTE_2nd,FRAME_HEADER_BYTE_3rd,//帧头（3个字节）
	FRAME_DECEIVE_TYPE1,
	0x3B,//数据区长度（1个字节）
	0x03,//类型为数据（1个字节）
	0x01,//为传感器数据（1个字节）
	0x38//为传感器数据长度（1个字节）
	//----//传感器数据区（8*N个字节）（N最大为30）
	//-		//校验（1个字节）
};
_sensorData Sensor_Data_ToFloat;//传感器数据转换为浮点数
_sensorData	Sensor_Data_To4Byte;//传感器数据传唤为4字节 内存6

/*
*********************************************************************************************************
*	函 数 名: sensor_TempHumi_Upload
*	功能说明: 温湿度传感器字节协议上报函数
*	形    参：temperature：温度, humidity：湿度, lm：光照, co2：二氧化碳, pm2_5：PM2.5,shake：震动状态,red：红外报警
*	返 回 值: 无
*********************************************************************************************************
*/
void sensor_Upload(float temperature,float humidity,float lm,uint16_t co2,uint16_t pm2_5,uint8_t shake,uint8_t red)
{
	uint8_t i=0;
	
	//定义一下温度传感器
	uint8_t sensor_Temp_buf[8]={
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
	uint8_t sensor_Humi_buf[8]={
								ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
								ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
								SENSOR_HUMI_CODE,
								SENSOR_STATUS_RUN,
								0,			
								0,
								0,
								0
								};								
	//定义一下光照强度传感器
	uint8_t sensor_Lm_buf[8]={
								ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
								ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
								SENSOR_LM_CODE,
								SENSOR_STATUS_RUN,
								0,			
								0,
								0,
								0
								};							
	//定义一下CO2传感器
	uint8_t sensor_Co2_buf[8]={
								ENVIRONMENT_SENSOR_TYPE_CODE_MSB,
								ENVIRONMENT_SENSOR_TYPE_CODE_LSB,
								SENSOR_CO2_CODE,
								SENSOR_STATUS_RUN,
								0,			
								0,
								0,
								0
								};
	//定义PM2.5传感器
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
	//赋值注意对共用体的浮点数赋值
	Sensor_Data_To4Byte.f=temperature;//温度
	//注意，内存中数组是小端模式存储的，因此需要转换为大端模式
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4+i]=Sensor_Data_To4Byte.x[3-i];
	}							
	
	Sensor_Data_To4Byte.f=humidity;//湿度
	
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4+i]=Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f=lm;//光照
	
	for(i=0;i<4;i++)
	{
		sensor_Lm_buf[4+i]=Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f=co2;//二氧化碳
	
	for(i=0;i<4;i++)
	{
		sensor_Co2_buf[4+i]=Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = pm2_5;//pm2.5
	
	for(i=0;i<4;i++)
	{
		sensor_PM2_5_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];	
	}
	
	for(i=0;i<5;i++)
	{
		sensor_shake_buf[3 + i] = shake;//震动	
	}
		
	for(i=0;i<5;i++)
	{
		sensor_red_buf[3 + i] = red;//红外	
	}
	
	//偏移地址 0x04,（改动前为0x03，因被zigbee占用了，后移一位）
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 59;//(1 + 1 + 1 + 7 * 8)长度59字节
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
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN))],sensor_Lm_buf,8);//拷贝光照强度

	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 3 * SENSOR_SINGLE_DATA_LEN))],sensor_Co2_buf,8);//拷贝二氧化碳
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 4 * SENSOR_SINGLE_DATA_LEN))],sensor_PM2_5_buf,8);//拷贝pm2.5
	
	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 5 * SENSOR_SINGLE_DATA_LEN))],sensor_shake_buf,8);//拷贝震动传感器数据

	memcpy(&SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN))],sensor_red_buf,8);//拷贝人体红外传感器数据

	//计算校验和
	SENSOR_DATA_BYTE[(SENSOR_DATA_START_OFFSET + (7 * SENSOR_SINGLE_DATA_LEN))] = CheckSum(&SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET]);//计算校验码
	
	USART1_Send_Data(SENSOR_DATA_BYTE,65);//64发送数据 3（帧头）+ 1 (ZigBee) + 1 (长度字节) + 59（数据区长度） + 1（校验位）
}