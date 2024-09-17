/*
*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : Vulcan������ת����LCD���豸���ơ�
*	�޸ļ�¼ :
*		�汾��  ����        ����    	  ˵��
*		V1.0    2024-03-26 	���໪���Ե�  �׷�
*
*	Copyright (C), 2014-2019, �������׵���
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */
#include "pic.h"				/* ͼ���ͼƬ���� */

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
#define EXAMPLE_NAME "WSNEP_V10-005_����1����ӡʵ�飩"
#define EXAMPLE_DATE "2024-03-22"
#define DEMO_VER 	 "1.0"
#define NAME		 "���໪_2350713224"
#define LOGO_NAME	 "SMART HOME"

/*�������ļ��ڵ��õĺ�������*/
static void PrintfLogo(void);
static void PrintfHelp(void);
static void Show_LCD(void);
static MOTOR_T t_moto;
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
		
	bsp_Init();				/*Ӳ����ʼ��*/
	PrintfLogo();			/*��ӡ������Ϣ������1*/
	PrintfHelp();			/*��ӡ������Ϣ*/
	bsp_InitSM28BYJ48();	/*���������ʼ��*/	
	TFTLCD_Init();			/*LCD��ʼ��*/
	
	Show_LCD();				/*LCDԤ��Ϣ��ʾ*/
	
	bsp_StartAutoTimer(0,5000);	/*����һ��500ms���Զ���װ�Ķ�ʱ��*/
	bsp_StartAutoTimer(1,100);	/*����һ��100ms���Զ���װ�Ķ�ʱ��*/
	bsp_StartAutoTimer(2,10);	/*����һ��10ms���Զ���װ�Ķ�ʱ��*/
	bsp_StartAutoTimer(3,100);	/*����һ��100ms���Զ���װ�Ķ�ʱ��*/
	
	while(1)
	{
		if(bsp_CheckTimer(2))
		{
			usart4_cmd_receive();
			usart1_cmd_receive();
			usart2_cmd_receive();
		}
			
		if(temp>24)
			test_fun(0,0,1);//�����ת
		else
			test_fun(0,0,5);//�����ת
		
		if(bsp_CheckTimer(0))//�����Ž�������
		{
			bsp_InitRc522();
			PcdReset();
			M500PcdConfigISOType('A');
			IC_Verify();//�Ž�����֤
		}
			
		if(bsp_CheckTimer(3)) /*�ж϶�ʱ����ʱʱ��*/
		{
			LCD_ShowxNum(80+16*3,160,light,5,16,0x00);//����ֵ��ʾ

			LCD_ShowxNum(80+16*3,100,(uint8_t)temp,2,16,0x00);//ȡ�¶ȵ�����λ��ʾ
			
			LCD_ShowxNum(80+16*4.5,100,(uint8_t)(temp*10)%10,1,16,0x00);//ȡ�¶ȵ�С�����1λ��ʾ			

			LCD_ShowxNum(80+16*3,130,(uint8_t)humid,2,16,0x00);//ȡʪ�ȵ�����λ��ʾ

			LCD_ShowxNum(80+16*4.5,130,(uint8_t)(humid*10)%10,1,16,0x00);//ȡʪ�ȵ�С�����1λ��ʾ

			LCD_ShowxNum(80+16*4.5,190,co2,4,16,0x00);//������ֵ̼��ʾ

			LCD_ShowxNum(80+10*5,220,pm25,3,16,0x00);//PM2.5ֵ��ʾ			
//////////////////////////////////////////////////////////////////////////////////////////
/////*����+�𶯴�����*/		
				if(pir == 1)//���⴫������⵽����
				{
					LCD_ShowPicture(50,65,25,25,(u16*)gImage_person_2);//����״̬ͼ��
					POINT_COLOR = RED;//�����Ϊ��ɫ
					LCD_ShowChinese(80,70,37,16,0);//��
					LCD_ShowChinese(80+16,70,38,16,0);//��
					LCD_ShowChinese(80+16*2,70,39,16,0);//��
					LCD_ShowChinese(80+16*3,70,40,16,0);//��				
					POINT_COLOR = 0x3333;//����Ϊ����ɫ
					
				}
				else if(pir== 0)
				{
					LCD_ShowPicture(50,65,25,25,(u16*)gImage_person);//����״̬ͼ��
					LCD_ShowChinese(80,70,41,16,0);//��
					LCD_ShowChinese(80+16,70,38,16,0);//��
					LCD_ShowChinese(80+16*2,70,39,16,0);//��
					LCD_ShowChinese(80+16*3,70,40,16,0);//��			
				}			
				
					LCD_ShowChinese(80,40,63,16,0);//��
					LCD_ShowChinese(80+16,40,64,16,0);//��
					LCD_ShowChinese(80+16*2,40,65,16,0);//��
					LCD_ShowChinese(80+16*3,40,66,16,0);//��
					LCD_ShowChinese(80+16*4,40,67,16,0);//�����帲��
					LCD_ShowChinese(80+16*5,40,67,16,0);//�����帲��
					LCD_ShowChinese(80+16*6,40,67,16,0);//�����帲��
				
				if(vib == 1)//�𶯴�������������Ϊ
				{
					POINT_COLOR = RED;//�����Ϊ��ɫ					
					LCD_ShowPicture(50,35,25,25,(u16*)gImage_warning);//����������Ϊ״̬ͼ��
					LCD_ShowChinese(80,40,55,16,0);//��
					LCD_ShowChinese(80+16,40,56,16,0);//��
					LCD_ShowChinese(80+16*2,40,57,16,0);//��
					LCD_ShowChinese(80+16*3,40,58,16,0);//��
					LCD_ShowChinese(80+16*4,40,59,16,0);//��
					LCD_ShowChinese(80+16*5,40,60,16,0);//��
					LCD_ShowChinese(80+16*6,40,61,16,0);//��
					Data_Verify_Calc(alarm_buf,sizeof(alarm_buf) / sizeof(alarm_buf[0]));//��������У��
					UART5_Send_Data(alarm_buf,sizeof(alarm_buf) / sizeof(alarm_buf[0]));//��������
					Uart5_STA_Clr();				
					POINT_COLOR = 0x3333;//���ָֻ�Ϊ����ɫ
				}
				else if(vib == 0)
				{
					LCD_ShowPicture(50,35,25,25,(u16*)gImage_Safety);//δ����������Ϊͼ��
				}

//////////////////////////////////////////////////////////////////////////////////////
/*�̵���+����*/
				if(fan_1)
				{
					LCD_ShowPicture(50,250,30,30,(u16*)gImage_FAN_ON);//����/��ͼ��
					LCD_ShowChinese(80+16*3,258,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,250,30,30,(u16*)gImage_FAN_OFF);//���ȹ�ͼ��
					LCD_ShowChinese(80+16*3,258,82,16,0);//��
				}
				if(fan_2)
				{
					LCD_ShowPicture(50,280,30,30,(u16*)gImage_FAN_ON);//����/��ͼ��
					LCD_ShowChinese(80+16*3,288,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,280,30,30,(u16*)gImage_FAN_OFF);//���ȹ�ͼ��				
					LCD_ShowChinese(80+16*3,288,82,16,0);//��					
				}
				if(relay_1)
				{
					LCD_ShowPicture(50,310,30,30,(u16*)gImage_LIGHT_ON);//�ƿ�ͼ��
					LCD_ShowChinese(80+16*3,318,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,310,30,30,(u16*)gImage_LIGHT_OFF);//�ƹ�ͼ��
					LCD_ShowChinese(80+16*3,318,82,16,0);//��
				}			
				if(relay_2)
				{
					LCD_ShowPicture(50,340,30,30,(u16*)gImage_LIGHT_ON);//�ƿ�ͼ��
					LCD_ShowChinese(80+16*3,348,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,340,30,30,(u16*)gImage_LIGHT_OFF);//�ƹ�ͼ��
					LCD_ShowChinese(80+16*3,348,82,16,0);//��	
				}
				if(relay_3)
				{
					LCD_ShowPicture(50,370,30,30,(u16*)gImage_DOOR_ON);//������ͼ��
					LCD_ShowChinese(80+16*2,378,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,370,30,30,(u16*)gImage_DOOR_OFF);//������ͼ��
					LCD_ShowChinese(80+16*2,378,82,16,0);//��					
				}
				if(relay_4)
				{
					LCD_ShowPicture(50,400,30,30,(u16*)gImage_AIR_ON);//�յ���ͼ��
					LCD_ShowChinese(80+16*2,408,81,16,0);//��
				}
				else
				{
					LCD_ShowPicture(50,400,30,30,(u16*)gImage_AIR_OFF);//�յ���ͼ��
					LCD_ShowChinese(80+16*2,408,82,16,0);//��					
				}			
				POINT_COLOR = BLACK;
				LCD_ShowString(80+24*3,390,150,16,24,(uint8_t*)LOGO_NAME);//SMART HOME	
		}	
	}
}


