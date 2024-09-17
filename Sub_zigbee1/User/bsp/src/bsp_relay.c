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
void bsp_InitRelay(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY,ENABLE);
	//在结构体赋值之前，关闭四个继电器
	bsp_RelayOff(1);
	bsp_RelayOff(2);
	bsp_RelayOff(3);
	bsp_RelayOff(4);
	//结构体赋值+初始化结构体
	//均为通用推挽输出，速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;
	GPIO_Init(RELAY1_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;
	GPIO_Init(RELAY2_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;
	GPIO_Init(RELAY3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;
	GPIO_Init(RELAY4_GPIO_PORT,&GPIO_InitStructure);


}
/*
*********************************************************************************************************
*	函 数 名: bsp_RelayOn
*	功能说明: 继电器开
*	形    参：uint8_t _no  继电器选择变量_no:1~4
*	返 回 值: 1表示闭合，0表示开启
*********************************************************************************************************
*/
uint8_t bsp_IsRelayOn(uint8_t _no)
{
	if(_no == 1)//RELAY1
	{
			if((RELAY1_GPIO_PORT->ODR & RELAY1_GPIO_PIN) == RELAY1_GPIO_PIN)
		{
			return 1;
		}
			return 0;
		
	}else if(_no == 2)//RELAY2
	{
			if((RELAY2_GPIO_PORT->ODR & RELAY2_GPIO_PIN) == RELAY2_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}else if(_no == 3)//RELAY3
	{
			if((RELAY3_GPIO_PORT->ODR & RELAY3_GPIO_PIN) == RELAY3_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}else if(_no == 4)//RELAY4
	{
			if((RELAY4_GPIO_PORT->ODR & RELAY4_GPIO_PIN) == RELAY4_GPIO_PIN)
		{
			return 1;
		}
			return 0;
	}
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RelayOff
*	功能说明: 继电器关
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayOff(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1 = 0;
	}
	else if(_no == 2)
	{
		RELAY2 = 0;
	}
	else if(_no == 3)
	{
		RELAY3 = 0;
	}
	else if(_no == 4)
	{
		RELAY4 = 0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_RelayOn
*	功能说明: 继电器开
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayOn(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1 = 1;
	}
	else if(_no == 2)
	{
		RELAY2 = 1;
	}
	else if(_no == 3)
	{
		RELAY3 = 1;
	}
	else if(_no == 4)
	{
		RELAY4 = 1;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_RelayToggle
*	功能说明: 继电器状态翻转
*	形    参：uint8_t _no 继电器选择 _no:1~4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayToggle(uint8_t _no)
{
	if(_no == 1)
	{
		RELAY1_GPIO_PORT->ODR ^= RELAY1_GPIO_PIN;
	}else if(_no == 2)
	{
		RELAY2_GPIO_PORT->ODR ^= RELAY2_GPIO_PIN;
	}else if(_no == 3)
	{
		RELAY3_GPIO_PORT->ODR ^= RELAY3_GPIO_PIN;
	}else if(_no == 4)
	{
		RELAY4_GPIO_PORT->ODR ^= RELAY4_GPIO_PIN;
	}
}


///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
