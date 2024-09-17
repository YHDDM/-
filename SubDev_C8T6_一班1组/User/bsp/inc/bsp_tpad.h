/*
*********************************************************************************************************
*
*	模块名称 : 电容按键模块
*	文件名称 : bsp_tpad.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-04-07	付燕华
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_TPAD_H_
#define _BSP_TPAD_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */

#define TIMx_CAP_TIM						TIM5
#define TIMx_CAP_TIM_APBxClock_FUN			RCC_APB1PeriphClockCmd
#define TIMx_CAP_TIM_CLK					RCC_APB1Periph_TIM5
#define	TPAD_ARR_MAX_VAL					0xFFFF
#define	TIMx_CAP_TIM_PSC					( 6 - 1 )				//以72M/6=12MHz来计数

//GPIO
#define	TIMx_CAP_TIM_CH_GPIO_CLK			RCC_APB2Periph_GPIOA
#define	TIMx_CAP_TIM_CH_PORT				GPIOA
#define	TIMx_CAP_TIM_CH_PIN					GPIO_Pin_1
#define	TIMx_CAP_TIM_CHANNEL_x				TIM_Channel_2			//通

//中断
#define TIMx_CAP_TIM_IT_CCx					TIM_IT_CC2
#define TIMx_CAP_TIM_FLAG_CCx				TIM_FLAG_CC2
#define TIMx_CAP_TIM_IT_UPDATE  			TIM_IT_Update 
#define TIMx_CAP_TIM_IRQ    				TIM5_IRQn 
#define TIMx_CAP_TIM_INT_FUN 				TIM5_IRQHandler 

// 获取捕获寄存器值函数宏定义 
#define TIMx_CAP_TIM_GetCapturex_FUN		TIM_GetCapture2

//捕获信号极性函数宏定义
#define TIMx_CAP_TIM_OCxPolarityConfig_FUN  TIM_OC2PolarityConfig 
//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
#define TPAD_GATE_VAL						100						
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
uint8_t bsp_TpadInit(void);  	/* 触摸按键初始化 */ 
uint8_t bsp_TpadScan(void); 	/* 触摸按键扫描 */
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
