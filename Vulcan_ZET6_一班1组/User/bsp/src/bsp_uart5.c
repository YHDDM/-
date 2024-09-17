/*
*********************************************************************************************************
*
*	模块名称 : UART5驱动模块
*	文件名称 : bsp_uart5.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-6-20 	赵堤  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
#include "sys.h"
///////////////////////////////////////
/* 变量定义区 */
#ifdef EN_UART5_RX							//如果使能了接收

uint8_t UART5_RX_BUF[UART5_BUF_LEN];		//接收缓存器
uint8_t UART5_RX_CNT = 0;						//
static uint8_t ReceiveState5 = 0;				//接收状态，是否是一帧
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: USART5_IRQHandler
*	功能说明: USART5中断，主要是接收中断和空闲中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART5_IRQHandler(void)														//串口5中断服务程序
{
	uint8_t res;
	
	//判断是否产生接收中断		
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET)							//接收中断（接收到1个字节的数据）
	{
		res = USART_ReceiveData(UART5);//读取接收到的数据
		//将数据存入缓存中
		if(UART5_RX_CNT < UART5_BUF_LEN)
		{
			UART5_RX_BUF[UART5_RX_CNT] = res;//记录接收到的值
			UART5_RX_CNT++;
		}
	}
	
		//判断是否是空闲中断
	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET)
	{
		//必须先读取状态寄存器SR,再读数据寄存器DR，才能清除IDLE中断
		res = UART5->SR;
		res = UART5->DR;
		ReceiveState5 = 1;	//表示接受完一帧数据
	}
}
#endif

/*
*********************************************************************************************************
*	函 数 名: Uart5_STA_Clr
*	功能说明: 清除接收状态和长度变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart5_STA_Clr(void)
{
	UART5_RX_CNT = 0;
	ReceiveState5 = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart5
*	功能说明: UART5初始化
*	形    参: bound波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart5(uint32_t bound)
{
	//1.定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//2.打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能UART5时钟
	
	
	//3.端口初始
		//TX,PC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//RX,PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	

	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5,ENABLE);//复位UART5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5,DISABLE);//停止复位
	
	#ifdef EN_UART5_RX									//如果使能了接收
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	
	USART_Init(UART5,&USART_InitStructure);//初始化串口
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//使能串口5中断								//串口1中断号							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//响应
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);									//打开接收中断
	USART_Cmd(UART5,ENABLE);	//使能串口								
	#endif												
	
}
/*
*********************************************************************************************************
*	函 数 名: UART5_Send_Data
*	功能说明: UART5发送len个字。
*	形    参: buf:发送区首地址
len:发送的字节数 ，为了和本代码的接收匹配，这里建议不要超过64个字节
*	返 回 值: 无
*********************************************************************************************************
*/
void UART5_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;	
	for(t=0;t<len;t++) //循环发送数据
	{
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
		USART_SendData(UART5,buf[t]);
	}
	
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
	UART5_RX_CNT = 0;

	
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
