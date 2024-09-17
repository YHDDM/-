/*
*********************************************************************************************************
*
*	模块名称 : 步进电机驱动模块
*	文件名称 : bsp_28byj48.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.2    2024-07-03 	Lawrence  正式发布
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
static uint8_t _index = 0;
static uint8_t test[9]={1,5,2,6,3,7,4,8,1};

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitSM28BYJ48
*	功能说明: GPIO和时钟初始化
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSM28BYJ48(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_ALL_28BYJ48,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//A相
	GPIO_InitStructure.GPIO_Pin = A_GPIO_PIN;
	GPIO_Init(A_GPIO_PORT,&GPIO_InitStructure);
	//B相
	GPIO_InitStructure.GPIO_Pin = B_GPIO_PIN;
	GPIO_Init(B_GPIO_PORT,&GPIO_InitStructure);
	//C相
	GPIO_InitStructure.GPIO_Pin = C_GPIO_PIN;
	GPIO_Init(C_GPIO_PORT,&GPIO_InitStructure);
	//D相
	GPIO_InitStructure.GPIO_Pin = D_GPIO_PIN;
	GPIO_Init(D_GPIO_PORT,&GPIO_InitStructure);
	
	//设置默认关闭
	GPIO_ResetBits(A_GPIO_PORT,A_GPIO_PIN);
	GPIO_ResetBits(B_GPIO_PORT,B_GPIO_PIN);
	GPIO_ResetBits(C_GPIO_PORT,C_GPIO_PIN);
	GPIO_ResetBits(D_GPIO_PORT,D_GPIO_PIN);

}

/*
*********************************************************************************************************
*	函 数 名: motPowerOn
*	功能说明: 正转
*	形    参：uint8_t _ucMode
*	返 回 值: 无
*********************************************************************************************************
*/
void motPowerOn(uint8_t _ucMode)
{
	if(_ucMode == SM_OFF)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}
	else if(_ucMode == SM_A)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}
	else if(_ucMode == SM_B)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_C)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_D)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 1;
	}else if(_ucMode == SM_AB)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_BC)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_CD)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_D = 1;
	}else if(_ucMode == SM_DA)
	{
		SM28BYJ48_A = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 1;
	}
}
/*
*********************************************************************************************************
*	函 数 名: motPowerOn2
*	功能说明: 反转
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void motPowerOn2(uint8_t _ucMode)
{
	if(_ucMode == SM_OFF)
	{
		SM28BYJ48_A = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_D = 0;
	}else if(_ucMode == SM_DA)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}else if(_ucMode == SM_D)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_CD)
	{
		SM28BYJ48_D = 1;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_C)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_BC)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 1;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_B)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 0;
	}else if(_ucMode == SM_AB)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 1;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}else if(_ucMode == SM_A)
	{
		SM28BYJ48_D = 0;
		SM28BYJ48_B = 0;
		SM28BYJ48_C = 0;
		SM28BYJ48_A = 1;
	}
}
/*
*********************************************************************************************************
*	函 数 名: test_fun
*	功能说明: 电机控制
*	形    参：无=uint8_t _ucStop 启动停止,uint8_t _ucDir 正反转,uint8_t _ucSpeed 转速最快1ms
*	返 回 值: 无
*********************************************************************************************************
*/

void test_fun(uint8_t _ucStop,uint8_t _ucDir,uint8_t _ucSpeed)
{
	if(_ucStop == 0)
	{
		
		switch(_ucDir)
		{
			case 0://正转
			{
				if(bsp_CheckTimer(3))
				{
					motPowerOn(test[_index++]);
					if(_index == 9) _index = 0;
					bsp_StartTimer(3,_ucSpeed);
				}
				
			}break;
			
			case 1://反转
			{
				if(bsp_CheckTimer(3))
				{
					if(_index <= 0) _index = 8;
					motPowerOn(test[_index--]);
					bsp_StartTimer(3,_ucSpeed);
				}
			}break;
			
			case 2:
			{
					motPowerOn(0);
			}break;
		}

	}

}


///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
