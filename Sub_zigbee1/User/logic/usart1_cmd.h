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

#ifndef _USART1_CMD_H_
#define _USART1_CMD_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */
//帧头
#define FRAME_HEADER_BYTE_1st		0x44
#define FRAME_HEADER_BYTE_2nd		0x5A
#define FRAME_HEADER_BYTE_3rd		0x4C
//0，1，2


#define	FRAME_DECEIVE_TYPE1			0x01//主从板
#define	FRAME_DECEIVE_TYPE2			0x02//zigbee
//3

#define FRAME_DATA_FIELD_OFFSET		0x05
//4

#define FRAME_CONST_BYTE_LEN		0x06
   


//上位机->下位机的命令
#define FUN_CODE_CMD				0x01//功能码0x01

#define FUN_CODE_DEVICE_RELAY		0x01//是继电器命令

//闭合、断开命令
#define FUN_CODE_CMD_RELAY_ON		0x01//继电器闭合
#define FUN_CODE_CMD_RELAY_OFF		0x02//继电器断开
#define FUN_CODE_CMD_RELAY_TOGGLE	0x03//继电器反转
#define FUN_CODE_CMD_RELAY_QUERY	0x04//继电器查询

//RSP
#define FUN_CODE_RSP_RELAY_ON		0xFF//继电器闭合
#define FUN_CODE_RSP_RELAY_OFF		0x00//继电器断开

#define FUN_CODE_DECEIVE_FAN		0x02
#define FUN_CODE_CMD_FAN_ON			0x01
#define FUN_CODE_CMD_FAN_OFF		0x02
#define FUN_CODE_CMD_FAN_TOGGLE		0x03
#define FUN_CODE_CMD_FAN_QUERY		0x04
#define FUN_CODE_RSP_FAN_ON		    0x00
#define FUN_CODE_RSP_FAN_OFF		0xff
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void usart2_cmd(void);
uint8_t CheckSum(uint8_t *buf,uint16_t len);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
