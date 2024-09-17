/*
*********************************************************************************************************
*
*	ģ������ : ģ��
*	�ļ����� : .h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef __BSP_28BYJ48_H_
#define __BSP_28BYJ48_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
///////////////////////////////////////
/* �궨������ */
#define RCC_ALL_28BYJ48	(A_GPIO_CLK | B_GPIO_CLK | C_GPIO_CLK | D_GPIO_CLK)

#define A_GPIO_PIN			GPIO_Pin_4			/*A�����ź�*/
#define A_PIN_ID			4					/*A���������*/
#define A_GPIO_PORT			GPIOC				/*A��Ķ˿ں�*/
#define A_GPIO_CLK			RCC_APB2Periph_GPIOC/*A���ʱ��*/
#define A_FUN_OUT			PCout				/*A��������ú���*/
#define A_FUN_IN			PCin				/*A����������ú���*/

#define B_GPIO_PIN			GPIO_Pin_5			/*B�����ź�*/
#define B_PIN_ID			5					/*B���������*/
#define B_GPIO_PORT			GPIOC				/*B��Ķ˿ں�*/
#define B_GPIO_CLK			RCC_APB2Periph_GPIOC/*B���ʱ��*/
#define B_FUN_OUT			PCout				/*B��������ú���*/
#define B_FUN_IN			PCin				/*B����������ú���*/

#define C_GPIO_PIN			GPIO_Pin_6			/*C�����ź�*/
#define C_PIN_ID			6					/*C���������*/
#define C_GPIO_PORT			GPIOC				/*C��Ķ˿ں�*/
#define C_GPIO_CLK			RCC_APB2Periph_GPIOC/*C���ʱ��*/
#define C_FUN_OUT			PCout				/*C��������ú���*/
#define C_FUN_IN			PCin				/*C����������ú���*/

#define D_GPIO_PIN			GPIO_Pin_7			/*D�����ź�*/
#define D_PIN_ID			7					/*D���������*/
#define D_GPIO_PORT			GPIOC				/*D��Ķ˿ں�*/
#define D_GPIO_CLK			RCC_APB2Periph_GPIOC/*D���ʱ��*/
#define D_FUN_OUT			PCout				/*D��������ú���*/
#define D_FUN_IN			PCin				/*D����������ú���*/


/*IO��������*/
#define SM28BYJ48_A A_FUN_OUT(A_PIN_ID)
#define SM28BYJ48_B B_FUN_OUT(B_PIN_ID)
#define SM28BYJ48_C C_FUN_OUT(C_PIN_ID)
#define SM28BYJ48_D D_FUN_OUT(D_PIN_ID)
///////////////////////////////////////
/* �ⲿ���������� */



typedef struct
{
	uint8_t ucSpeed;	/*����ٶ�*/
	uint8_t ucDir;		/*������� 0��ת��1��ת 2ֹͣ*/
	uint8_t ucStop;		/*0 ���� 1ֹͣ*/
}MOTOR_T;

/*���������ͨ���ƶ���*/
enum
{
	SM_OFF = 0,		/*ABCD�������ͨ*/
	SM_A,			/*A�ർͨ*/
	SM_B,			/*B�ർͨ*/
	SM_C,			/*C�ർͨ*/
	SM_D,			/*D�ർͨ*/
	SM_AB,			/*AB�ർͨ*/
	SM_BC,			/*BC�ർͨ*/
	SM_CD,			/*CD�ർͨ*/
	SM_DA,			/*DA�ർͨ*/	
};



///////////////////////////////////////
/* ���������� */
void bsp_InitSM28BYJ48(void);
void motPowerOn(uint8_t _ucMode);
void motPowerOn2(uint8_t _ucMode);
void test_fun(uint8_t _ucStop,uint8_t _ucDir,uint8_t _ucSpeed);
///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
