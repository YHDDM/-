/*
*********************************************************************************************************
*
*	模块名称 : uart驱动模块
*	文件名称 : bsp_uart.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-20 付燕华  正式发布
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
uint8_t 	USART_RX_BUF[USART_REC_LEN] = {0};	//串口1 的接收缓存数组
uint8_t 	ReceiveState = 0;				//接收状态，是否是一帧
uint16_t	RxCounter = 0;					//接收长度
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 串口1初始化
*	形    参：uint32_t baud 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t baud)
{
	//1.定义结构体
	//GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//USART
	USART_InitTypeDef	USART_InitStructure;
	
	//NVIC
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	//2.打开时钟
	//打开GPIO时钟，USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//3.端口参数设置
	//写TX,PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//读RX, 端口PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.串口参数设置
	USART_InitStructure.USART_BaudRate = baud;											//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//收发模式--全双工
	USART_InitStructure.USART_Parity = USART_Parity_No;									//无奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//数据长度8位
	USART_Init(USART1,&USART_InitStructure);
	
	//5.中断参数设置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//串口1中断号
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//打开接收中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);			//打开空闲中断	
	
	//6.串口1使能
	USART_Cmd(USART1,ENABLE);								//使能串口1
	
}
/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 中断服务函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t Res = Res;
	//是否产生接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//将数据存入缓存中//自动清除中断
		USART_RX_BUF[RxCounter++] = USART1->DR;
	}
	//判断是否是空闲中断
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
	{
		//必须先读取状态寄存器SR,再读数据寄存器DR，才能清除IDLE中断
		Res = USART1->SR;
		Res = USART1->DR;
		ReceiveState = 1;	//表示接受完一帧数据
	}
}
/*
*********************************************************************************************************
*	函 数 名: Uart0_STA_Clr
*	功能说明: 清除接收状态和长度变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart0_STA_Clr(void)
{
	RxCounter = 0;
	ReceiveState = 0;
}

//对应的printf支持函数或设置
//加入一下代码，支持printf函数，而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
/*标准库需要支持函数*/
struct __FILE
{
	int handle;
};
FILE __stdout;

/*定义_sys_ezit()以避免使用半主机模式*/
void _sys_exit(int x)
{
	x= x;
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明:重定义pufc函数，这样可以使用printf函数从串口1打印输出 
*	形    参：int ch,FILE *f
*	返 回 值: ch
*********************************************************************************************************
*/

int fputc(int ch,FILE *f)
{
	while((USART1->SR & 0X40) == 0){};//循环发送，直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/*
*********************************************************************************************************
*	函 数 名: USART1_Send_Data
*	功能说明: USART1发送len个字
*	形    参：buf：发送区首地址
				len：发送的字节数 0~255
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)== RESET);
		USART_SendData(USART1,buf[t]);
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void usart1_cmd_receive()
{
	uint8_t len;
//	uint8_t t;
	
	//串口一接收数据
	if(ReceiveState)
	{
		len = RxCounter;
//		for(t=0;t<len;t++)
//		{
//		printf("%x",USART_RX_BUF[t]);
//		}
	
		//串口四发送
		USART1_Send_Data(USART_RX_BUF,RxCounter);
		Uart0_STA_Clr();
	}
}



///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
