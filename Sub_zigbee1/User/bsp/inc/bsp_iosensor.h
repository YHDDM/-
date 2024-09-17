#ifndef __BSP_IOSENSOR_H 
#define __BSP_IOSENSOR_H 

#include "sys.h" 

/////////////////////////////////////////////////////////// 
//��ֲ�޸��� 
//����ʵ�ʶ˿��޸� 
//��ǰ�汾����������ֲ(ע�⣬�˿ڽ��������ڷ�JTAG/SWD���ţ������JTAG���ţ���Ҫ��AFIOʱ�ӣ���ʧ��JTAG) 



/////////////////////// 
//DO101 �𶯴����� 
#define VIBRATE_OUT_GPIO_PIN		GPIO_Pin_11  			//VIBRATE_OUT ���ź� 
#define VIBRATE_OUT_PIN_ID			11  					//VIBRATE_OUT ������� 
#define VIBRATE_OUT_GPIO_PORT		GPIOA  					//VIBRATE_OUT �˿ں� 
#define VIBRATE_OUT_GPIO_CLK		RCC_APB2Periph_GPIOA	//VIBRATE_OUT ʱ��
//#define VIBRATE_OUT_FUN_OUT  		PAout     				//VIBRATE_OUT����˿����ú���
#define VIBRATE_OUT_FUN_IN  		PAin     				//VIBRATE_OUT����˿����ú���   
#define VIBRATE_OUT_GPIO_MODE  		GPIO_Mode_IPU   		//VIBRATE_OUT�ⲿ�ж�����ģʽ 
#define VIBRATE_OUT_ACTIVE_LEVEL 	0     					//VIBRATE_OUT��Ч��ƽΪ�͵�ƽ

#define VIBRATE_OUT_PORT_SRC  		GPIO_PortSourceGPIOA  	//VIBRATE_OUT�ⲿ�ж϶˿ں�
#define VIBRATE_OUT_PIN_SRC 		GPIO_PinSource11  		//VIBRATE_OUT�ⲿ�ж����ź�
#define	VIBRATE_OUT_EXTI_LINE		EXTI_Line11				//VIBRATE_OUT�ⲿ�жϺ� 
#define	VIBRATE_OUT_EXTI_TRIG		EXTI_Trigger_Falling	//VIBRATE_OUT�ⲿ�жϴ�����ʽ
#define	VIBRATE_OUT_EXTI_IRQN		EXTI15_10_IRQn			//VIBRATE_OUT�ⲿ�ж�NVIC��
///////////////////
///PIR ������⴫���� 
#define PIR_OUT_GPIO_PIN  			GPIO_Pin_3   			//PIR_OUT���ź� 
#define PIR_OUT_PIN_ID				3    					//PIR_OUT������� 
#define PIR_OUT_GPIO_PORT			GPIOB					//PIR_OUT�˿ں�
#define PIR_OUT_GPIO_CLK			RCC_APB2Periph_GPIOB	//PIR_OUTʱ��
//#define PIR_OUT_FUN_OUT			PBout					//PIR_OUT����˿����ú��� 
#define PIR_OUT_FUN_IN				PBin					//PIR_OUT����˿����ú���
#define	PIR_OUT_GPIO_MODE			GPIO_Mode_IPD			//PIR_OUT�ⲿ�ж�����ģʽ
#define	PIR_OUT_ACTIVE_LEVEL		1						//PIR_OUT��Ч��ƽΪ�ߵ�ƽ

#define PIR_OUT_PORT_SRC			GPIO_PortSourceGPIOB	//PIR_OUT�ⲿ�ж϶˿ں�
#define PIR_OUT_PIN_SRC  			GPIO_PinSource3   		//PIR_OUT�ⲿ�ж����ź� 
#define	PIR_OUT_EXTI_LINE			EXTI_Line3				//PIR_OUT�ⲿ�жϺ�
#define	PIR_OUT_EXTI_TRIG			EXTI_Trigger_Rising		//PIR_OUT�ⲿ�жϴ�����ʽ
#define	PIR_OUT_EXTI_IRQN			EXTI3_IRQn			//PIR_OUT�ⲿ�ж�NVIC�� 


///DO111 ���洫���� 
#define FIRE_OUT_GPIO_PIN  			GPIO_Pin_12   			//FIRE_OUT���ź� 
#define FIRE_OUT_PIN_ID				12    					//FIRE_OUT������� 
#define FIRE_OUT_GPIO_PORT			GPIOA					//FIRE_OUT�˿ں�
#define FIRE_OUT_GPIO_CLK			RCC_APB2Periph_GPIOA	//FIRE_OUTʱ��
//#define FIRE_OUT_FUN_OUT			PAout					//FIRE_OUT����˿����ú��� 
#define FIRE_OUT_FUN_IN				PAin					//FIRE_OUT����˿����ú���
#define	FIRE_OUT_GPIO_MODE			GPIO_Mode_IPU			//FIRE_OUT�ⲿ�ж�����ģʽ
#define	FIRE_OUT_ACTIVE_LEVEL		0						//FIRE_OUT��Ч��ƽΪ�͵�ƽ

