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

#ifndef _BSP_RELAY_H_
#define _BSP_RELAY_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */
#ifdef WSNEP_V01

#define RCC_ALL_RELAY			(RELAY1_GPIO_CLK | RELAY2_GPIO_CLK | RELAY3_GPIO_CLK | RELAY4_GPIO_CLK)

#define RELAY4_GPIO_PIN				GPIO_Pin_4			//RELAY4引脚号
#define RELAY4_PIN_ID				4					//RELAY4引脚序号
#define RELAY4_GPIO_PORT			GPIOB				//RELAY4端口号
#define RELAY4_GPIO_CLK				RCC_APB2Periph_GPIOB//RELAY4时钟
#define RELAY4_FUN_OUT				PBout				//RELAY4的输出端口配置函数
//#define RELAY4_GPIO_IN				PBin				//RELAY4的输入端口配置函数

#define RELAY3_GPIO_PIN				GPIO_Pin_5			//RELAY3引脚号
#define RELAY3_PIN_ID				5					//RELAY3引脚序号
#define RELAY3_GPIO_PORT			GPIOB				//RELAY3端口号
#define RELAY3_GPIO_CLK				RCC_APB2Periph_GPIOB//RELAY3时钟
#define RELAY3_FUN_OUT				PBout				//RELAY3的输出端口配置函数
//#define RELAY3_GPIO_IN				PBin				//RELAY3的输入端口配置函数

#define RELAY2_GPIO_PIN				GPIO_Pin_6			//RELAY2引脚号
#define RELAY2_PIN_ID				6					//RELAY2引脚序号
#define RELAY2_GPIO_PORT			GPIOB				//RELAY2端口号
#define RELAY2_GPIO_CLK				RCC_APB2Periph_GPIOB//RELAY2时钟
#define RELAY2_FUN_OUT				PBout				//RELAY2的输出端口配置函数
//#define RELAY2_GPIO_IN				PBin				//RELAY2的输入端口配置函数

//1B
#define RELAY1_GPIO_PIN				GPIO_Pin_7			//RELAY1引脚号
#define RELAY1_PIN_ID				7					//RELAY1引脚序号
#define RELAY1_GPIO_PORT			GPIOB				//RELAY1端口号
#define RELAY1_GPIO_CLK				RCC_APB2Periph_GPIOB//RELAY1时钟
#define RELAY1_FUN_OUT				PBout				//RELAY1的输出端口配置函数
//#define RELAY1_GPIO_IN				PBin				//RELAY1的输入端口配置函数
			//RELAY4的输入端口配置函数
#endif

///////////////////////////////////////
//IO操作函数
#define RELAY1				RELAY1_FUN_OUT(RELAY1_PIN_ID)//RELAY1
#define RELAY2				RELAY2_FUN_OUT(RELAY2_PIN_ID)//RELAY2
#define RELAY3				RELAY3_FUN_OUT(RELAY3_PIN_ID)//RELAY3
#define RELAY4				RELAY4_FUN_OUT(RELAY4_PIN_ID)//RELAY4

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitRelay(void);
uint8_t bsp_IsRelayOn(uint8_t _no);
void bsp_RelayOn(uint8_t _no);
void bsp_RelayOff(uint8_t _no);
void bsp_RelayToggle(uint8_t _no);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
