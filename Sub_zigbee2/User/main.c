/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : WSNEP_V01-103_IIC实验(OPT3001)
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-20	Lawrence  首发
*
*	Copyright (C), 2014-2019, 天津德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */
/*SN起始地址*/
#define STM32F10X_SN_ADDR 0x1FFFF7E8

/*定义例程名和例程发布日期*/
#define EXAMPLE_NAME	"WSNEP_V01-103_自制板综合实验"
#define EXAMPLE_DATE	" 2024-06-20"
#define DEMO_VER		"1.0"
#define NAME			"赵堤_2350412806"

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
	uint16_t lux = 0;
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
	
	
	
	bsp_StartAutoTimer(0,500); /*定时器0 500毫秒 检测温湿度*/
	while(1)
	{
		bsp_Idle();//CPU空闲时执行的函数，在bsp.c
		

		usart2_cmd();	
		if(bsp_CheckTimer(0))		/*定时到*/
		{	

			if ( VIBRATE_OUT_flag == 1 ) 
			{ 
				VIBRATE_OUT_flag = 0;         //清零 
				shake_value = 1;
			} 
			else
			{
				shake_value = 0;
			}
			if ( PIR_OUT_flag == 1 ) 
			{ 
				PIR_OUT_flag = 0;         //清零 
				red_value = 1;
				bsp_BeepOn();
			} 
			else
			{
				red_value = 0;
				bsp_BeepOff();
			}
			co2_value = CO2_READ();
			pm2_5_value = PM25_READ();
			if(co2_value!=65535 ) co2_real = co2_value;
			if(pm2_5_value!=65535 ) pm2_5_real = pm2_5_value;
			lux = GetLuxValue();
			sensor_Upload(Tem_Value,RH_Value,lux,co2_real,pm2_5_real,shake_value,red_value);
			//printf( "温度为%.1f℃,湿度为%.1f%%RH,光照强度%dLUX,二氧化碳%dppm,PM2.5%dppm,震动传感器%d,红外传感器%d\r\n",Tem_Value,RH_Value,lux,co2_value,pm2_5_real,shake_value,red_value);
			
			if(SHT3X_GetValue())
			{
				bsp_InitI2C();
				bsp_InitSht3x();
			}
			else//if(co2_value != 65535  && pm25_value != 65535)
			{
				//printf("环境光照度为%.2flux\r\n",lux);
				//printf("温度为%.1f℃，湿度为%.1f%%RH\r\n",Tem_Value,RH_Value);
				//printf("二氧化碳浓度%d\r\n",co2_value);
				//printf("PM2.5浓度为%d\r\n",pm25_value);
			}	

		}
	}
}

	

/*
*********************************************************************************************************
*	函 数 名: PrintfHelp
*	功能说明: 串口窗口操作提示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfHelp(void)
{
	printf("操作提示:\r\n");
	printf("1.本程序中，每500毫秒DS0红灯翻转一次\r\n");
	printf("2.每500ms读取光照度、温湿度、二氧化碳浓度、PM2.5浓度并显示数据信息\r\n\r\n");

}
/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	{
		uint32_t CPU_Sn0,CPU_Sn1,CPU_Sn2;
		
		CPU_Sn0=*(__IO uint32_t*)(STM32F10X_SN_ADDR);
		CPU_Sn1=*(__IO uint32_t*)(STM32F10X_SN_ADDR + 4);
		CPU_Sn2=*(__IO uint32_t*)(STM32F10X_SN_ADDR + 8);
		
		printf("\r\nCPU:STM32F103ZET6,LQFP144,UID=%08X%08X%08X\n\r",CPU_Sn2,CPU_Sn1,CPU_Sn0);
	}
	
	printf("\r\n");
	printf("************************************************\r\n");
	printf("* 例程名称  :%s\r\n",EXAMPLE_NAME);
	printf("* 例程版本  :%s\r\n",DEMO_VER);
	printf("* 发布日期  :%s\r\n",EXAMPLE_DATE);
	printf("* 姓名  :%s\r\n",NAME);
	
	
	printf("* 固件库版本:V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n",__STM32F10X_STDPERIPH_VERSION_MAIN,__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("\r\n");
	printf("***************************************\r\n");
}

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
