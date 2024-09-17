/*
*	串口四模块
*	2024-06-03
*	付燕华
*/
#ifndef	__BSP_UART4_H
#define	__BSP_UART4_H

#include "sys.h"

#define	USART2_BUF_LEN 64	//定义UART4缓冲长度位64个字节
extern	uint8_t	USART2_RX_BUF[USART2_BUF_LEN];	//接收缓冲，最大UART4_BUF_LEN个字节
extern	uint8_t	USART2_RX_CNT;					//接收到的数据长度
extern 	uint8_t	ReceiveState2;					//接收状态标记

//如果想串口中断接收，请不要注释一下宏定义
#define	EN_USART2_RX 	1			//0,不接受，1接收

void bsp_InitUart2(uint32_t bound);
void USART2_Send_Data(uint8_t *buf,uint8_t len);
void USART2_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
