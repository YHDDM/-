/*
*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : WSNEP_V01-103_IICʵ��(OPT3001)
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-20	Lawrence  �׷�
*
*	Copyright (C), 2014-2019, �������׵���
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */
/*SN��ʼ��ַ*/
#define STM32F10X_SN_ADDR 0x1FFFF7E8

/*���������������̷�������*/
#define EXAMPLE_NAME	"WSNEP_V01-103_���ư��ۺ�ʵ��"
#define EXAMPLE_DATE	" 2024-06-20"
#define DEMO_VER		"1.0"
#define NAME			"�Ե�_2350412806"

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
	uint16_t lux = 0;
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
	
	
	
	bsp_StartAutoTimer(0,500); /*��ʱ��0 500���� �����ʪ��*/
	while(1)
	{
		bsp_Idle();//CPU����ʱִ�еĺ�������bsp.c
		

		usart2_cmd();	
		if(bsp_CheckTimer(0))		/*��ʱ��*/
		{	

			if ( VIBRATE_OUT_flag == 1 ) 
			{ 
				VIBRATE_OUT_flag = 0;         //���� 
				shake_value = 1;
			} 
			else
			{
				shake_value = 0;
			}
			if ( PIR_OUT_flag == 1 ) 
			{ 
				PIR_OUT_flag = 0;         //���� 
				red_value = 1;
				bsp_BeepOn();
			} 
			else
			{
				red_value = 0;
				bsp_BeepOff();
			}
			co2_value = CO2_READ();
			pm2_5_value = PM25_READ();
			if(co2_value!=65535 ) co2_real = co2_value;
			if(pm2_5_value!=65535 ) pm2_5_real = pm2_5_value;
			lux = GetLuxValue();
			sensor_Upload(Tem_Value,RH_Value,lux,co2_real,pm2_5_real,shake_value,red_value);
			//printf( "�¶�Ϊ%.1f��,ʪ��Ϊ%.1f%%RH,����ǿ��%dLUX,������̼%dppm,PM2.5%dppm,�𶯴�����%d,���⴫����%d\r\n",Tem_Value,RH_Value,lux,co2_value,pm2_5_real,shake_value,red_value);
			
			if(SHT3X_GetValue())
			{
				bsp_InitI2C();
				bsp_InitSht3x();
			}
			else//if(co2_value != 65535  && pm25_value != 65535)
			{
				//printf("�������ն�Ϊ%.2flux\r\n",lux);
				//printf("�¶�Ϊ%.1f�棬ʪ��Ϊ%.1f%%RH\r\n",Tem_Value,RH_Value);
				//printf("������̼Ũ��%d\r\n",co2_value);
				//printf("PM2.5Ũ��Ϊ%d\r\n",pm25_value);
			}	

		}
	}
}

	

/*
*********************************************************************************************************
*	�� �� ��: PrintfHelp
*	����˵��: ���ڴ��ڲ�����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfHelp(void)
{
	printf("������ʾ:\r\n");
	printf("1.�������У�ÿ500����DS0��Ʒ�תһ��\r\n");
	printf("2.ÿ500ms��ȡ���նȡ���ʪ�ȡ�������̼Ũ�ȡ�PM2.5Ũ�Ȳ���ʾ������Ϣ\r\n\r\n");

}
/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ���̺���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	{
		uint32_t CPU_Sn0,CPU_Sn1,CPU_Sn2;
		
		CPU_Sn0=*(__IO uint32_t*)(STM32F10X_SN_ADDR);
		CPU_Sn1=*(__IO uint32_t*)(STM32F10X_SN_ADDR + 4);
		CPU_Sn2=*(__IO uint32_t*)(STM32F10X_SN_ADDR + 8);
		
		printf("\r\nCPU:STM32F103ZET6,LQFP144,UID=%08X%08X%08X\n\r",CPU_Sn2,CPU_Sn1,CPU_Sn0);
	}
	
	printf("\r\n");
	printf("************************************************\r\n");
	printf("* ��������  :%s\r\n",EXAMPLE_NAME);
	printf("* ���̰汾  :%s\r\n",DEMO_VER);
	printf("* ��������  :%s\r\n",EXAMPLE_DATE);
	printf("* ����  :%s\r\n",NAME);
	
	
	printf("* �̼���汾:V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n",__STM32F10X_STDPERIPH_VERSION_MAIN,__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("\r\n");
	printf("***************************************\r\n");
}

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
