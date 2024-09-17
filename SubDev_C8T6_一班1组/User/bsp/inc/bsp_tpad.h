/*
*********************************************************************************************************
*
*	ģ������ : ���ݰ���ģ��
*	�ļ����� : bsp_tpad.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-04-07	���໪
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_TPAD_H_
#define _BSP_TPAD_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
///////////////////////////////////////
/* �궨������ */

#define TIMx_CAP_TIM						TIM5
#define TIMx_CAP_TIM_APBxClock_FUN			RCC_APB1PeriphClockCmd
#define TIMx_CAP_TIM_CLK					RCC_APB1Periph_TIM5
#define	TPAD_ARR_MAX_VAL					0xFFFF
#define	TIMx_CAP_TIM_PSC					( 6 - 1 )				//��72M/6=12MHz������

//GPIO
#define	TIMx_CAP_TIM_CH_GPIO_CLK			RCC_APB2Periph_GPIOA
#define	TIMx_CAP_TIM_CH_PORT				GPIOA
#define	TIMx_CAP_TIM_CH_PIN					GPIO_Pin_1
#define	TIMx_CAP_TIM_CHANNEL_x				TIM_Channel_2			//ͨ

//�ж�
#define TIMx_CAP_TIM_IT_CCx					TIM_IT_CC2
#define TIMx_CAP_TIM_FLAG_CCx				TIM_FLAG_CC2
#define TIMx_CAP_TIM_IT_UPDATE  			TIM_IT_Update 
#define TIMx_CAP_TIM_IRQ    				TIM5_IRQn 
#define TIMx_CAP_TIM_INT_FUN 				TIM5_IRQHandler 

// ��ȡ����Ĵ���ֵ�����궨�� 
#define TIMx_CAP_TIM_GetCapturex_FUN		TIM_GetCapture2

//�����źż��Ժ����궨��
#define TIMx_CAP_TIM_OCxPolarityConfig_FUN  TIM_OC2PolarityConfig 
//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
#define TPAD_GATE_VAL						100						
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
uint8_t bsp_TpadInit(void);  	/* ����������ʼ�� */ 
uint8_t bsp_TpadScan(void); 	/* ��������ɨ�� */
///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
