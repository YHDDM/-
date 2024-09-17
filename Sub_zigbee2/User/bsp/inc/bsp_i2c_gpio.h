/*
*********************************************************************************************************
*
*	模块名称 : I2C模块
*	文件名称 : bsp_i2c_gpio.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-03	Lawrence
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H_
#define _BSP_I2C_GPIO_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
#include "sys.h"

///////////////////////////////////////
/* 宏定义区域 */
#define	SCL_GPIO_PIN	GPIO_Pin_0			//SCL引脚号
#define	SCL_PIN_ID		0					//SCL引脚序号
#define	SCL_GPIO_PORT	GPIOB				//SCL端口号
#define	SCL_GPIO_CLK	RCC_APB2Periph_GPIOB//SCL时钟
#define	SCL_FUN_OUT		PBout				//SCL输出端口配置函数

#define	SDA_GPIO_PIN	GPIO_Pin_1			//SDA引脚号
#define	SDA_PIN_ID		1					//SDA引脚序号
#define	SDA_GPIO_PORT	GPIOB				//SDA端口号
#define	SDA_GPIO_CLK	RCC_APB2Periph_GPIOB//SDA时钟
#define	SDA_FUN_OUT		PBout				//SDA输出端口配置函数
#define SDA_FUN_IN		PBin				//SDA输入端口配置函数

//IO方向设置
#if SDA_PIN_ID	>= 8						//当端口号大于等于8，是高8位的用CRH
	#define IIC_ROLBIT						((SDA_PIN_ID - 8) * 4)
	
	#define	IIC_CLR_VAL						(~((uint32_t)0xF << IIC_ROLBIT))
	
	#define SDA_IN() {SDA_GPIO_PORT->CRH &= IIC_CLR_VAL; SDA_GPIO_PORT->CRH |= (u32)8<<IIC_ROLBIT;}
	#define SDA_OUT() {SDA_GPIO_PORT->CRH &= IIC_CLR_VAL; SDA_GPIO_PORT->CRH |= (u32)3<<IIC_ROLBIT;}
#else										//当端口号小于8，是低8位的 用CRL
	#define IIC_ROLBIT						(SDA_PIN_ID * 4)
	
	#define IIC_CLR_VAL						(~((uint32_t)0xF << IIC_ROLBIT))
	
	#define SDA_IN(){SDA_GPIO_PORT->CRL &= IIC_CLR_VAL; SDA_GPIO_PORT->CRL |= (u32)8<<IIC_ROLBIT;}
	#define SDA_OUT(){SDA_GPIO_PORT->CRL &= IIC_CLR_VAL; SDA_GPIO_PORT->CRL |= (u32)3<<IIC_ROLBIT;}
#endif

//IO操作函数
#define IIC_SCL		SCL_FUN_OUT(SCL_PIN_ID)	//SCL
#define IIC_SDA		SDA_FUN_OUT(SDA_PIN_ID)	//SDA
#define READ_SDA	SDA_FUN_IN(SDA_PIN_ID)	//输入SDA
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitI2C(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);


///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