#define FIRE_OUT_PORT_SRC			GPIO_PortSourceGPIOA	//FIRE_OUT�ⲿ�ж϶˿ں�
#define FIRE_OUT_PIN_SRC  			GPIO_PinSource12   		//FIRE_OUT�ⲿ�ж����ź� 
#define	FIRE_OUT_EXTI_LINE			EXTI_Line12				//FIRE_OUT�ⲿ�жϺ�
#define	FIRE_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//FIRE_OUT�ⲿ�жϴ�����ʽ
#define	FIRE_OUT_EXTI_IRQN			EXTI15_10_IRQn			//FIRE_OUT�ⲿ�ж�NVIC�� 

///DO201 ��ȼ���崫���� 
#define GASES_OUT_GPIO_PIN  		GPIO_Pin_15   			//GASES_OUT���ź� 
#define GASES_OUT_PIN_ID			15    					//GASES_OUT������� 
#define GASES_OUT_GPIO_PORT			GPIOA					//GASES_OUT�˿ں�
#define GASES_OUT_GPIO_CLK			RCC_APB2Periph_GPIOA	//GASES_OUTʱ��
//#define GASES_OUT_FUN_OUT			PAout					//GASES_OUT����˿����ú��� 
#define GASES_OUT_FUN_IN			PAin					//GASES_OUT����˿����ú���
#define	GASES_OUT_GPIO_MODE			GPIO_Mode_IPU			//GASES_OUT�ⲿ�ж�����ģʽ
#define	GASES_OUT_ACTIVE_LEVEL		0						//GASES_OUT��Ч��ƽΪ�͵�ƽ

#define GASES_OUT_PORT_SRC			GPIO_PortSourceGPIOA	//GASES_OUT�ⲿ�ж϶˿ں�
#define GASES_OUT_PIN_SRC  			GPIO_PinSource15   		//GASES_OUT�ⲿ�ж����ź� 
#define	GASES_OUT_EXTI_LINE			EXTI_Line15				//GASES_OUT�ⲿ�жϺ�
#define	GASES_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//GASES_OUT�ⲿ�жϴ�����ʽ
#define	GASES_OUT_EXTI_IRQN			EXTI15_10_IRQn			//GASES_OUT�ⲿ�ж�NVIC�� 

///DO211 ���������� 
#define HALL_OUT_GPIO_PIN  			GPIO_Pin_13   			//HALL_OUT���ź� 
#define HALL_OUT_PIN_ID				13    					//HALL_OUT������� 
#define HALL_OUT_GPIO_PORT			GPIOC					//HALL_OUT�˿ں�
#define HALL_OUT_GPIO_CLK			RCC_APB2Periph_GPIOC	//HALL_OUTʱ��
//#define HALL_OUT_FUN_OUT			PAout					//HALL_OUT����˿����ú��� 
#define HALL_OUT_FUN_IN				PAin					//HALL_OUT����˿����ú���
#define	HALL_OUT_GPIO_MODE			GPIO_Mode_IPU			//HALL_OUT�ⲿ�ж�����ģʽ
#define	HALL_OUT_ACTIVE_LEVEL		0						//HALL_OUT��Ч��ƽΪ�͵�ƽ

#define HALL_OUT_PORT_SRC			GPIO_PortSourceGPIOC	//HALL_OUT�ⲿ�ж϶˿ں�
#define HALL_OUT_PIN_SRC  			GPIO_PinSource13   		//HALL_OUT�ⲿ�ж����ź� 
#define	HALL_OUT_EXTI_LINE			EXTI_Line13				//HALL_OUT�ⲿ�жϺ�
#define	HALL_OUT_EXTI_TRIG			EXTI_Trigger_Falling	//HALL_OUT�ⲿ�жϴ�����ʽ
#define	HALL_OUT_EXTI_IRQN			EXTI15_10_IRQn			//HALL_OUT�ⲿ�ж�NVIC�� 

////////////////////////////////////////////////////

//IO��������
#define VIBRATE_OUT		VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID)	//��ȡVIBRATE_OUT �͵�ƽ��Ч
#define PIR_OUT 		PIR_OUT_FUN_IN(PIR_OUT_PIN_ID)  		//��ȡPIR_OUT �ߵ�ƽ��Ч 
#define FIRE_OUT		FIRE_OUT_FUN_IN(FIRE_OUT_PIN_ID)		//��ȡFIRE_OUT �͵�ƽ��Ч
#define GASES_OUT		GASES_OUT_FUN_IN(GASES_OUT_PIN_ID)		//��ȡGASES_OUT �͵�ƽ��Ч
#define HALL_OUT		HALL_OUT_FUN_IN(HALL_OUT_PIN_ID)		//��ȡHALL_OUT �͵�ƽ��Ч

extern uint8_t VIBRATE_OUT_flag; 
extern uint8_t PIR_OUT_flag;
extern uint8_t FIRE_OUT_flag;  	
extern uint8_t HALL_OUT_flag;  
extern uint8_t GASES_OUT_flag;  	

/* ���ⲿ���õĺ������� */ 
void bsp_InitIoSensor(void); 
 
#endif



/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
