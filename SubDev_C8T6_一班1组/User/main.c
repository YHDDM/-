/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 自制板数据获取上传及设备控制
*	修改记录 :
*		版本号  日期        作者     				说明
*		V1.0    2024-05-31 付燕华、单东昭、赵堤 	首发
*
*	Copyright (C), 2014-2024, 天津德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */

#include "usart1_cmd.h"
#include "sensor_Data_Proc.h"

/*
STM32每个系列都会有唯一的一个芯片序列号（96位bit):
	STM32F10X 的地址是0x1FFFF7E8
	STM32F20X 的地址是0x1FFF7A10
	STM32F30X 的地址是0x1FFFF7AC
	STM32F40X 的地址是0x1FFF7A10
	STM32L1XX 的地址是0x1FF80050
*/
/*SN起始地址*/
#define STM32F10X_SN_ADDR 0x1FFFF7E8

/*定义例程名和例程发布日期*/
#define EXAMPLE_NAME "WSNEP_V01-102b_IIC实验（SHT30字节协议传输方式））"
#define EXAMPLE_DATE "2024-06-21"
#define DEMO_VER 	 "1.0"
#define NAME		 "付燕华_2350713224"


/* 定义LCD显示相关公共信息 */ 
#define DEV_NAME   "WSNEP_V01" 
#define LCD_EXAMPLE_NAME  "FAN TEST" 


/*仅允许本文件内调用的函数声明*/
static void PrintfLogo(void);
static void PrintfHelp(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	float lux = 0,temp_real=0,humid_real=0;
	uint16_t co2_value = 0; //二氧化碳获取变量
	uint16_t co2_real = 0;
	uint16_t pm2_5_value = 0;//PM2.5获取变量
	uint16_t pm2_5_real = 0;//PM2.5获取变量
	uint8_t shake_value = 0;//震动传感器
	uint8_t red_value = 0;//红外传感器
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f10x.c 文件，主要功能是
	配置CPU系统的时钟
	*/
	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/*打印例程信息到串口1*/
	PrintfHelp();	/*打印提示操作*/	

	bsp_InitRelay();
	bsp_InitFan();
	bsp_InitIoSensor();
	bsp_InitI2C();
	bsp_InitSht3x();
	bsp_InitOpt3001();
	
	bsp_StartAutoTimer(0,100); /*定时器0 500毫秒 检测温湿度*/
	while(1)
	{
		bsp_Idle();//CPU空闲时执行的函数，在bsp.c
		//bsp_BeepOn();

		usart1_cmd();	
		if(bsp_CheckTimer(0))		/*定时到*/
		{	

			if(SHT3X_GetValue())
			{
				bsp_InitI2C();
				bsp_InitSht3x();
			}
			else 
			{
				temp_real = Tem_Value;
				humid_real = RH_Value;
			}	
			
			co2_value = CO2_READ();//二氧化碳数值读取
			pm2_5_value = PM25_READ();//PM2.5数值读取
			if(co2_value!=65535 ) co2_real = co2_value;//二氧化碳真实数据读取
			if(pm2_5_value!=65535 ) pm2_5_real = pm2_5_value;//PM2.5真实数据读取
			lux = GetLuxValue();//光照度获取
			
			if ( VIBRATE_OUT_flag == 1 ) 
			{ 
				
				shake_value = 0x3F;			  //震动检测到标志位
				VIBRATE_OUT_flag = 0;         //清零 
				
			} 
			else
			{
				shake_value = 0x00;
			}
			
			if ( PIR_OUT_flag == 1 ) 
			{ 
				
				red_value = 0x3F;		  //红外检测到标志位
				PIR_OUT_flag = 0;         //清零 	
				bsp_BeepOn();			  //蜂鸣器报警
			} 
			else
			{
				red_value = 0x00;	
				bsp_BeepOff();
			}		
			sensor_Upload(temp_real,humid_real,lux,co2_real,pm2_5_real,shake_value,red_value);//数据上传
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印信息
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void) 
{ 
 /* 检测CPU ID */ 
 { 
	uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2; 
 
	CPU_Sn0 = *(__IO uint32_t*)(STM32F10X_SN_ADDR); 
	CPU_Sn1 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 4); 
	CPU_Sn2 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 8); 
 
	printf("\r\nCPU : STM32F103ZET6, LQFP144, UID = %08X %08X %08X\n\r" 
	, CPU_Sn2, CPU_Sn1, CPU_Sn0); 
 } 
 
	printf("\r\n"); 
	printf("*************************************************************\r\n"); 
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME); /* 打印例程名称 */ 
	printf("* 例程版本   : %s\r\n", DEMO_VER);  /* 打印例程版本 */ 
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE); /* 打印例程日期 */ 
	printf("* 提交者     : %s\r\n", NAME); 		/* 打印提交者 */ 

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */ 
	printf("* 固件库版本 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", 
	__STM32F10X_STDPERIPH_VERSION_MAIN, 
	__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2); 
	printf("* \r\n"); /* 打印一行空格 */ 
	printf("*************************************************************\r\n"); 
} 

/*
********************************************************************************************************* 
* 函 数 名: PrintfHelp 
* 功能说明: 打印操作提示 
* 形    参：无 
* 返 回 值: 无 
***********************************************8+********************************************************** 
*/ 
static void PrintfHelp(void) 
{ 
	 printf("操作提示:\r\n"); 
	 printf("1.本程序中，每500msDS0红灯翻转一次\r\n"); 
	 printf("2.每500ms读取显示显示温湿度信息,字节协议\r\n"); 
	 printf("3.通过按下KEY_UP按键切换成字节协议方式输出，通过按下KEY0按键切换成字符串方式输出\r\n"); 
	 printf("\r\n\r\n"); 
} 


/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

