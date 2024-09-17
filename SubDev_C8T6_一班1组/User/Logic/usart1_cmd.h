/*
*********************************************************************************************************
*
*	ģ������ : ִ����Э������ģ��
*	�ļ����� : usart1_cmd.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-07-03
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _USART1_CMD_H_
#define _USART1_CMD_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
///////////////////////////////////////
/* �궨������ */
//֡ͷ
#define FRAME_HEADER_BYTE_1st		0x44//D�̶�֡ͷ
#define FRAME_HEADER_BYTE_2nd		0x5A//Z�̶�֡ͷ
#define FRAME_HEADER_BYTE_3rd		0x4C//L�̶�֡ͷ

#define FRAME_DATA_FIELD_OFFSET		0x05
#define FRAME_CONST_BYTE_LEN		0x06

//��λ��->��λ��������
#define FUN_CODE_CMD				0x01//������0x01
#define FUN_CODE_DEVICE_RELAY		0x01//�Ǽ̵�������

//�պϡ��Ͽ�����
#define FUN_CODE_CMD_RELAY_ON		0x01//�̵����պ�
#define FUN_CODE_CMD_RELAY_OFF		0x02//�̵����Ͽ�
#define FUN_CODE_CMD_RELAY_TOGGLE	0x03//�̵�����ת
#define FUN_CODE_CMD_RELAY_QUERY	0x04//�̵�����ѯ

//RSP
#define FUN_CODE_RSP_RELAY_ON		0x00//�̵����պ�
#define FUN_CODE_RSP_RELAY_OFF		0xFF//�̵����Ͽ�

#define FUN_CODE_DECEIVE_FAN		0x02//�����豸
#define FUN_CODE_CMD_FAN_ON			0x01//���ȿ�
#define FUN_CODE_CMD_FAN_OFF		0x02//���ȹ�
#define FUN_CODE_CMD_FAN_TOGGLE		0x03//���ȷ�ת
#define FUN_CODE_CMD_FAN_QUERY		0x04//����״̬��ѯ
#define FUN_CODE_RSP_FAN_ON		    0x00
#define FUN_CODE_RSP_FAN_OFF		0xff
///////////////////////////////////////
/* �ⲿ���������� */
///////////////////////////////////////
/* ���������� */
void usart1_cmd(void);
uint8_t CheckSum(uint8_t *buf,uint16_t len);
///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
