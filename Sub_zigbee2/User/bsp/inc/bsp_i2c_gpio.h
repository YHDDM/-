/*
*********************************************************************************************************
*
*	ģ������ : I2Cģ��
*	�ļ����� : bsp_i2c_gpio.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-03	Lawrence
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H_
#define _BSP_I2C_GPIO_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"
#include "sys.h"

///////////////////////////////////////
/* �궨������ */
#define	SCL_GPIO_PIN	GPIO_Pin_0			//SCL���ź�
#define	SCL_PIN_ID		0					//SCL�������
#define	SCL_GPIO_PORT	GPIOB				//SCL�˿ں�
#define	SCL_GPIO_CLK	RCC_APB2Periph_GPIOB//SCLʱ��
#define	SCL_FUN_OUT		PBout				//SCL����˿����ú���

#define	SDA_GPIO_PIN	GPIO_Pin_1			//SDA���ź�
#define	SDA_PIN_ID		1					//SDA�������
#define	SDA_GPIO_PORT	GPIOB				//SDA�˿ں�
#define	SDA_GPIO_CLK	RCC_APB2Periph_GPIOB//SDAʱ��
#define	SDA_FUN_OUT		PBout				//SDA����˿����ú���
#define SDA_FUN_IN		PBin				//SDA����˿����ú���

//IO��������
#if SDA_PIN_ID	>= 8						//���˿ںŴ��ڵ���8���Ǹ�8λ����CRH
	#define IIC_ROLBIT						((SDA_PIN_ID - 8) * 4)
	
	#define	IIC_CLR_VAL						(~((uint32_t)0xF << IIC_ROLBIT))
	
	#define SDA_IN() {SDA_GPIO_PORT->CRH &= IIC_CLR_VAL; SDA_GPIO_PORT->CRH |= (u32)8<<IIC_ROLBIT;}
	#define SDA_OUT() {SDA_GPIO_PORT->CRH &= IIC_CLR_VAL; SDA_GPIO_PORT->CRH |= (u32)3<<IIC_ROLBIT;}
#else										//���˿ں�С��8���ǵ�8λ�� ��CRL
	#define IIC_ROLBIT						(SDA_PIN_ID * 4)
	
	#define IIC_CLR_VAL						(~((uint32_t)0xF << IIC_ROLBIT))
	
	#define SDA_IN(){SDA_GPIO_PORT->CRL &= IIC_CLR_VAL; SDA_GPIO_PORT->CRL |= (u32)8<<IIC_ROLBIT;}
	#define SDA_OUT(){SDA_GPIO_PORT->CRL &= IIC_CLR_VAL; SDA_GPIO_PORT->CRL |= (u32)3<<IIC_ROLBIT;}
#endif

//IO��������
#define IIC_SCL		SCL_FUN_OUT(SCL_PIN_ID)	//SCL
#define IIC_SDA		SDA_FUN_OUT(SDA_PIN_ID)	//SDA
#define READ_SDA	SDA_FUN_IN(SDA_PIN_ID)	//����SDA
///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
void bsp_InitI2C(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);


///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
