#include "bsp.h" 
 
 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_RCC_Configuration 
* 功能说明: 使能SPI时钟 
* 形    参: SPIx 需要使用的SPI 
* 返 回 值: 无 
********************************************************************************************************* 
*/ 
static void SPI_RCC_Configuration(SPI_TypeDef* SPIx) 
{ 
 if(SPIx==SPI1) 
 { 
  RCC_SPI1(SPI1_CLK,ENABLE);  //使能SPI1时钟 
 } 
 else 
 { 
  RCC_SPI2(SPI2_CLK,ENABLE);  //使能SPI2时钟 
 } 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_GPIO_Configuration 
* 功能说明: 配置指定SPI的引脚 
* 形    参: SPIx 需要使用的SPI 
* 返 回 值: 无 
********************************************************************************************************* 
*/ 
static void SPI_GPIO_Configuration(SPI_TypeDef* SPIx) 
{ 
 GPIO_InitTypeDef SPI_InitStructure; 
    if(SPIx==SPI1) 
 {  
  RCC_APB2PeriphClockCmd(SPI1_GPIO_ALL_CLK,ENABLE); //使能SPI1端口时钟 
   
  SPI_InitStructure.GPIO_Pin =  SPI1_SCK_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI1_SCK_GPIO_PORT, &SPI_InitStructure); 
  GPIO_SetBits(SPI1_SCK_GPIO_PORT,SPI1_SCK_GPIO_PIN);  //上拉 
   
  SPI_InitStructure.GPIO_Pin =  SPI1_MISO_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI1_MISO_GPIO_PORT, &SPI_InitStructure); 
  GPIO_SetBits(SPI1_MISO_GPIO_PORT,SPI1_MISO_GPIO_PIN);  //上拉 
   
  SPI_InitStructure.GPIO_Pin =  SPI1_MOSI_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI1_MOSI_GPIO_PORT, &SPI_InitStructure);   
  GPIO_SetBits(SPI1_MOSI_GPIO_PORT,SPI1_MOSI_GPIO_PIN);  //上拉 
   
 }else 
 { 
  RCC_APB2PeriphClockCmd(SPI2_GPIO_ALL_CLK,ENABLE); //使能SPI2端口时钟
	 SPI_InitStructure.GPIO_Pin =  SPI2_SCK_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI2_SCK_GPIO_PORT, &SPI_InitStructure); 
  GPIO_SetBits(SPI2_SCK_GPIO_PORT,SPI2_SCK_GPIO_PIN);  //上拉 
   
  SPI_InitStructure.GPIO_Pin =  SPI2_MISO_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI2_MISO_GPIO_PORT, &SPI_InitStructure); 
  GPIO_SetBits(SPI2_MISO_GPIO_PORT,SPI2_MISO_GPIO_PIN);  //上拉 
   
  SPI_InitStructure.GPIO_Pin =  SPI2_MOSI_GPIO_PIN; 
  SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
  GPIO_Init(SPI2_MOSI_GPIO_PORT, &SPI_InitStructure); 
  GPIO_SetBits(SPI2_MOSI_GPIO_PORT,SPI2_MOSI_GPIO_PIN);  //上拉 
 }  
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPIx_SetCPOL_CPHA 
* 功能说明: 配置指定SPI CPOL极性 CPHA 采样时钟的位置 
* 形    参:  SPIx 需要使用的SPI 
*    CPOL Set:SPI_CPOL_Low,SPI_CPOL_High 
*    CPHA Set:SPI_CPHA_1Edge,SPI_CPHA_2Edge 
* 返 回 值: 无 
* 注    意：必须在bsp_InitSPIx函数后调用 
********************************************************************************************************* 
*/ 
void SPIx_SetCPOL_CPHA(SPI_TypeDef* SPIx,uint16_t SPI_CPOL,uint16_t SPI_CPHA) 
{ 
 assert_param(IS_SPI_ALL_PERIPH(SPIx)); 
 assert_param(IS_SPI_CPOL(SPI_CPOL)); 
 assert_param(IS_SPI_CPHA(SPI_CPHA)); 
  
 //设置SPIx串行同步时钟的空闲状态为?电平  
 if(SPI_CPOL == SPI_CPOL_High) 
 { 
  SPIx->CR1 |= SPI_CPOL_High; 
 } 
 else 
 { 
  SPIx->CR1 &= (uint16_t)~SPI_CPOL_High; 
 } 
  
 //设置SPIx串行同步时钟的第?个跳变沿（上升或下降）数据被采样 
 if(SPI_CPHA == SPI_CPHA_2Edge ) 
 { 
  SPIx->CR1 |= SPI_CPHA_2Edge; 
 } 
 else 
 { 
  SPIx->CR1 &= (uint16_t)~SPI_CPHA_2Edge; 
 } 
  
 SPI_Cmd(SPIx,ENABLE);  
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPIx_SetSpeed 
* 功能说明: SPI 速度设置函数 
* 形    参:  SPIx 需要使用的SPI 
*    SpeedSet: 
*       SPI_BaudRatePrescaler_2   2分频    
*       SPI_BaudRatePrescaler_8   8分频    
*       SPI_BaudRatePrescaler_16  16分频   
*       SPI_BaudRatePrescaler_256 256分频  
* 返 回 值: 无 
* 注    意：必须在bsp_InitSPIx函数后调用 
********************************************************************************************************* 
*/ 
void SPIx_SetSpeed(SPI_TypeDef* SPIx,uint16_t SPI_BaudRatePrescaler)
	{  
 assert_param(IS_SPI_ALL_PERIPH(SPIx)); 
 assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); 
  
 SPIx->CR1 &= (uint16_t)~SPI_BaudRatePrescaler_256;  //只清除速度设置位  
  
 SPIx->CR1 |= SPI_BaudRatePrescaler;      //设置SPIx速度  
  
 SPI_Cmd(SPIx,ENABLE);  
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: bsp_InitSPIx 
* 功能说明: 配置SPIx相关的内容。 
* 形    参: SPIx 需要使用的SPI 
* 返 回 值: 无 
********************************************************************************************************* 
*/ 
void bsp_InitSPIx(SPI_TypeDef* SPIx) 
{           
 SPI_InitTypeDef SPI_InitStructure; 
 
 SPI_RCC_Configuration(SPIx); 
  
 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工 
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI 
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //设置SPI的数据大小:SPI发送接收8位帧结构 
  
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //串行同步时钟的空闲状态为低电平 
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //串行同步时钟的第一个跳变沿（上升或下降）数据被采样 
  
// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;        //串行同步时钟的空闲状态为高电平 
// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;       //串行同步时钟的第二个跳变沿（上升或下降）数据被采样 
  
  
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256 
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 
 SPI_InitStructure.SPI_CRCPolynomial = 7;        //CRC值计算的多项式 
 SPI_Init(SPIx, &SPI_InitStructure);           //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器 
 
  
 SPI_GPIO_Configuration(SPIx);           //端口配置 
 SPI_Cmd(SPIx, ENABLE);             //使能SPIx 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_WriteByte 
* 功能说明: 写1字节数据到SPI总线 
* 形    参: TxData 写到总线的数据 
* 返 回 值: 数据发送状态:0 数据发送成功,-1 数据发送失败 
********************************************************************************************************* 
*/ 
int32_t SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t TxData) 
{ 
 uint8_t retry=0; 
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位   
 { 
  retry++; 
  if(retry>200)return -1;
	 }      
 SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据  
 retry=0; 
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位 
 { 
  retry++; 
  if(retry>200)return -1; 
 }           
 return 0;              //返回正确发送 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_ReadByte 
* 功能说明: 从SPI总线读取1字节数据 
* 形    参: p_RxData 数据储存地址 
* 返 回 值: 数据读取状态:0 数据发送成功,-1 数据发送失败 
********************************************************************************************************* 
*/ 
int32_t SPI_ReadByte(SPI_TypeDef* SPIx, uint16_t *p_RxData) 
{ 
 uint8_t retry=0;      
 //while((SPIx->SR&SPI_I2S_FLAG_TXE)==0)    //等待发送区空 
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位 
 { 
  retry++; 
  if(retry>200)return -1; 
 }  
 SPI_I2S_SendData(SPIx, 0xFF);   //通过外设SPIx发送一个dumy数据  
 retry=0; 
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位 
 { 
  retry++; 
  if(retry>200)return -1; 
 } 
 *p_RxData = SPI_I2S_ReceiveData(SPIx);             
 return 0;              //返回正确接收 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_WriteNBytes 
* 功能说明: 向SPI总线写多字节数据 
* 形    参:  p_TxData 发送数据缓冲区首地址 
*    sendDataNum 发送数据字节数 
* 返 回 值: 数据发送状态:0 数据发送成功,-1 数据发送失败 
********************************************************************************************************* 
*/ 
int32_t SPI_WriteNBytes(SPI_TypeDef* SPIx, uint8_t *p_TxData,uint32_t sendDataNum) 
{ 
 uint16_t retry=0; 
 while(sendDataNum--) 
 { 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位 
  { 
   retry++; 
   if(retry>20000)return -1; 
  } 
  SPI_I2S_SendData(SPIx, *p_TxData++); //通过外设SPIx发送一个数据  
  retry=0;  
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位 
  { 
   retry++; 
   if(retry>20000)return -1; 
  } 
 } 
 return 0;//返回正确发送 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI_ReadNBytes 
* 功能说明: 从SPI总线读取多字节数据 
* 形    参:  p_RxData 数据储存地址 
*    readDataNum 读取数据字节数 
* 返 回 值: 数据读取状态:0 数据发送成功,-1 数据发送失败 
********************************************************************************************************* 
*/ 
int32_t SPI_ReadNBytes(SPI_TypeDef* SPIx, uint8_t *p_RxData,uint32_t readDataNum) 
{ 
 uint16_t retry=0; 
 while(readDataNum--) 
 { 
  SPI_I2S_SendData(SPIx, 0xFF);   //通过外设SPIx发送一个dumy数据 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位 
  { 
   retry++; 
   if(retry>20000)return -1; 
  } 
  retry = 0; 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位 
  { 
   retry++; 
   if(retry>20000)return -1; 
  } 
  *p_RxData++ = SPI_I2S_ReceiveData(SPIx); 
 }  
 return 0;//返回正确接收 
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPIx_ReadWriteByte 
* 功能说明: 从SPI总线读写一个字节 
* 形    参:  SPIx 需要使用的SPI 
*    TxData:要写入的字节 
* 返 回 值: 读取到的字节 
********************************************************************************************************* 
*/ 
uint8_t SPIx_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData) 
{   
 u8 retry=0;       
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位 
 { 
  retry++; 
  if(retry>200)return 0; 
 }      
 SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据 
 retry=0; 
 
 while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位 
 { 
  retry++; 
  if(retry>200)return 0; 
 }              
 return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据          
} 
 
/* 
********************************************************************************************************* 
* 函 数 名: SPI1_ReadWriteByte 
* 功能说明: 从SPI总线读写一个字节 
* 形    参: TxData:要写入的字节 
* 返 回 值: 读取到的字节 
********************************************************************************************************* 
*/ 
uint8_t SPI1_ReadWriteByte(uint8_t TxData) 
{ 
 uint8_t temp; 
 temp = SPIx_ReadWriteByte(SPI1,TxData); 
 return temp; 
} 

/* 
********************************************************************************************************* 
* 函 数 名: SPI2_ReadWriteByte 
* 功能说明: 从SPI总线读写一个字节 
* 形    参: TxData:要写入的字节 
* 返 回 值: 读取到的字节 
********************************************************************************************************* 
*/ 
uint8_t SPI2_ReadWriteByte(uint8_t TxData) 
{ 
 uint8_t temp; 
 temp = SPIx_ReadWriteByte(SPI2,TxData); 
 return temp; 
} 
 
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
