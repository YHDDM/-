/*
*	������ģ��
*	2024-06-03
*	���໪
*/
#ifndef	__BSP_UART4_H
#define	__BSP_UART4_H

#include "sys.h"

#define	USART2_BUF_LEN 64	//����UART4���峤��λ64���ֽ�
extern	uint8_t	USART2_RX_BUF[USART2_BUF_LEN];	//���ջ��壬���UART4_BUF_LEN���ֽ�
extern	uint8_t	USART2_RX_CNT;					//���յ������ݳ���
extern 	uint8_t	ReceiveState2;					//����״̬���

//����봮���жϽ��գ��벻Ҫע��һ�º궨��
#define	EN_USART2_RX 	1			//0,�����ܣ�1����

void bsp_InitUart2(uint32_t bound);
void USART2_Send_Data(uint8_t *buf,uint8_t len);
void USART2_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
