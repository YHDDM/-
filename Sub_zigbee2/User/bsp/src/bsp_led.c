/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯，配合新建工程使用，非完整文件
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-08-02 waroyal  正式发布
*
*	Copyright (C), 2014-2019, 德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/* 打开GPIO时钟 */
//	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);
//	
//	GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
//	
//	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
//	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
	
	//1.定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.打开时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_LED,ENABLE);
	
	//3.结构体赋值
	//LED0
	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//LED0的
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //LED0模式设置为推免输出模式
	
	//4.调用GPIO_Init()函数进行初始化
	//LDE0
	GPIO_Init(LED0_GPIO_PORT,&GPIO_InitStructure);
	
	//LDE1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);

	
	//5.端口初始电平设置
	//位带
//	LED0 = 1;
//	LED1 = 1;
	//库函数
	GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);

	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: LED点亮
*	形    参: uint8_t _no: 1~2;
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if( _no == 1)
	{
		//位带
		LED0 = 0;
	}else if( _no == 2)
	{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN); //库函数操作
	}
}

void bsp_LedOff(uint8_t _no)
{
	if( _no == 1)
	{
		LED0 = 1;
	}else if( _no == 2)
	{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	}
}



/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		LED0_GPIO_PORT->ODR ^= LED0_GPIO_PIN;
	}
	else if(_no == 2)
	{
		LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
	}
}



/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