/*
********************************************************************************************************* 
* �� �� ��: PrintfHelp 
* ����˵��: ��ӡ������ʾ 
* ��    �Σ��� 
* �� �� ֵ: �� 
********************************************************************************************************* 
*/ 
static void PrintfHelp(void) 
{ 
	printf("������ʾ:\r\n"); 
	printf("1.�������У�ÿ300ms DS0��Ʒ�תһ��\r\n"); 
	printf("2.���������ݣ����������\r\n");
	printf("\r\n\r\n");  
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
*	�� �� ��: Show_LCD
*	����˵��: ����ͼ���Ԥ����ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Show_LCD()
{
	LCD_Fill(0,0,320,480,0x3333);//����������ɫΪǳ��ɫ
	LCD_Fill(50,35,275,445,0xFFFF);//�ֲ�ͼ�걳��Ϊ��ɫ
	BACK_COLOR = 0xFFFF;//ǰ��ɫ��ɫ
	POINT_COLOR = 0x3333;//����Ϊǳ��ɫ
	POINT_COLOR = WHITE;//�߿�&Բ��ɫΪ��ɫ
	LCD_DrawRectangle(10,10,310,470);//������߿�
	LCD_DrawRectangle(5,5,315,475);//�����ڱ߿�
	LCD_Draw_Circle(20,20,5);//�������Ͻ�СԲ
	LCD_Draw_Circle(20,460,5);//�������½�СԲ
	LCD_Draw_Circle(300,460,5);//�������½�СԲ
	LCD_Draw_Circle(300,20,5);//�������Ͻ�СԲ
	LCD_ShowPicture(230,35,47,73,(uint16_t*)gImage_logo);//�Ŷ�logo
	LCD_ShowPicture(180,310,75,75,(uint16_t*)gImage_Delinc);
	
	POINT_COLOR = 0x3333;//����Ϊ����ɫ
	LCD_ShowPicture(50,105+50,25,25,(u16*)gImage_sunlight);//ʪ��ͼ��
	LCD_ShowChinese(80,160,20,16,0);//��
	LCD_ShowChinese(80+16,160,21,16,0);//��
	LCD_ShowChinese(80+16*2,160,22,16,0);//��
	LCD_ShowChinese(80+16*5.5,160,23,16,0);//��l
	LCD_ShowChinese(80+16*6,160,24,16,0);//��u
	LCD_ShowChinese(80+16*6.5,160,25,16,0);//��x			
		
	POINT_COLOR = 0x3333;//����Ϊǳ��ɫ��ɫ		
	/*��ʾ�¶�*/
	LCD_ShowPicture(50,95,25,25,(u16*)gImage_temperature);//�¶�ͼ��
	LCD_ShowChinese(80,100,10,16,0);//��
	LCD_ShowChinese(80+16,100,11,16,0);//��
	LCD_ShowChinese(80+16*2,100,12,16,0);//��
	LCD_ShowChar(80+16*4,100,'.',16,0);//С����
	LCD_ShowChinese(80+16*5.2,100,13,16,0);//��
				
				
	/*��ʾʪ��*/
	LCD_ShowPicture(50,100+25,25,25,(u16*)gImage_Humidity);//ʪ��ͼ��
	LCD_ShowChinese(80,130,14,16,0);//ʪ
	LCD_ShowChinese(80+16,130,11,16,0);//��
	LCD_ShowChinese(80+16*2,130,12,16,0);//��
	LCD_ShowChar(80+16*4,130,'.',16,0);//С����
	LCD_ShowChinese(80+16*5.2,130,17,16,0);//%
	LCD_ShowChinese(80+16*6,130,18,16,0);//R
	LCD_ShowChinese(80+16*6.5,130,19,16,0);//H
				
	//��ʾ������̼
	LCD_ShowPicture(50,110+75,25,25,(u16*)gImage_Co2);//������̼ͼ��
	LCD_ShowChinese(80,190,26,16,0);//��
	LCD_ShowChinese(80+16,190,27,16,0);//��
	LCD_ShowChinese(80+16*2,190,28,16,0);//��
	LCD_ShowChinese(80+16*3,190,29,16,0);//̼
	LCD_ShowChinese(80+16*4,190,30,16,0);//��
	LCD_ShowChinese(80+16*6.5,190,31,16,0);//P
	LCD_ShowChinese(80+16*7,190,32,16,0);//P
	LCD_ShowChinese(80+16*7.5,190,33,16,0);//M
				
	//��ʾPM25
	LCD_ShowPicture(50,215,25,25,(u16*)gImage_PM25);//PM2.5ͼ��
	LCD_ShowChinese(80,220,45,16,0);//P
	LCD_ShowChinese(80+10,220,46,16,0);//M
	LCD_ShowChinese(80+10*2,220,47,16,0);//2
	LCD_ShowChinese(80+10*3,220,48,16,0);//.
	LCD_ShowChinese(80+10*3.5,220,49,16,0);//5
	LCD_ShowChinese(80+10*4.5,220,30,16,0);//��			
	LCD_ShowChinese(80+16*4.8,220,50,16,0);//��
	LCD_ShowChinese(80+16*5.5,220,51,16,0);//g
	LCD_ShowChinese(80+16*6,220,52,16,0);//'/'
	LCD_ShowChinese(80+16*6.5,220,53,16,0);//m
	LCD_ShowChinese(80+16*7,220,54,16,0);//�ϽǱ�3
			
				
	//����1	
	LCD_ShowChinese(80,258,68,16,0);//��
	LCD_ShowChinese(80+16,258,69,16,0);//��
	LCD_ShowChinese(80+16*2,258,70,16,0);//��
					
					
	//����2
	LCD_ShowChinese(80,288,68,16,0);//��
	LCD_ShowChinese(80+16,288,69,16,0);//��
	LCD_ShowChinese(80+16*2,288,71,16,0);//��
				
	//��1
	LCD_ShowChinese(80,318,72,16,0);//��
	LCD_ShowChinese(80+16,318,73,16,0);//��
	LCD_ShowChinese(80+16*2,318,76,16,0);//��
				
				
	//��2				
	LCD_ShowChinese(80,348,74,16,0);//��
	LCD_ShowChinese(80+16,348,75,16,0);//��
	LCD_ShowChinese(80+16*2,348,76,16,0);//��
				
				
	//��
	LCD_ShowChinese(80,378,77,16,0);//��
	LCD_ShowChinese(80+16,378,78,16,0);//��
				
	//�յ�
	LCD_ShowChinese(80,408,79,16,0);//��
	LCD_ShowChinese(80+16,408,80,16,0);//��
}



/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

