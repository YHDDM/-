/*
*********************************************************************************************************
*
*	模块名称 : 电容按键驱动模块
*	文件名称 : bsp_tpad.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-04-07 付燕华  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 变量定义区 */
__IO uint16_t tpad_default_val = 0;		//__IO限定防止编译器优化此变量

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
static void TPAD_TIM_GPIO_Config(void);
static void TPAD_TIM_Config(void);

static void bsp_TpadTimInit(void);
static void bsp_TpadReset(void);

static uint16_t Tpad_Get_val(void);
static uint16_t TPAD_Get_MaxVal( uint8_t n );

///////////////////////////////////////
/* 函数实体区 */



/*
*********************************************************************************************************
*	函 数 名: Tpad_Get_val
*	功能说明: 电容触摸按键获取充电时间,如果超时，则直接返回定时器的计数值 
*	形    参：无
*	返 回 值: 电容按键充电捕获到高电平的计数器值 
*********************************************************************************************************
*/
static uint16_t Tpad_Get_val(void)
{
	bsp_TpadReset();	//每次捕获前都需要复位
	
	while(TIM_GetFlagStatus(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx)==RESET)
	{
		//超过某一时刻，超时，将CNT的值返回
		if(TIM_GetCounter(TIMx_CAP_TIM)>(TPAD_ARR_MAX_VAL - 500))
		{
			return TIM_GetCounter(TIMx_CAP_TIM);
		}		
	}
	return TIMx_CAP_TIM_GetCapturex_FUN(TIMx_CAP_TIM);	//返回捕获值
}
/*
*********************************************************************************************************
*	函 数 名: TPAD_Get_MaxVal
*	功能说明: 电容触摸按键获取充电时间,采样多次，获取最大值
*	形    参：n 连续采样次数
*	返 回 值: 电容按键充电捕获到高电平的计数器值（n次采样的最大值） 
*********************************************************************************************************
*/
static uint16_t TPAD_Get_MaxVal( uint8_t n )
{
	uint16_t temp = 0;
	uint16_t res =0;
	while(n--)
	{
		temp = Tpad_Get_val();	//得到一次值
		if(temp > res)
		{
			res = temp;
		}
	};
	return res;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadScan
*	功能说明: 扫描触摸按键
*	形    参：n 连续采样次数
*	返 回 值: 0,没有按下;1,有按下; 
*********************************************************************************************************
*/
uint8_t bsp_TpadScan(void)
{
	static uint8_t keyen = 0; //0，可以开始检测；>0，还不能开始检测
	uint8_t res = 0;
	uint8_t sample = 3;			//默认采样3次
	uint16_t rval;
	
	rval = TPAD_Get_MaxVal(sample);
	
	if(rval > (tpad_default_val+TPAD_GATE_VAL))
	{
		if(keyen == 0)
		{
			res =1;	//keyen == 0,有效
		}
		keyen = 3;		 /* 至少要再过3次之后才能按键有效 */
	}
	if(keyen)
	{
		keyen--;
	}
	return res;
	
}
/*
*********************************************************************************************************
*	函 数 名: TPAD_TIM_GPIO_Config
*	功能说明: 电容触摸按键 GPIO 初始化函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);		 /* 使能TIMx时钟 */ 
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);	 /* 使能对应电容触摸输入端口时钟 */
	
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;			/* 电容触摸端口配置 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		/* 端口速度50MHz */ 
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;			/* 浮空输入 */ 
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);		/* 初始化触摸端口 */
}
/*
*********************************************************************************************************
*	函 数 名: TPAD_TIM_Config
*	功能说明: 电容触摸按键 定时器 初始化函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TPAD_TIM_Config(void)
{
	//结构体设置
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef			TIM_ICInitStructure;

	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	//基本定时器设置
	TIM_TimeBaseStructure.TIM_Period = TPAD_ARR_MAX_VAL;				//ARR
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_CAP_TIM_PSC;			//PSC
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上奇数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//不做时钟分割
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//不适用重复计数器，高级
	
	TIM_TimeBaseInit(TIMx_CAP_TIM,&TIM_TimeBaseStructure);
	
	
	TIM_ICInitStructure.TIM_Channel = TIMx_CAP_TIM_CHANNEL_x;//捕获通道
	TIM_ICInitStructure.TIM_ICFilter = 0x03;//硬件滤波 8次N
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//
	TIM_ICInit(TIMx_CAP_TIM,&TIM_ICInitStructure);
	
	/* 使能计数器 */ 
	TIM_Cmd( TIMx_CAP_TIM,ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadTimInit
*	功能说明: 电容触摸按键 定时器 引脚初始化函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_TpadTimInit(void)
{
	TPAD_TIM_GPIO_Config(); 
	TPAD_TIM_Config(); 
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadReset
*	功能说明: 根据TIM 得到RCC寄存器
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_TpadReset(void)
{
	//定义结构体
	GPIO_InitTypeDef		GPIO_InitStructure;
	//打开GPIO时钟
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	//打开定时器时钟
	//TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(TIMx_CAP_TIM_CH_PORT,TIMx_CAP_TIM_CH_PIN);		//通用推挽输出，设置为0，放电
	
	delay_ms( 5 );
	
	TIM_SetCounter(TIMx_CAP_TIM, 0);								//清零等待上升沿捕获
	
	//TIM_ClearFlag(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx);				//清除标识CC2
	TIM_ClearITPendingBit ( TIMx_CAP_TIM, TIMx_CAP_TIM_IT_CCx | TIMx_CAP_TIM_IT_UPDATE ); //清除中断标志 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadInit
*	功能说明: 获得空载的时候触摸按键的取值.
*	形    参：无
*	返 回 值:  0,初始化成功;1,初始化失败 
*********************************************************************************************************
*/
uint8_t bsp_TpadInit(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	bsp_TpadTimInit();
	
	for(i=0;i<10;i++)			 /* 连续读取10次 */
	{
		buf[i] = Tpad_Get_val();
		delay_ms(10);
	}
	for(i = 0;i<9;i++)			 /* 排序 */ 
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])	 /* 升序排列 */
			{
				temp = buf[i]; 
                buf[i] = buf[j]; 
                buf[j] = temp;
			}
		}
	}
	temp = 0;
	for(i=2;i<8;i++)
	{
		temp+=buf[i];		 /* 取中间的6个数据进行平均 */ 
	}
	tpad_default_val=temp/6;
	
	printf("tpad_default_val:%delay_ms\r\n",tpad_default_val);
	printf("\r\n\r\n");
	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
	{
		return 1;		 /* 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常! */ 
	}
	return 0;
}


///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
