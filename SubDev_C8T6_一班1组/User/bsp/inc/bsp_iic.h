/*
*********************************************************************************************************
*
*	模块名称 : IIC模块
*	文件名称 : bsp_iic.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-05-31	付燕华
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_IIC_H_
#define _BSP_IIC_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "sys.h"

/////////////////////////////////////////////////////////// 
//移植修改区 
//按照实际端口修改 
//较前版本更加容易移植(注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTAG引脚，需要打开AFIO时钟，并失能JTAG) 


///////////////////////////////////////
/* 宏定义区域 */


#define	SCL_GPIO_PIN	GPIO_Pin_0			//SCL 引脚号
#define	SCL_PIN_ID		0				//SCL 引脚序号
#define	SCL_GPIO_PORT	GPIOB				//SCL 端口号
#define	SCL_GPIO_CLK	RCC_APB2Periph_GPIOB//SCL 时钟 
#define	SCL_FUN_OUT		PBout 				//SCL 输出端口配置函数 
#define SCL_FUN_IN 	  	PBin				//SCL 输入端口配置函数 

///
#define	SDA_GPIO_PIN	GPIO_Pin_1			//SDA 引脚号
#define	SDA_PIN_ID		1					//SDA 引脚序号
#define	SDA_GPIO_PORT	GPIOB				//SDA 端口号
#define	SDA_GPIO_CLK	RCC_APB2Periph_GPIOB//SDA 时钟 
#define	SDA_FUN_OUT		PBout 				//SDA 输出端口配置函数 
#define SDA_FUN_IN   	PBin				//SDA 输入端口配置函数 



///////////////////////////

//IO方向设计
#if	SDA_PIN_ID>=8		//当端口号大于等于8，是高8位的 用CRH 
	
	#define	IIC_ROLBIT	((SDA_PIN_ID-8)*4)
	
	#define	IIC_CLR_VAL	(~((uint32_t)0XF<<IIC_ROLBIT))
	
	#define SDA_IN()    {SDA_GPIO_PORT->CRH&=IIC_CLR_VAL;SDA_GPIO_PORT->CRH|=(u32)8<<IIC_ROLBIT;} 
	#define SDA_OUT()   {SDA_GPIO_PORT->CRH&=IIC_CLR_VAL;SDA_GPIO_PORT->CRH|=(u32)3<<IIC_ROLBIT;}
#else
	#define	IIC_ROLBIT	(SDA_PIN_ID*4)
	
	#define	IIC_CLR_VAL	( ~( (uint32_t)0XF << IIC_ROLBIT ) ) 
	
	#define SDA_IN()    {SDA_GPIO_PORT->CRL&=IIC_CLR_VAL;SDA_GPIO_PORT->CRL|=(u32)8<<IIC_ROLBIT;} 
	#define SDA_OUT()   {SDA_GPIO_PORT->CRL&=IIC_CLR_VAL;SDA_GPIO_PORT->CRL|=(u32)3<<IIC_ROLBIT;}

#endif

//IO操作函数
#define IIC_SCL      	SCL_FUN_OUT(SCL_PIN_ID) //SCL 
#define IIC_SDA     	SDA_FUN_OUT(SDA_PIN_ID) //SDA   
#define READ_SDA   	 	SDA_FUN_IN(SDA_PIN_ID)  //输入SDA 

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitI2C(void);               //初始化IIC的IO口
void IIC_Start(void);    //发送IIC开始信号 
void IIC_Stop(void);      //发送IIC停止信号 
void IIC_Send_Byte(uint8_t txd);   //IIC发送一个字节 
uint8_t IIC_Read_Byte(unsigned char ack); 
uint8_t IIC_Wait_Ack(void);    //IIC等待ACK信号 
void IIC_Ack(void);     //IIC发送ACK信号 
void IIC_NAck(void);    //IIC不发送ACK信号
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

