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
*		V1.0    2024-04-07 lawrence  正式发布
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

//__IO uint16_t tpad_default_val = 0; 

/* volatile 限定，编译器会在每次访问status变量时都从内存中读取最新的值，
而不会使用寄存器中的缓存值。这样可以确保程序在访问status变量时能够获得最新的值，而不会出现意外的结果*/

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
static void TPAD_TIM_GPIO_Config(void);
static void TPAD_TIM_Config(void);

static void bsp_TpadTimInit(void);
static void bsp_TpadReset(void);

static uint16_t TPAD_Get_Val(void);
static uint16_t	TPAD_Get_MaxVal(uint8_t n);


///////////////////////////////////////
/* 函数实体区 */
/*tpad 空载值*/
vu16 tpad_default_val = 0;
/*
*********************************************************************************************************
*	函 数 名: TPAD_Get_Val
*	功能说明: 电容触摸按键获取充电时间，如果超时，则直接返回定时器的计数值
*	形    参: 无
*	返 回 值: 电容按键充电捕获到高电平的计数器值
*********************************************************************************************************
*/
static uint16_t TPAD_Get_Val(void)
{
	bsp_TpadReset();//每次捕获的时候，必须先复位放电
	 //等待捕获上升沿
	while(TIM_GetFlagStatus(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx) == RESET)
	{
		//一般充电时间都是在ms级别以内，很少会超过定时器的最大定时值
		if(TIM_GetCounter(TIMx_CAP_TIM) > TPAD_ARR_MAX_VAL - 500)
		{
			//超时了，直接返回CNT的值
			return TIM_GetCounter(TIMx_CAP_TIM);
		}
	};
	return TIMx_CAP_TIM_GetCapturex_FUN(TIMx_CAP_TIM);
}

/*
*********************************************************************************************************
*	函 数 名: TPAD_Get_MaxVal
*	功能说明: 电容触摸按键获取充电时间，采样多次，获取最大值
*	形    参: n连续采样次数
*	返 回 值: 电容按键充电捕获到高电平的计数器值（n次采样的最大值）
*********************************************************************************************************
*/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp = 0;
	uint16_t res = 0;
	
	while(n--)
	{
		temp = TPAD_Get_Val();//得到一次值
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
*	形    参: n 连续采样次数
*	返 回 值: 0 没有按下  1有按下
*********************************************************************************************************
*/
uint8_t bsp_TpadScan(void)
{
	static uint8_t keyen = 0;//0：可以开始检测	>0：还不能开始检测
	uint8_t res = 0;
	uint8_t sample = 3;		 //默认采样次数为3次
	uint16_t rval;
	
	rval = TPAD_Get_MaxVal(sample);
	
	/*大于tpad_default_val+TPAD_GATE_VAL，有效*/
	
	if(rval > (tpad_default_val+TPAD_GATE_VAL))
	{
		if(keyen == 0)
		{
			res = 1;//keyen==0有效
		}
		keyen = 3;	//只要要过3次才能按键有效
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
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);		//使能TIMx时钟
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);	//使能对应电容触摸输入端口时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;			//电容触摸端口设置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//端口速度 50MHz
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);		//初始化啊触摸端口
}
/*
*********************************************************************************************************
*	函 数 名: TPAD_TIM_Config
*	功能说明: 电容触摸按键 定时器  初始化函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TPAD_TIM_Config(void)
{
	//结构体设置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//捕获结构体
	TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	//打开定时器时钟
	TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);

	//基本定时器设置
	TIM_TimeBaseStructure.TIM_Period = TPAD_ARR_MAX_VAL;			//ARR设置自动重装载寄存器
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_CAP_TIM_PSC;			//PSC设置时钟预分频寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//设置时钟分割，TDTS = Tck_tim，这里不做时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				//重复计数器的值，高级的没用到，不用设置
	TIM_TimeBaseInit(TIMx_CAP_TIM,&TIM_TimeBaseStructure);			//初始化TIMx
	
	//捕获相关设置
	TIM_ICInitStructure.TIM_Channel = TIMx_CAP_TIM_CHANNEL_x;		//配置输入捕获的通道
	TIM_ICInitStructure.TIM_ICFilter = 0x03;						//8次滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler =TIM_ICPSC_DIV1;			//分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //直连映射
	TIM_ICInit(TIMx_CAP_TIM,&TIM_ICInitStructure);
	
	//使能定时器
	TIM_Cmd(TIMx_CAP_TIM,ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadTimInit
*	功能说明: 电容触摸按键  定时器  引脚初始化函数
*	形    参: 无
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
*	功能说明: 根据 TIM 得到RCC 寄存器
*	形    参: 无
*	返 回 值: TIM 外设时钟名
*********************************************************************************************************
*/
static void bsp_TpadReset(void)
{
	//定义结构体
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	//打开GPIO时钟
	RCC_APB2PeriphClockCmd(TIMx_CAP_TIM_CH_GPIO_CLK,ENABLE);
	
	//打开定时器的时钟
	//TIMx_CAP_TIM_APBxClock_FUN(TIMx_CAP_TIM_CLK,ENABLE);
	
	//结构体赋值
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = TIMx_CAP_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(TIMx_CAP_TIM_CH_PORT,TIMx_CAP_TIM_CH_PIN);//通用推挽输出，设置为0，放电
	
	delay_ms(5);//放电等待
	
	TIM_SetCounter(TIMx_CAP_TIM,0);//清零，等待上升沿捕获
	//TIM_ClearFlag(TIMx_CAP_TIM,TIMx_CAP_TIM_FLAG_CCx);//清除标志CC2
	TIM_ClearITPendingBit(TIMx_CAP_TIM,TIMx_CAP_TIM_IT_CCx | TIMx_CAP_TIM_IT_UPDATE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(TIMx_CAP_TIM_CH_PORT,&GPIO_InitStructure);
	
	//开始充电
}
/*
*********************************************************************************************************
*	函 数 名: bsp_TpadInit	初始化触摸按键
*	功能说明: 获得空载的时候触摸按键的取值
*	形    参: 无
*	返 回 值: 0，初始化成功；1初始化失败
*********************************************************************************************************
*/
uint8_t bsp_TpadInit(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	bsp_TpadTimInit();
	
	for(i=0 ; i<10 ; i++)/*连续取10次*/
	{
		buf[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	for(i = 0; i < 9; i++)//排序
	{
		for( j = i+1;j<10;j++)
		{
			if(buf[i] > buf[j])//升序排序
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
		temp += buf[i];	//取中间的6个数据进行平均
	}
	
	tpad_default_val = temp /6;
	
	printf("tpad_default_val:%d\r\n",tpad_default_val);
	printf("\r\n\r\n");
	
	if(tpad_default_val > TPAD_ARR_MAX_VAL / 2)
	{
		return 1;//初始化遇到超过 TPAD_ARR_MAX_VAL/2的数值，不正常1
	}
	return 0;
}

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
