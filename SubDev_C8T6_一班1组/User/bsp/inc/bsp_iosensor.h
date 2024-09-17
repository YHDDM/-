#ifndef __BSP_IOSENSOR_H 
#define __BSP_IOSENSOR_H 

#include "sys.h" 

/////////////////////////////////////////////////////////// 
//移植修改区 
//按照实际端口修改 
//较前版本更加容易移植(注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTAG引脚，需要打开AFIO时钟，并失能JTAG) 



/////////////////////// 
//DO101 震动传感器 
#define VIBRATE_OUT_GPIO_PIN		GPIO_Pin_11  			//VIBRATE_OUT 引脚号 
#define VIBRATE_OUT_PIN_ID			11  					//VIBRATE_OUT 引脚序号 
#define VIBRATE_OUT_GPIO_PORT		GPIOA  					//VIBRATE_OUT 端口号 
#define VIBRATE_OUT_GPIO_CLK		RCC_APB2Periph_GPIOA	//VIBRATE_OUT 时钟
//#define VIBRATE_OUT_FUN_OUT  		PAout     				//VIBRATE_OUT输出端口配置函数
#define VIBRATE_OUT_FUN_IN  		PAin     				//VIBRATE_OUT输入端口配置函数   
#define VIBRATE_OUT_GPIO_MODE  		GPIO_Mode_IPU   		//VIBRATE_OUT外部中断输入模式 
#define VIBRATE_OUT_ACTIVE_LEVEL 	0     					//VIBRATE_OUT有效电平为低电平

#define VIBRATE_OUT_PORT_SRC  		GPIO_PortSourceGPIOA  	//VIBRATE_OUT外部中断端口号
#define VIBRATE_OUT_PIN_SRC 		GPIO_PinSource11  		//VIBRATE_OUT外部中断引脚号
#define	VIBRATE_OUT_EXTI_LINE		EXTI_Line11				//VIBRATE_OUT外部中断号 
#define	VIBRATE_OUT_EXTI_TRIG		EXTI_Trigger_Falling	//VIBRATE_OUT外部中断触发方式
#define	VIBRATE_OUT_EXTI_IRQN		EXTI15_10_IRQn			//VIBRATE_OUT外部中断NVIC号
///////////////////
///PIR 人体红外传感器 
#define PIR_OUT_GPIO_PIN  			GPIO_Pin_3  			//PIR_OUT引脚号 
#define PIR_OUT_PIN_ID				3  					//PIR_OUT引脚序号 
#define PIR_OUT_GPIO_PORT			GPIOB					//PIR_OUT端口号
#define PIR_OUT_GPIO_CLK			RCC_APB2Periph_GPIOB	//PIR_OUT时钟
//#define PIR_OUT_FUN_OUT			PBout					//PIR_OUT输出端口配置函数 
#define PIR_OUT_FUN_IN				PBin					//PIR_OUT输入端口配置函数
#define	PIR_OUT_GPIO_MODE			GPIO_Mode_IPD			//PIR_OUT外部中断输入模式
#define	PIR_OUT_ACTIVE_LEVEL		1						//PIR_OUT有效电平为高电平

#define PIR_OUT_PORT_SRC			GPIO_PortSourceGPIOB	//PIR_OUT外部中断端口号
#define PIR_OUT_PIN_SRC  			GPIO_PinSource3   		//PIR_OUT外部中断引脚号 
#define	PIR_OUT_EXTI_LINE			EXTI_Line3				//PIR_OUT外部中断号
#define	PIR_OUT_EXTI_TRIG			EXTI_Trigger_Rising		//PIR_OUT外部中断触发方式
#define	PIR_OUT_EXTI_IRQN			EXTI3_IRQn			//PIR_OUT外部中断NVIC号 

////////////////////////////////////////////////////

//IO操作函数
#define VIBRATE_OUT		VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID)	//读取VIBRATE_OUT 低电平有效
#define PIR_OUT 		PIR_OUT_FUN_IN(PIR_OUT_PIN_ID)  		//读取PIR_OUT 高电平有效 

extern uint8_t VIBRATE_OUT_flag; 
extern uint8_t PIR_OUT_flag;


/* 供外部调用的函数声明 */ 
void bsp_InitIoSensor(void); 
 
#endif



/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
