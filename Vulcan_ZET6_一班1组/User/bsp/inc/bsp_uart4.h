/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : bsp_uart4.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef __BSP_UART4_H
#define __BSP_UART4_H

///////////////////////////////////////
/* 头文件包含区 */

#include "bsp.h"



///////////////////////////////////////
/* 宏定义区域 */
#define UART4_BUF_LEN 		65 				//定义 UART4 缓冲长度为 64 个字节
//如果想串口中断接收，请不要注释以下宏定义
#define EN_UART4_RX 		1 				//0,不接收;1,接收.

///////////////////////////////////////
/* 外部变量申明区 */
extern uint8_t	USART4_RX_BUF[UART4_BUF_LEN];	//接收缓存，最大USART_REC_LEN个字节，末字节为换行符
extern uint8_t	ReceiveState4;					//接收状态标记
extern uint16_t	RxCounter4;						//接收长度
extern float temp,humid,light,co2,pm25;
extern uint8_t relay_1,relay_2,relay_3,relay_4,fan_1,fan_2,vib,pir;

///////////////////////////////////////
/* 函数申明区 */

void bsp_InitUart4(uint32_t bound);
void Uart4_STA_Clr(void);
void USART4_Send_Data(uint8_t *buf,uint8_t len);
void usart4_cmd_receive(void);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

