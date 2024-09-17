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

#ifndef __BSP_28BYJ48_H_
#define __BSP_28BYJ48_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */
#define RCC_ALL_28BYJ48	(A_GPIO_CLK | B_GPIO_CLK | C_GPIO_CLK | D_GPIO_CLK)

#define A_GPIO_PIN			GPIO_Pin_4			/*A相引脚号*/
#define A_PIN_ID			4					/*A相引脚序号*/
#define A_GPIO_PORT			GPIOC				/*A相的端口号*/
#define A_GPIO_CLK			RCC_APB2Periph_GPIOC/*A相的时钟*/
#define A_FUN_OUT			PCout				/*A相输出配置函数*/
#define A_FUN_IN			PCin				/*A相的输入配置函数*/

#define B_GPIO_PIN			GPIO_Pin_5			/*B相引脚号*/
#define B_PIN_ID			5					/*B相引脚序号*/
#define B_GPIO_PORT			GPIOC				/*B相的端口号*/
#define B_GPIO_CLK			RCC_APB2Periph_GPIOC/*B相的时钟*/
#define B_FUN_OUT			PCout				/*B相输出配置函数*/
#define B_FUN_IN			PCin				/*B相的输入配置函数*/

#define C_GPIO_PIN			GPIO_Pin_6			/*C相引脚号*/
#define C_PIN_ID			6					/*C相引脚序号*/
#define C_GPIO_PORT			GPIOC				/*C相的端口号*/
#define C_GPIO_CLK			RCC_APB2Periph_GPIOC/*C相的时钟*/
#define C_FUN_OUT			PCout				/*C相输出配置函数*/
#define C_FUN_IN			PCin				/*C相的输入配置函数*/

#define D_GPIO_PIN			GPIO_Pin_7			/*D相引脚号*/
#define D_PIN_ID			7					/*D相引脚序号*/
#define D_GPIO_PORT			GPIOC				/*D相的端口号*/
#define D_GPIO_CLK			RCC_APB2Periph_GPIOC/*D相的时钟*/
#define D_FUN_OUT			PCout				/*D相输出配置函数*/
#define D_FUN_IN			PCin				/*D相的输入配置函数*/


/*IO操作函数*/
#define SM28BYJ48_A A_FUN_OUT(A_PIN_ID)
#define SM28BYJ48_B B_FUN_OUT(B_PIN_ID)
#define SM28BYJ48_C C_FUN_OUT(C_PIN_ID)
#define SM28BYJ48_D D_FUN_OUT(D_PIN_ID)
///////////////////////////////////////
/* 外部变量申明区 */



typedef struct
{
	uint8_t ucSpeed;	/*电机速度*/
	uint8_t ucDir;		/*电机方向 0正转，1反转 2停止*/
	uint8_t ucStop;		/*0 运行 1停止*/
}MOTOR_T;

/*步进电机导通控制定义*/
enum
{
	SM_OFF = 0,		/*ABCD相均不导通*/
	SM_A,			/*A相导通*/
	SM_B,			/*B相导通*/
	SM_C,			/*C相导通*/
	SM_D,			/*D相导通*/
	SM_AB,			/*AB相导通*/
	SM_BC,			/*BC相导通*/
	SM_CD,			/*CD相导通*/
	SM_DA,			/*DA相导通*/	
};



///////////////////////////////////////
/* 函数申明区 */
void bsp_InitSM28BYJ48(void);
void motPowerOn(uint8_t _ucMode);
void motPowerOn2(uint8_t _ucMode);
void test_fun(uint8_t _ucStop,uint8_t _ucDir,uint8_t _ucSpeed);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
