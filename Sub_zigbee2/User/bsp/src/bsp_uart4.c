/*
*********************************************************************************************************
*
*	模块名称 : 串口驱动模块
*	文件名称 : bsp_uart4.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-03   SDZ  正式发布
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
#ifdef EN_UART4_RX		//如果使能了接收
uint8_t  UART4_RX_BUF[UART4_BUF_LEN];    //接受缓存，最大USART_REC_LEN个字节，末字节为换行符（接受缓存数组）
//接收到的数据的长度
uint8_t UART4_RX_CNT = 0;	
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: UART4中断，主要是接收中断和空闲中断
*	形    参：无
*	返 回 值: 无
*   注    意：USARTx->SR 能避免莫名其妙的错误
*********************************************************************************************************
*/
void UART4_IRQHandler(void) //串口4中断服务函数程序
{
	uint8_t res;
	//是否是接收中断
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)	//接收到数据
	{
		res = USART_ReceiveData(UART4);
		if(UART4_RX_CNT<UART4_BUF_LEN)
		{
			UART4_RX_BUF[UART4_RX_CNT] = res;
			UART4_RX_CNT++;
		}
	}
}
#endif
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart4
*	功能说明: 初始化CPU的UART4串口硬件设备
*	形    参：uint32_t baud 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart4(uint32_t bound)
{
	/*定义结构体*/
	//GPIO
	GPIO_InitTypeDef      GPIO_Instructure;
	//NVIC
	NVIC_InitTypeDef      NVIC_InitStructure;
	//USART
	USART_InitTypeDef     USART_InitStructure;
	/*打开时钟*/
	/*打开GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能GPIOC时钟
	/*打开UART4时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART4时钟
	
	/*端口初始*/
	/*配置UART Tx PA9*/
	GPIO_Instructure.GPIO_Pin = GPIO_Pin_10;//PC10
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_Init(GPIOC,&GPIO_Instructure);
	//RX,PA10
	GPIO_Instructure.GPIO_Pin = GPIO_Pin_11;//PC11
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOC,&GPIO_Instructure);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);
	
#ifdef EN_UART4_RX
	//串口参数设置
	USART_InitStructure.USART_BaudRate = bound;                                         //波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                    //收发模式---全双工
	USART_InitStructure.USART_Parity = USART_Parity_No;                                //无奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                             //1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        //数据长度8位
	USART_Init(UART4,&USART_InitStructure);
	
	//中断设置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;            //串口1中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);  //打开接收中断
	
	//串口4使能
	USART_Cmd(UART4,ENABLE);//使能串口1
#endif
}
/*
*********************************************************************************************************
*	函 数 名: UART4_Send_Data
*	功能说明: UART4发送len个字节
*	形    参：buf发送去首地址
			  len发送的字节数(建议不要超过64个字节)
*	返 回 值: 无
*********************************************************************************************************
*/
void UART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++)//循环发送数据
	{
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
		USART_SendData(UART4,buf[t]);
	}
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
	UART4_RX_CNT = 0;
}
/*
*********************************************************************************************************
*	函 数 名: UART4_Receive_Data
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART4_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART4_RX_CNT;
	uint8_t i = 0;
	*len = 0;
	delay_ms(10);
	if(rxlen==UART4_RX_CNT&&rxlen)
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i] = UART4_RX_BUF[i];
		}
		*len = UART4_RX_CNT;
		UART4_RX_CNT = 0;
	}
}

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
