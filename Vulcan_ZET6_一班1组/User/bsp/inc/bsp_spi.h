#ifndef __BSP_SPI_H 
#define __BSP_SPI_H 
#include "sys.h"  
 
/****************************************************************************/ 
#define RCC_SPI1   RCC_APB2PeriphClockCmd 
#define SPI1_CLK   RCC_APB2Periph_SPI1 
 
#define SPI1_GPIO_ALL_CLK  ( SPI1_SCK_GPIO_CLK | SPI1_MISO_GPIO_CLK | SPI1_MOSI_GPIO_CLK ) 
 
#define SPI1_SCK_GPIO_PIN  GPIO_Pin_5    //SPI1_SCK引脚号 
#define SPI1_SCK_GPIO_PORT  GPIOA     //SPI1_SCK端口号 
#define SPI1_SCK_GPIO_CLK  RCC_APB2Periph_GPIOA //SPI1_SCK时钟 
 
#define SPI1_MISO_GPIO_PIN  GPIO_Pin_6    //SPI1_MISO引脚号 
#define SPI1_MISO_GPIO_PORT  GPIOA     //SPI1_MISO端口号 
#define SPI1_MISO_GPIO_CLK  RCC_APB2Periph_GPIOA //SPI1_MISO时钟 
 
#define SPI1_MOSI_GPIO_PIN  GPIO_Pin_7    //SPI1_MOSI引脚号 
#define SPI1_MOSI_GPIO_PORT  GPIOA     //SPI1_MOSI端口号 
#define SPI1_MOSI_GPIO_CLK  RCC_APB2Periph_GPIOA //SPI1_MOSI时钟 
 
/****************************************************************************/ 
#define RCC_SPI2   RCC_APB1PeriphClockCmd 
#define SPI2_CLK   RCC_APB1Periph_SPI2 
 
#define SPI2_GPIO_ALL_CLK  ( SPI2_SCK_GPIO_CLK | SPI2_MISO_GPIO_CLK | SPI2_MOSI_GPIO_CLK ) 
 
#define SPI2_SCK_GPIO_PIN  GPIO_Pin_13    //SPI2_SCK引脚号 
#define SPI2_SCK_GPIO_PORT  GPIOB     //SPI2_SCK端口号 
#define SPI2_SCK_GPIO_CLK  RCC_APB2Periph_GPIOB //SPI2_SCK时钟 
 
#define SPI2_MISO_GPIO_PIN  GPIO_Pin_14    //SPI2_MISO引脚号 
#define SPI2_MISO_GPIO_PORT  GPIOB     //SPI2_MISO端口号 
#define SPI2_MISO_GPIO_CLK  RCC_APB2Periph_GPIOB //SPI2_MISO时钟 
 
#define SPI2_MOSI_GPIO_PIN  GPIO_Pin_15    //SPI2_MOSI引脚号 
#define SPI2_MOSI_GPIO_PORT  GPIOB     //SPI2_MOSI端口号 
#define SPI2_MOSI_GPIO_CLK  RCC_APB2Periph_GPIOB //SPI2_MOSI时钟 
 
/****************************************************************************/ 
 
//SPI所有操作函数 
void bsp_InitSPIx(SPI_TypeDef* SPIx);   //初始化SPIx  
void SPIx_SetCPOL_CPHA(SPI_TypeDef* SPIx,uint16_t SPI_CPOL,uint16_t SPI_CPHA); 
void SPIx_SetSpeed(SPI_TypeDef* SPIx,uint16_t SPI_BaudRatePrescaler); 
 
int32_t SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t TxData); 
int32_t SPI_ReadByte(SPI_TypeDef* SPIx, uint16_t *p_RxData); 
int32_t SPI_WriteNBytes(SPI_TypeDef* SPIx, uint8_t *p_TxData,uint32_t sendDataNum); 
int32_t SPI_ReadNBytes(SPI_TypeDef* SPIx, uint8_t *p_RxData,uint32_t readDataNum); 
uint8_t SPIx_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData); 
uint8_t SPI1_ReadWriteByte(uint8_t TxData); 
uint8_t SPI2_ReadWriteByte(uint8_t TxData); 
 
#endif 
 
/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
