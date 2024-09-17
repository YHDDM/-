/*
*********************************************************************************************************
*
*	模块名称 : 串口4驱动模块
*	文件名称 : bsp_uart4.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-07-01  赵堤  正式发布
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
uint8_t		USART4_RX_BUF[UART4_BUF_LEN]={0};					//接收缓存，最大USART_REC_LEN个字节，末字节为换行符
uint8_t		ReceiveState4=0;									//接收状态标记,1时接收完整一帧
uint16_t	RxCounter4=0;										//接收长度
uint8_t 	relay_1,relay_2,relay_3,relay_4,fan_1,fan_2,vib,pir;//截取自制板的帧数据
float 		temp,humid,light,co2,pm25;							//截取自制板数据
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */

/*
*********************************************************************************************************
* 函 数 名: UART4_IRQHandler
* 功能说明: 串口 4 中断。
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
	uint8_t res;
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //接收到数据
	{	
		res =USART_ReceiveData(UART4); //读取接收到的数据
		if(RxCounter4<UART4_BUF_LEN)
		{
			USART4_RX_BUF[RxCounter4]=res; //记录接收到的值
			RxCounter4++; //接收数据增加 1
		}
		ReceiveState4=1;//标记接收状态置位，表示接受完1帧数据
	}
}

/*
*********************************************************************************************************
*	函 数 名: Uart4_STA_Clr
*	功能说明: 串口4状态清除函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart4_STA_Clr(void)
{
	RxCounter4=0;
	ReceiveState4=0;
}
/*
*********************************************************************************************************
* 函 数 名: bsp_InitUart4
* 功能说明: 初始化 CPU 的 UART4 串口硬件设备。
* 形 参：uint32_t bound:波特率
* 返 回 值: 无
*********************************************************************************************************
*/

void bsp_InitUart4(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能 GPIOC 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能 UART4 时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);//复位串口 4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);//停止复位
	
#ifdef EN_UART4_RX //如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	
	USART_Init(UART4, &USART_InitStructure); ; //初始化串口
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //使能串口 4 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
	
	USART_Cmd(UART4, ENABLE); //使能串口
#endif
}

/*
*********************************************************************************************************
* 函 数 名: UART4_Send_Data
* 功能说明: UART4 发送 len 个字节
* 形 参： buf:发送区首地址
* len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过 64 个字节)
* 返 回 值: 无
*********************************************************************************************************
*/
void USART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //循环发送数据
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
		USART_SendData(UART4,buf[t]);
	}
	
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}
/*
*********************************************************************************************************
*	函 数 名: usart4_cmd_receive
*	功能说明: 串口四接收，串口一发送
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void usart4_cmd_receive()
{	
	//串口四接收数据
	if(ReceiveState4)
	{
		
		USART1_Send_Data(USART4_RX_BUF,RxCounter4);

		if(USART4_RX_BUF[8] == 0x22 && USART4_RX_BUF[9] == 0x00) //为传感器类型设备
		{
			if(USART4_RX_BUF[10] == 0x01 && USART4_RX_BUF[11] == 0x02)//温度传感器正常工作
			{
				*((char *)(&temp)) 	   = USART4_RX_BUF[15];
				*((char *)(&temp) + 1) = USART4_RX_BUF[14];
				*((char *)(&temp) + 2) = USART4_RX_BUF[13];
				*((char *)(&temp) + 3) = USART4_RX_BUF[12];					
			}
			
			if(USART4_RX_BUF[18] == 0x02 && USART4_RX_BUF[19] == 0x02)//湿度传感器正常
			{
				
				*((char *)(&humid))     = USART4_RX_BUF[23];
				*((char *)(&humid) + 1) = USART4_RX_BUF[22];
				*((char *)(&humid) + 2) = USART4_RX_BUF[21];
				*((char *)(&humid) + 3) = USART4_RX_BUF[20];
			}
			
			if(USART4_RX_BUF[26] == 0x04 && USART4_RX_BUF[27] == 0x02)//光照传感器正常
			{					
				*((char *)(&light))     = USART4_RX_BUF[31];
				*((char *)(&light) + 1) = USART4_RX_BUF[30];
				*((char *)(&light) + 2) = USART4_RX_BUF[29];
				*((char *)(&light) + 3) = USART4_RX_BUF[28];
			}
			

			if(USART4_RX_BUF[34] == 0x05 && USART4_RX_BUF[35] == 0x02)//二氧化碳传感器正常
			{
				*((char *)(&co2))     = USART4_RX_BUF[39];
				*((char *)(&co2) + 1) = USART4_RX_BUF[38];
				*((char *)(&co2) + 2) = USART4_RX_BUF[37];
				*((char *)(&co2) + 3) = USART4_RX_BUF[36];				
			}	

			if(USART4_RX_BUF[42] == 0x06 && USART4_RX_BUF[43] == 0x02)//PM2.5
			{
				*((char *)(&pm25))     = USART4_RX_BUF[47];
				*((char *)(&pm25) + 1) = USART4_RX_BUF[46];
				*((char *)(&pm25) + 2) = USART4_RX_BUF[45];
				*((char *)(&pm25) + 3) = USART4_RX_BUF[44];	
			}
			
			if(USART4_RX_BUF[50] == 0x03)//震动
			{
				if(USART4_RX_BUF[52] == 0x3F || USART4_RX_BUF[53] == 0x3F || USART4_RX_BUF[54] == 0x3F || USART4_RX_BUF[55] == 0x3F)
				{
					vib = 1;//震动
				}	
				else 
				{
					vib = 0;
				}						
			}		
			if(USART4_RX_BUF[58] == 0x07)//红外
			{
				if(USART4_RX_BUF[60] == 0x3F)
					pir = 1;
				else
					pir = 0;
			}						
		}	
		Uart4_STA_Clr();
	}
}

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
