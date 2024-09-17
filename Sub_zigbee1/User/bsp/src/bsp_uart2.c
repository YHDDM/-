/*
*********************************************************************************************************
*
*	模块名称 : UART2驱动模块
*	文件名称 : bsp_uart2.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-6-20	赵堤  正式发布
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
#ifdef EN_UART2_RX							//如果使能了接收

uint8_t UART2_RX_BUF[UART2_BUF_LEN];		//接收缓存器
uint8_t UART2_RX_CNT = 0;						//接收到的数据长度
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: USART2_IRQHandler
*	功能说明: USART2中断，主要是接收中断和空闲中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_IRQHandler(void)														//串口2中断服务程序
{
	uint8_t res;
	
	//判断是否产生接收中断		
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)							//接收中断（接收到1个字节的数据）
	{
		res = USART_ReceiveData(USART2);//读取接收到的数据
		//将数据存入缓存中
		if(UART2_RX_CNT < UART2_BUF_LEN)
		{
			UART2_RX_BUF[UART2_RX_CNT] = res;//记录接收到的值
			UART2_RX_CNT++;
		}
	}
}
#endif
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart2
*	功能说明: UART2初始化
*	形    参: bound波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart2(uint32_t bound)
{
	//1.定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//2.打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	
	//3.端口初始
	//TX,PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//RX,PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);							//复位UART5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);							//停止复位
	
	#ifdef EN_UART2_RX																//如果使能了接收
	
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						    //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;							    //奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	
	USART_Init(USART2,&USART_InitStructure);											//初始化串口
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//串口2中断号							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;						//抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;								//响应
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);										//打开接收中断
	USART_Cmd(USART2,ENABLE);														//使能串口								
	#endif												
	
}
/*
*********************************************************************************************************
*	函 数 名: UART2_Send_Data
*	功能说明: UART2发送len个字。
*	形    参: buf:发送区首地址
len:发送的字节数 ，为了和本代码的接收匹配，这里建议不要超过64个字节
*	返 回 值: 无
*********************************************************************************************************
*/
void UART2_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //循环发送数据
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	UART2_RX_CNT = 0;
}
/*
*********************************************************************************************************
*	函 数 名: UART2_Receive_Data
*	功能说明: UART2查询接收到的数据
*	形    参：buf：接收缓存首地址
			  len:读到的数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void UART2_Receive_Data(uint8_t *buf,uint8_t *len)
{
	uint8_t rxlen = UART2_RX_CNT;
	uint8_t i=0;
	
	*len = 0;			//默认为0
	delay_ms(10);		//等待10ms，连续超过10ms没有接收到一个数据，则认为接收结束
	if(rxlen==UART2_RX_CNT && rxlen)//接收到了数据，且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i] = UART2_RX_BUF[i];
		}
		*len = UART2_RX_CNT;
		UART2_RX_CNT = 0;//清零
	}
}

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
