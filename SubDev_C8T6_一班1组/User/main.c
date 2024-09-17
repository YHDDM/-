/*
*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ���ư����ݻ�ȡ�ϴ����豸����
*	�޸ļ�¼ :
*		�汾��  ����        ����     				˵��
*		V1.0    2024-05-31 ���໪�������ѡ��Ե� 	�׷�
*
*	Copyright (C), 2014-2024, �������׵���
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */

#include "usart1_cmd.h"
#include "sensor_Data_Proc.h"

/*
STM32ÿ��ϵ�ж�����Ψһ��һ��оƬ���кţ�96λbit):
	STM32F10X �ĵ�ַ��0x1FFFF7E8
	STM32F20X �ĵ�ַ��0x1FFF7A10
	STM32F30X �ĵ�ַ��0x1FFFF7AC
	STM32F40X �ĵ�ַ��0x1FFF7A10
	STM32L1XX �ĵ�ַ��0x1FF80050
*/
/*SN��ʼ��ַ*/
#define STM32F10X_SN_ADDR 0x1FFFF7E8

/*���������������̷�������*/
#define EXAMPLE_NAME "WSNEP_V01-102b_IICʵ�飨SHT30�ֽ�Э�鴫�䷽ʽ����"
#define EXAMPLE_DATE "2024-06-21"
#define DEMO_VER 	 "1.0"
#define NAME		 "���໪_2350713224"


/* ����LCD��ʾ��ع�����Ϣ */ 
#define DEV_NAME   "WSNEP_V01" 
#define LCD_EXAMPLE_NAME  "FAN TEST" 


/*�������ļ��ڵ��õĺ�������*/
static void PrintfLogo(void);
static void PrintfHelp(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	float lux = 0,temp_real=0,humid_real=0;
	uint16_t co2_value = 0; //������̼��ȡ����
	uint16_t co2_real = 0;
	uint16_t pm2_5_value = 0;//PM2.5��ȡ����
	uint16_t pm2_5_real = 0;//PM2.5��ȡ����
	uint8_t shake_value = 0;//�𶯴�����
	uint8_t red_value = 0;//���⴫����
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f10x.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ��
	*/
	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/*��ӡ������Ϣ������1*/
	PrintfHelp();	/*��ӡ��ʾ����*/	

	bsp_InitRelay();
	bsp_InitFan();
	bsp_InitIoSensor();
	bsp_InitI2C();
	bsp_InitSht3x();
	bsp_InitOpt3001();
	
	bsp_StartAutoTimer(0,100); /*��ʱ��0 500���� �����ʪ��*/
	while(1)
	{
		bsp_Idle();//CPU����ʱִ�еĺ�������bsp.c
		//bsp_BeepOn();

		usart1_cmd();	
		if(bsp_CheckTimer(0))		/*��ʱ��*/
		{	

			if(SHT3X_GetValue())
			{
				bsp_InitI2C();
				bsp_InitSht3x();
			}
			else 
			{
				temp_real = Tem_Value;
				humid_real = RH_Value;
			}	
			
			co2_value = CO2_READ();//������̼��ֵ��ȡ
			pm2_5_value = PM25_READ();//PM2.5��ֵ��ȡ
			if(co2_value!=65535 ) co2_real = co2_value;//������̼��ʵ���ݶ�ȡ
			if(pm2_5_value!=65535 ) pm2_5_real = pm2_5_value;//PM2.5��ʵ���ݶ�ȡ
			lux = GetLuxValue();//���նȻ�ȡ
			
			if ( VIBRATE_OUT_flag == 1 ) 
			{ 
				
				shake_value = 0x3F;			  //�𶯼�⵽��־λ
				VIBRATE_OUT_flag = 0;         //���� 
				
			} 
			else
			{
				shake_value = 0x00;
			}
			
			if ( PIR_OUT_flag == 1 ) 
			{ 
				
				red_value = 0x3F;		  //�����⵽��־λ
				PIR_OUT_flag = 0;         //���� 	
				bsp_BeepOn();			  //����������
			} 
			else
			{
				red_value = 0x00;	
				bsp_BeepOff();
			}		
			sensor_Upload(temp_real,humid_real,lux,co2_real,pm2_5_real,shake_value,red_value);//�����ϴ�
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ��Ϣ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void) 
{ 
 /* ���CPU ID */ 
 { 
	uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2; 
 
	CPU_Sn0 = *(__IO uint32_t*)(STM32F10X_SN_ADDR); 
	CPU_Sn1 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 4); 
	CPU_Sn2 = *(__IO uint32_t*)(STM32F10X_SN_ADDR + 8); 
 
	printf("\r\nCPU : STM32F103ZET6, LQFP144, UID = %08X %08X %08X\n\r" 
	, CPU_Sn2, CPU_Sn1, CPU_Sn0); 
 } 
 
	printf("\r\n"); 
	printf("*************************************************************\r\n"); 
	printf("* ��������   : %s\r\n", EXAMPLE_NAME); /* ��ӡ�������� */ 
	printf("* ���̰汾   : %s\r\n", DEMO_VER);  /* ��ӡ���̰汾 */ 
	printf("* ��������   : %s\r\n", EXAMPLE_DATE); /* ��ӡ�������� */ 
	printf("* �ύ��     : %s\r\n", NAME); 		/* ��ӡ�ύ�� */ 

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */ 
	printf("* �̼���汾 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", 
	__STM32F10X_STDPERIPH_VERSION_MAIN, 
	__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2); 
	printf("* \r\n"); /* ��ӡһ�пո� */ 
	printf("*************************************************************\r\n"); 
} 

/*
********************************************************************************************************* 
* �� �� ��: PrintfHelp 
* ����˵��: ��ӡ������ʾ 
* ��    �Σ��� 
* �� �� ֵ: �� 
***********************************************8+********************************************************** 
*/ 
static void PrintfHelp(void) 
{ 
	 printf("������ʾ:\r\n"); 
	 printf("1.�������У�ÿ500msDS0��Ʒ�תһ��\r\n"); 
	 printf("2.ÿ500ms��ȡ��ʾ��ʾ��ʪ����Ϣ,�ֽ�Э��\r\n"); 
	 printf("3.ͨ������KEY_UP�����л����ֽ�Э�鷽ʽ�����ͨ������KEY0�����л����ַ�����ʽ���\r\n"); 
	 printf("\r\n\r\n"); 
} 


/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

