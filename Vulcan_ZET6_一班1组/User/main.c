/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : Vulcan板数据转发及LCD等设备控制。
*	修改记录 :
*		版本号  日期        作者    	  说明
*		V1.0    2024-03-26 	付燕华、赵堤  首发
*
*	Copyright (C), 2014-2019, 天津德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */
#include "pic.h"				/* 图标和图片数据 */

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
#define EXAMPLE_NAME "WSNEP_V10-005_串口1（打印实验）"
#define EXAMPLE_DATE "2024-03-22"
#define DEMO_VER 	 "1.0"
#define NAME		 "付燕华_2350713224"
#define LOGO_NAME	 "SMART HOME"

/*仅允许本文件内调用的函数声明*/
static void PrintfLogo(void);
static void PrintfHelp(void);
static void Show_LCD(void);
static MOTOR_T t_moto;
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
		
	bsp_Init();				/*硬件初始化*/
	PrintfLogo();			/*打印例程信息到串口1*/
	PrintfHelp();			/*打印帮助信息*/
	bsp_InitSM28BYJ48();	/*步进电机初始化*/	
	TFTLCD_Init();			/*LCD初始化*/
	
	Show_LCD();				/*LCD预信息显示*/
	
	bsp_StartAutoTimer(0,5000);	/*启动一个500ms的自动重装的定时器*/
	bsp_StartAutoTimer(1,100);	/*启动一个100ms的自动重装的定时器*/
	bsp_StartAutoTimer(2,10);	/*启动一个10ms的自动重装的定时器*/
	bsp_StartAutoTimer(3,100);	/*启动一个100ms的自动重装的定时器*/
	
	while(1)
	{
		if(bsp_CheckTimer(2))
		{
			usart4_cmd_receive();
			usart1_cmd_receive();
			usart2_cmd_receive();
		}
			
		if(temp>24)
			test_fun(0,0,1);//电机快转
		else
			test_fun(0,0,5);//电机慢转
		
		if(bsp_CheckTimer(0))//用于门禁和语音
		{
			bsp_InitRc522();
			PcdReset();
			M500PcdConfigISOType('A');
			IC_Verify();//门禁卡验证
		}
			
		if(bsp_CheckTimer(3)) /*判断定时器超时时间*/
		{
			LCD_ShowxNum(80+16*3,160,light,5,16,0x00);//光照值显示

			LCD_ShowxNum(80+16*3,100,(uint8_t)temp,2,16,0x00);//取温度的整数位显示
			
			LCD_ShowxNum(80+16*4.5,100,(uint8_t)(temp*10)%10,1,16,0x00);//取温度的小数点后1位显示			

			LCD_ShowxNum(80+16*3,130,(uint8_t)humid,2,16,0x00);//取湿度的整数位显示

			LCD_ShowxNum(80+16*4.5,130,(uint8_t)(humid*10)%10,1,16,0x00);//取湿度的小数点后1位显示

			LCD_ShowxNum(80+16*4.5,190,co2,4,16,0x00);//二氧化碳值显示

			LCD_ShowxNum(80+10*5,220,pm25,3,16,0x00);//PM2.5值显示			
//////////////////////////////////////////////////////////////////////////////////////////
/////*红外+震动传感器*/		
				if(pir == 1)//红外传感器检测到有人
				{
					LCD_ShowPicture(50,65,25,25,(u16*)gImage_person_2);//有人状态图标
					POINT_COLOR = RED;//字体变为红色
					LCD_ShowChinese(80,70,37,16,0);//有
					LCD_ShowChinese(80+16,70,38,16,0);//人
					LCD_ShowChinese(80+16*2,70,39,16,0);//经
					LCD_ShowChinese(80+16*3,70,40,16,0);//过				
					POINT_COLOR = 0x3333;//文字为深蓝色
					
				}
				else if(pir== 0)
				{
					LCD_ShowPicture(50,65,25,25,(u16*)gImage_person);//无人状态图标
					LCD_ShowChinese(80,70,41,16,0);//无
					LCD_ShowChinese(80+16,70,38,16,0);//人
					LCD_ShowChinese(80+16*2,70,39,16,0);//经
					LCD_ShowChinese(80+16*3,70,40,16,0);//过			
				}			
				
					LCD_ShowChinese(80,40,63,16,0);//屋
					LCD_ShowChinese(80+16,40,64,16,0);//外
					LCD_ShowChinese(80+16*2,40,65,16,0);//正
					LCD_ShowChinese(80+16*3,40,66,16,0);//常
					LCD_ShowChinese(80+16*4,40,67,16,0);//无字体覆盖
					LCD_ShowChinese(80+16*5,40,67,16,0);//无字体覆盖
					LCD_ShowChinese(80+16*6,40,67,16,0);//无字体覆盖
				
				if(vib == 1)//震动传感器检测扒窃行为
				{
					POINT_COLOR = RED;//字体变为红色					
					LCD_ShowPicture(50,35,25,25,(u16*)gImage_warning);//发生扒窃行为状态图标
					LCD_ShowChinese(80,40,55,16,0);//玻
					LCD_ShowChinese(80+16,40,56,16,0);//璃
					LCD_ShowChinese(80+16*2,40,57,16,0);//检
					LCD_ShowChinese(80+16*3,40,58,16,0);//测
					LCD_ShowChinese(80+16*4,40,59,16,0);//到
					LCD_ShowChinese(80+16*5,40,60,16,0);//震
					LCD_ShowChinese(80+16*6,40,61,16,0);//动
					Data_Verify_Calc(alarm_buf,sizeof(alarm_buf) / sizeof(alarm_buf[0]));//语音数据校验
					UART5_Send_Data(alarm_buf,sizeof(alarm_buf) / sizeof(alarm_buf[0]));//语音报警
					Uart5_STA_Clr();				
					POINT_COLOR = 0x3333;//文字恢复为深蓝色
				}
				else if(vib == 0)
				{
					LCD_ShowPicture(50,35,25,25,(u16*)gImage_Safety);//未发生扒窃行为图标
				}

//////////////////////////////////////////////////////////////////////////////////////
/*继电器+风扇*/
				if(fan_1)
				{
					LCD_ShowPicture(50,250,30,30,(u16*)gImage_FAN_ON);//风扇/开图标
					LCD_ShowChinese(80+16*3,258,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,250,30,30,(u16*)gImage_FAN_OFF);//风扇关图标
					LCD_ShowChinese(80+16*3,258,82,16,0);//关
				}
				if(fan_2)
				{
					LCD_ShowPicture(50,280,30,30,(u16*)gImage_FAN_ON);//风扇/开图标
					LCD_ShowChinese(80+16*3,288,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,280,30,30,(u16*)gImage_FAN_OFF);//风扇关图标				
					LCD_ShowChinese(80+16*3,288,82,16,0);//关					
				}
				if(relay_1)
				{
					LCD_ShowPicture(50,310,30,30,(u16*)gImage_LIGHT_ON);//灯开图标
					LCD_ShowChinese(80+16*3,318,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,310,30,30,(u16*)gImage_LIGHT_OFF);//灯关图标
					LCD_ShowChinese(80+16*3,318,82,16,0);//关
				}			
				if(relay_2)
				{
					LCD_ShowPicture(50,340,30,30,(u16*)gImage_LIGHT_ON);//灯开图标
					LCD_ShowChinese(80+16*3,348,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,340,30,30,(u16*)gImage_LIGHT_OFF);//灯关图标
					LCD_ShowChinese(80+16*3,348,82,16,0);//关	
				}
				if(relay_3)
				{
					LCD_ShowPicture(50,370,30,30,(u16*)gImage_DOOR_ON);//门锁开图标
					LCD_ShowChinese(80+16*2,378,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,370,30,30,(u16*)gImage_DOOR_OFF);//门锁关图标
					LCD_ShowChinese(80+16*2,378,82,16,0);//关					
				}
				if(relay_4)
				{
					LCD_ShowPicture(50,400,30,30,(u16*)gImage_AIR_ON);//空调开图标
					LCD_ShowChinese(80+16*2,408,81,16,0);//开
				}
				else
				{
					LCD_ShowPicture(50,400,30,30,(u16*)gImage_AIR_OFF);//空调关图标
					LCD_ShowChinese(80+16*2,408,82,16,0);//关					
				}			
				POINT_COLOR = BLACK;
				LCD_ShowString(80+24*3,390,150,16,24,(uint8_t*)LOGO_NAME);//SMART HOME	
		}	
	}
}


