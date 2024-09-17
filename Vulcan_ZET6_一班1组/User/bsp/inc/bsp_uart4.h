/*
*********************************************************************************************************
*
*	ģ������ : ģ��
*	�ļ����� : bsp_uart4.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef __BSP_UART4_H
#define __BSP_UART4_H

///////////////////////////////////////
/* ͷ�ļ������� */

#include "bsp.h"



///////////////////////////////////////
/* �궨������ */
#define UART4_BUF_LEN 		65 				//���� UART4 ���峤��Ϊ 64 ���ֽ�
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_UART4_RX 		1 				//0,������;1,����.

///////////////////////////////////////
/* �ⲿ���������� */
extern uint8_t	USART4_RX_BUF[UART4_BUF_LEN];	//���ջ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
extern uint8_t	ReceiveState4;					//����״̬���
extern uint16_t	RxCounter4;						//���ճ���
extern float temp,humid,light,co2,pm25;
extern uint8_t relay_1,relay_2,relay_3,relay_4,fan_1,fan_2,vib,pir;

///////////////////////////////////////
/* ���������� */

void bsp_InitUart4(uint32_t bound);
void Uart4_STA_Clr(void);
void USART4_Send_Data(uint8_t *buf,uint8_t len);
void usart4_cmd_receive(void);

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

