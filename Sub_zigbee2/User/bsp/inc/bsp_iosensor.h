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
#define PIR_OUT_GPIO_PIN  			GPIO_Pin_3   			//PIR_OUT引脚号 
#define PIR_OUT_PIN_ID				3    					//PIR_OUT引脚序号 
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


///DO111 火焰传感器 
#define FIRE_OUT_GPIO_PIN  			GPIO_Pin_12   			//FIRE_OUT引脚号 
#define FIRE_OUT_PIN_ID				12    					//FIRE_OUT引脚序号 
#define FIRE_OUT_GPIO_PORT			GPIOA					//FIRE_OUT端口号
#define FIRE_OUT_GPIO_CLK			RCC_APB2Periph_GPIOA	//FIRE_OUT时钟
//#define FIRE_OUT_FUN_OUT			PAout					//FIRE_OUT输出端口配置函数 
#define FIRE_OUT_FUN_IN				PAin					//FIRE_OUT输入端口配置函数
#define	FIRE_OUT_GPIO_MODE			GPIO_Mode_IPU			//FIRE_OUT外部中断输入模式
#define	FIRE_OUT_ACTIVE_LEVEL		0						//FIRE_OUT有效电平为低电平

#define FIRE_OUT_PORT_SRC			GPIO_PortSourceGPIOA	//FIRE_OUT外部中断端口号
#define FIRE_OUT_PIN_SRC  			GPIO_PinSource12   		//FIRE_OUT外部中断引脚号 
#define	FIRE_OUT_EXTI_LINE			EXTI_Line12				//FIRE_OUT外部中断号
#define	FIRE_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//FIRE_OUT外部中断触发方式
#define	FIRE_OUT_EXTI_IRQN			EXTI15_10_IRQn			//FIRE_OUT外部中断NVIC号 

///DO201 可燃气体传感器 
#define GASES_OUT_GPIO_PIN  		GPIO_Pin_15   			//GASES_OUT引脚号 
#define GASES_OUT_PIN_ID			15    					//GASES_OUT引脚序号 
#define GASES_OUT_GPIO_PORT			GPIOA					//GASES_OUT端口号
#define GASES_OUT_GPIO_CLK			RCC_APB2Periph_GPIOA	//GASES_OUT时钟
//#define GASES_OUT_FUN_OUT			PAout					//GASES_OUT输出端口配置函数 
#define GASES_OUT_FUN_IN			PAin					//GASES_OUT输入端口配置函数
#define	GASES_OUT_GPIO_MODE			GPIO_Mode_IPU			//GASES_OUT外部中断输入模式
#define	GASES_OUT_ACTIVE_LEVEL		0						//GASES_OUT有效电平为低电平

#define GASES_OUT_PORT_SRC			GPIO_PortSourceGPIOA	//GASES_OUT外部中断端口号
#define GASES_OUT_PIN_SRC  			GPIO_PinSource15   		//GASES_OUT外部中断引脚号 
#define	GASES_OUT_EXTI_LINE			EXTI_Line15				//GASES_OUT外部中断号
#define	GASES_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//GASES_OUT外部中断触发方式
#define	GASES_OUT_EXTI_IRQN			EXTI15_10_IRQn			//GASES_OUT外部中断NVIC号 

///DO211 霍尔传感器 
#define HALL_OUT_GPIO_PIN  			GPIO_Pin_13   			//HALL_OUT引脚号 
#define HALL_OUT_PIN_ID				13    					//HALL_OUT引脚序号 
#define HALL_OUT_GPIO_PORT			GPIOC					//HALL_OUT端口号
#define HALL_OUT_GPIO_CLK			RCC_APB2Periph_GPIOC	//HALL_OUT时钟
//#define HALL_OUT_FUN_OUT			PAout					//HALL_OUT输出端口配置函数 
#define HALL_OUT_FUN_IN				PAin					//HALL_OUT输入端口配置函数
#define	HALL_OUT_GPIO_MODE			GPIO_Mode_IPU			//HALL_OUT外部中断输入模式
#define	HALL_OUT_ACTIVE_LEVEL		0						//HALL_OUT有效电平为低电平

#define HALL_OUT_PORT_SRC			GPIO_PortSourceGPIOC	//HALL_OUT外部中断端口号
#define HALL_OUT_PIN_SRC  			GPIO_PinSource13   		//HALL_OUT外部中断引脚号 
#define	HALL_OUT_EXTI_LINE			EXTI_Line13				//HALL_OUT外部中断号
#define	HALL_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//HALL_OUT外部中断触发方式
#define	HALL_OUT_EXTI_IRQN			EXTI15_10_IRQn			//HALL_OUT外部中断NVIC号 

////////////////////////////////////////////////////

//IO操作函数
#define VIBRATE_OUT		VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID)	//读取VIBRATE_OUT 低电平有效
#define PIR_OUT 		PIR_OUT_FUN_IN(PIR_OUT_PIN_ID)  		//读取PIR_OUT 高电平有效 
#define FIRE_OUT		FIRE_OUT_FUN_IN(FIRE_OUT_PIN_ID)		//读取FIRE_OUT 低电平有效
#define GASES_OUT		GASES_OUT_FUN_IN(GASES_OUT_PIN_ID)		//读取GASES_OUT 低电平有效
#define HALL_OUT		HALL_OUT_FUN_IN(HALL_OUT_PIN_ID)		//读取HALL_OUT 低电平有效

extern uint8_t VIBRATE_OUT_flag; 
extern uint8_t PIR_OUT_flag;
extern uint8_t FIRE_OUT_flag;  	
extern uint8_t HALL_OUT_flag;  
extern uint8_t GASES_OUT_flag;  	

/* 供外部调用的函数声明 */ 
void bsp_InitIoSensor(void); 
 
#endif



/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
