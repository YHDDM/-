/*
*********************************************************************************************************
*
*	模块名称 : 继电器驱动模块
*	文件名称 : bsp_relay.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-05-29 付燕华  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

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
	GPIO_InitTypeDef	GPIO_InitStructure;
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY, ENABLE); 
	/* 
	配置所有的RELAY GPIO为推挽输出模式 
	由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮. 
	这是我不希望的，因此在改变GPIO为输出前，先关闭RELAY 
	*/ 
	bsp_RelayOff(1); 
	bsp_RelayOff(2); 
	bsp_RelayOff(3); 
	bsp_RelayOff(4); 
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出模式 */ 
  
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;  /* RELAY1端口 */ 
	GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure); /* 初始化RELAY1 */ 
 
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;  /* RELAY2端口 */ 
	GPIO_Init(RELAY2_GPIO_PORT, &GPIO_InitStructure); /* 初始化RELAY2 */ 
  
	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;  /* RELAY3端口 */ 
	GPIO_Init(RELAY3_GPIO_PORT, &GPIO_InitStructure); /* 初始化RELAY3 */ 
	
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;  /* RELAY4端口 */ 
	GPIO_Init(RELAY4_GPIO_PORT, &GPIO_InitStructure); /* 初始化RELAY4 */
	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RelayOn
*	功能说明: 闭合指定的继电器。 
*	形    参： _no : 继电器序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayOn(uint8_t _no) 
{
	if(_no == 1)
	{
		RELAY1=1;
	}
	else if(_no==2)
	{
		RELAY2=1;
	}
	else if(_no==3)
	{
		RELAY3=1;
	}
	else if(_no==4)
	{
		RELAY4=1;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_RelayOff
*	功能说明: 断开指定的继电器。 
*	形    参：_no : 继电器序号，范围 1 - 4 
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayOff(uint8_t _no)
{
	if(_no==1)
	{
		RELAY1=0;
	}
	else if(_no==2)
	{
		RELAY2=0;
	}
	else if(_no==3)
	{
		RELAY3=0;
	}
	else if(_no==4)
	{
		RELAY4=0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: bsp_RelayToggle
*	功能说明: 翻转指定的继电器。
*	形    参：_no : 继电器序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RelayToggle(uint8_t _no) 
{
	if(_no==1)
	{
		RELAY1_GPIO_PORT->ODR^=RELAY1_GPIO_PIN;
	}
	else if(_no==2)
	{
		RELAY2_GPIO_PORT->ODR^=RELAY2_GPIO_PIN;
	}
	else if(_no==3)
	{
		RELAY3_GPIO_PORT->ODR^=RELAY3_GPIO_PIN;
	}
	else if(_no==4)
	{
		RELAY4_GPIO_PORT->ODR^=RELAY4_GPIO_PIN;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_IsRelayOn
*	功能说明: 判断继电器是否已经闭合。
*	形    参：_no : 继电器序号，范围 1 - 4
*	返 回 值:  1表示已经闭合，0表示未闭合
*********************************************************************************************************
*/
uint8_t bsp_IsRelayOn(uint8_t _no)
{
	if(_no==1)
	{
		if((RELAY1_GPIO_PORT->ODR & RELAY1_GPIO_PIN) == RELAY1_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if(_no==2)
	{
		if ((RELAY2_GPIO_PORT->ODR & RELAY2_GPIO_PIN) == RELAY2_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if(_no==3)
	{
		if ((RELAY3_GPIO_PORT->ODR & RELAY3_GPIO_PIN) == RELAY3_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	else if(_no==4)
	{
		if ((RELAY4_GPIO_PORT->ODR & RELAY4_GPIO_PIN) == RELAY4_GPIO_PIN)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}



///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
