/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为SPI驱动文件，主要实现配置选择，发送，接收三个功能。
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件

#include "type.h"
#include "drive.h"

//变量引用
//变量定义


//函数引用
//函数定义
/*
函数名：DR_spi_init
参数：无
返回值：无
描述：SPI初始化
*/
void DR_spi_init()
{
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_SPI;
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIDIS;
  // Execute a software reset of the SPI twice
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;
  AT91C_BASE_SPI->SPI_MR = AT91C_SPI_MSTR | AT91C_SPI_MODFDIS|(0x01<<16);

  AT91C_BASE_SPI->SPI_CSR[1] = AT_CSR(AT91B_MCK, SPI_CLK);
}

/*
函数名：DR_spi_send
参数：无
返回值：无
描述：SPI发送数据
*/
void DR_spi_send(unsigned int npcs, unsigned short data)
{
  // Send data
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
  AT91C_BASE_SPI->SPI_TDR = data | SPI_PCS(npcs);
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE) == 0);
}

/*
函数名：DR_spi_recv
参数：无
返回值：无
描述：SPI接收数据
*/
unsigned short DR_spi_recv(void)
{
 
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF) == 0);
  return AT91C_BASE_SPI->SPI_RDR & 0xFFFF;
}




  