/*
********************************************************************************************************* 
* 函 数 名: PrintfHelp 
* 功能说明: 打印操作提示 
* 形    参：无 
* 返 回 值: 无 
********************************************************************************************************* 
*/ 
static void PrintfHelp(void) 
{ 
	printf("操作提示:\r\n"); 
	printf("1.本程序中，每300ms DS0红灯翻转一次\r\n"); 
	printf("2.请输入数据，并点击发送\r\n");
	printf("\r\n\r\n");  
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
*	函 数 名: Show_LCD
*	功能说明: 文字图标的预先显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Show_LCD()
{
	LCD_Fill(0,0,320,480,0x3333);//整个背景颜色为浅蓝色
	LCD_Fill(50,35,275,445,0xFFFF);//局部图标背景为白色
	BACK_COLOR = 0xFFFF;//前景色白色
	POINT_COLOR = 0x3333;//文字为浅蓝色
	POINT_COLOR = WHITE;//边框&圆颜色为黑色
	LCD_DrawRectangle(10,10,310,470);//绘制外边框
	LCD_DrawRectangle(5,5,315,475);//绘制内边框
	LCD_Draw_Circle(20,20,5);//绘制左上角小圆
	LCD_Draw_Circle(20,460,5);//绘制左下角小圆
	LCD_Draw_Circle(300,460,5);//绘制右下角小圆
	LCD_Draw_Circle(300,20,5);//绘制右上角小圆
	LCD_ShowPicture(230,35,47,73,(uint16_t*)gImage_logo);//团队logo
	LCD_ShowPicture(180,310,75,75,(uint16_t*)gImage_Delinc);
	
	POINT_COLOR = 0x3333;//文字为深蓝色
	LCD_ShowPicture(50,105+50,25,25,(u16*)gImage_sunlight);//湿度图标
	LCD_ShowChinese(80,160,20,16,0);//光
	LCD_ShowChinese(80+16,160,21,16,0);//照
	LCD_ShowChinese(80+16*2,160,22,16,0);//：
	LCD_ShowChinese(80+16*5.5,160,23,16,0);//：l
	LCD_ShowChinese(80+16*6,160,24,16,0);//：u
	LCD_ShowChinese(80+16*6.5,160,25,16,0);//：x			
		
	POINT_COLOR = 0x3333;//文字为浅蓝色蓝色		
	/*显示温度*/
	LCD_ShowPicture(50,95,25,25,(u16*)gImage_temperature);//温度图标
	LCD_ShowChinese(80,100,10,16,0);//温
	LCD_ShowChinese(80+16,100,11,16,0);//度
	LCD_ShowChinese(80+16*2,100,12,16,0);//：
	LCD_ShowChar(80+16*4,100,'.',16,0);//小数点
	LCD_ShowChinese(80+16*5.2,100,13,16,0);//℃
				
				
	/*显示湿度*/
	LCD_ShowPicture(50,100+25,25,25,(u16*)gImage_Humidity);//湿度图标
	LCD_ShowChinese(80,130,14,16,0);//湿
	LCD_ShowChinese(80+16,130,11,16,0);//度
	LCD_ShowChinese(80+16*2,130,12,16,0);//：
	LCD_ShowChar(80+16*4,130,'.',16,0);//小数点
	LCD_ShowChinese(80+16*5.2,130,17,16,0);//%
	LCD_ShowChinese(80+16*6,130,18,16,0);//R
	LCD_ShowChinese(80+16*6.5,130,19,16,0);//H
				
	//显示二氧化碳
	LCD_ShowPicture(50,110+75,25,25,(u16*)gImage_Co2);//二氧化碳图标
	LCD_ShowChinese(80,190,26,16,0);//二
	LCD_ShowChinese(80+16,190,27,16,0);//氧
	LCD_ShowChinese(80+16*2,190,28,16,0);//化
	LCD_ShowChinese(80+16*3,190,29,16,0);//碳
	LCD_ShowChinese(80+16*4,190,30,16,0);//：
	LCD_ShowChinese(80+16*6.5,190,31,16,0);//P
	LCD_ShowChinese(80+16*7,190,32,16,0);//P
	LCD_ShowChinese(80+16*7.5,190,33,16,0);//M
				
	//显示PM25
	LCD_ShowPicture(50,215,25,25,(u16*)gImage_PM25);//PM2.5图标
	LCD_ShowChinese(80,220,45,16,0);//P
	LCD_ShowChinese(80+10,220,46,16,0);//M
	LCD_ShowChinese(80+10*2,220,47,16,0);//2
	LCD_ShowChinese(80+10*3,220,48,16,0);//.
	LCD_ShowChinese(80+10*3.5,220,49,16,0);//5
	LCD_ShowChinese(80+10*4.5,220,30,16,0);//：			
	LCD_ShowChinese(80+16*4.8,220,50,16,0);//μ
	LCD_ShowChinese(80+16*5.5,220,51,16,0);//g
	LCD_ShowChinese(80+16*6,220,52,16,0);//'/'
	LCD_ShowChinese(80+16*6.5,220,53,16,0);//m
	LCD_ShowChinese(80+16*7,220,54,16,0);//上角标3
			
				
	//风扇1	
	LCD_ShowChinese(80,258,68,16,0);//风
	LCD_ShowChinese(80+16,258,69,16,0);//扇
	LCD_ShowChinese(80+16*2,258,70,16,0);//①
					
					
	//风扇2
	LCD_ShowChinese(80,288,68,16,0);//风
	LCD_ShowChinese(80+16,288,69,16,0);//扇
	LCD_ShowChinese(80+16*2,288,71,16,0);//②
				
	//灯1
	LCD_ShowChinese(80,318,72,16,0);//客
	LCD_ShowChinese(80+16,318,73,16,0);//厅
	LCD_ShowChinese(80+16*2,318,76,16,0);//灯
				
				
	//灯2				
	LCD_ShowChinese(80,348,74,16,0);//卧
	LCD_ShowChinese(80+16,348,75,16,0);//室
	LCD_ShowChinese(80+16*2,348,76,16,0);//灯
				
				
	//门
	LCD_ShowChinese(80,378,77,16,0);//门
	LCD_ShowChinese(80+16,378,78,16,0);//锁
				
	//空调
	LCD_ShowChinese(80,408,79,16,0);//空
	LCD_ShowChinese(80+16,408,80,16,0);//调
}



/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

