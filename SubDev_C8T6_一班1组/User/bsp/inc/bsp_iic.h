/*
*********************************************************************************************************
*
*	ģ������ : IICģ��
*	�ļ����� : bsp_iic.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-05-31	���໪
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_IIC_H_
#define _BSP_IIC_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "sys.h"

/////////////////////////////////////////////////////////// 
//��ֲ�޸��� 
//����ʵ�ʶ˿��޸� 
//��ǰ�汾����������ֲ(ע�⣬�˿ڽ��������ڷ�JTAG/SWD���ţ������JTAG���ţ���Ҫ��AFIOʱ�ӣ���ʧ��JTAG) 


///////////////////////////////////////
/* �궨������ */


#define	SCL_GPIO_PIN	GPIO_Pin_0			//SCL ���ź�
#define	SCL_PIN_ID		0				//SCL �������
#define	SCL_GPIO_PORT	GPIOB				//SCL �˿ں�
#define	SCL_GPIO_CLK	RCC_APB2Periph_GPIOB//SCL ʱ�� 
#define	SCL_FUN_OUT		PBout 				//SCL ����˿����ú��� 
#define SCL_FUN_IN 	  	PBin				//SCL ����˿����ú��� 

///
#define	SDA_GPIO_PIN	GPIO_Pin_1			//SDA ���ź�
#define	SDA_PIN_ID		1					//SDA �������
#define	SDA_GPIO_PORT	GPIOB				//SDA �˿ں�
#define	SDA_GPIO_CLK	RCC_APB2Periph_GPIOB//SDA ʱ�� 
#define	SDA_FUN_OUT		PBout 				//SDA ����˿����ú��� 
#define SDA_FUN_IN   	PBin				//SDA ����˿����ú��� 



///////////////////////////

//IO�������
#if	SDA_PIN_ID>=8		//���˿ںŴ��ڵ���8���Ǹ�8λ�� ��CRH 
	
	#define	IIC_ROLBIT	((SDA_PIN_ID-8)*4)
	
	#define	IIC_CLR_VAL	(~((uint32_t)0XF<<IIC_ROLBIT))
	
	#define SDA_IN()    {SDA_GPIO_PORT->CRH&=IIC_CLR_VAL;SDA_GPIO_PORT->CRH|=(u32)8<<IIC_ROLBIT;} 
	#define SDA_OUT()   {SDA_GPIO_PORT->CRH&=IIC_CLR_VAL;SDA_GPIO_PORT->CRH|=(u32)3<<IIC_ROLBIT;}
#else
	#define	IIC_ROLBIT	(SDA_PIN_ID*4)
	
	#define	IIC_CLR_VAL	( ~( (uint32_t)0XF << IIC_ROLBIT ) ) 
	
	#define SDA_IN()    {SDA_GPIO_PORT->CRL&=IIC_CLR_VAL;SDA_GPIO_PORT->CRL|=(u32)8<<IIC_ROLBIT;} 
	#define SDA_OUT()   {SDA_GPIO_PORT->CRL&=IIC_CLR_VAL;SDA_GPIO_PORT->CRL|=(u32)3<<IIC_ROLBIT;}

#endif

//IO��������
#define IIC_SCL      	SCL_FUN_OUT(SCL_PIN_ID) //SCL 
#define IIC_SDA     	SDA_FUN_OUT(SDA_PIN_ID) //SDA   
#define READ_SDA   	 	SDA_FUN_IN(SDA_PIN_ID)  //����SDA 

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
void bsp_InitI2C(void);               //��ʼ��IIC��IO��
void IIC_Start(void);    //����IIC��ʼ�ź� 
void IIC_Stop(void);      //����IICֹͣ�ź� 
void IIC_Send_Byte(uint8_t txd);   //IIC����һ���ֽ� 
uint8_t IIC_Read_Byte(unsigned char ack); 
uint8_t IIC_Wait_Ack(void);    //IIC�ȴ�ACK�ź� 
void IIC_Ack(void);     //IIC����ACK�ź� 
void IIC_NAck(void);    //IIC������ACK�ź�
///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

