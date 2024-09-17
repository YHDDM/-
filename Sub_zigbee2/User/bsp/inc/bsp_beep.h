/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器模块
*	文件名称 : bsp_beep.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-3-1	付燕华
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_BEEP_H_
#define _BSP_BEEP_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */
#define RCC_ALL_BEEP	(BEEP_GPIO_CLK )

#define BEEP_GPIO_PIN		GPIO_Pin_8				//BEEP引脚号
#define BEEP_PIN_ID			8						//BEEP引脚序号
#define BEEP_GPIO_PORT		GPIOA					//BEEP端口号
#define BEEP_GPIO_CLK		RCC_APB2Periph_GPIOA	//BEEP时钟
#define BEEP_FUN_OUT		PAout					//BEEP输出端口配置函数
//#define BEEP_FUN_IN			PGin					//BEEP输入端口配置函数

//IO操作函数
#define BEEP    BEEP_FUN_OUT(BEEP_PIN_ID) //BEEP
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitBeep(void);
void bsp_BeepOn(void);
void bsp_BeepOff(void);
uint8_t bsp_IsBeepOn(void);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
