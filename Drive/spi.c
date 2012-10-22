/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*���ļ�ΪSPI�����ļ�����Ҫʵ������ѡ�񣬷��ͣ������������ܡ�
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�

#include "type.h"
#include "drive.h"

//��������
//��������


//��������
//��������
/*
��������DR_spi_init
��������
����ֵ����
������SPI��ʼ��
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
��������DR_spi_send
��������
����ֵ����
������SPI��������
*/
void DR_spi_send(unsigned int npcs, unsigned short data)
{
  // Send data
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
  AT91C_BASE_SPI->SPI_TDR = data | SPI_PCS(npcs);
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE) == 0);
}

/*
��������DR_spi_recv
��������
����ֵ����
������SPI��������
*/
unsigned short DR_spi_recv(void)
{
 
  while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF) == 0);
  return AT91C_BASE_SPI->SPI_RDR & 0xFFFF;
}




  