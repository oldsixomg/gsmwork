/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *���ļ�ΪWINB��˾W25X16����������
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef W25X16_H
#define W25X16_H
#include <includes.h>
#include "spi.h"
#include "CC110X.h"
//////FLASH����
#define W25X_WRITE_ENABLE     0x06  //FLASH дʹ��
#define W25X_WRITE_DISABLE    0x04  //д��ֹ
#define W25X_READ_STATUS      0X05  //��״̬�Ĵ���
#define W25X_WRITE_STATUS     0X01  //д״̬�Ĵ���
#define W25X_READ_DATA        0X03  //������
#define W25X_FAST_READ        0X0B  //���ٶ�����
#define W25X_DUAL_READ        0X3B  //˫ͨ��������
#define W25X_PAGE_WRITE       0X02  //дһҳ
#define W25X_BLOCK_ERASE      0XD8  //�����
#define W25X_SECTOR_ERASE     0X20  //��������
#define W25X_CHIP_ERASE       0XC7  //����оƬ����
#define W25X_POWER_DOWN       0XB9  //FLASH����
#define W25X_NULL_COMM        0X00  //�����������λ
//////FLASH״̬
#define W25X_IS_BUSY          0X01
#define W25X_IS_WRITE_ENABLE  0X02
#define W25X_IS_WP            0X80

#define W25X16_CS                            ((unsigned int)(1<<10))   
#define SPI_FLASH_CSR         1
//CS���ߺ�����
#define TO_W25X_CS_LOW        AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,W25X16_CS);
#define TO_W25X_CS_HIGH       AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS); 

#define WRITE_W25X_SPI(byte)  SPI_Write(AT91C_BASE_SPI,SPI_FLASH_CSR,(byte));
                             
#define READ_W25X_SPI         SPI_Read(AT91C_BASE_SPI)
//CS DE/SELECT WAIT
#define W25X_CS_LOW_START_WAIT   wait_us(46) //CS���͵�����ָ��������48��NOP(48M)
#define W25X_CS_STOP_HIGH_WAIT   wait_us(92) //CS����ǰ�������48��NOP(48M)
#define W25X_WRITE_PAGE_WAIT     wait_ms(30)   //������дҳָ���ȴ���ʱ��
#define W25X_CHIP_ERASE_WAIT     wait_s(20);
#define W25X_B_AND_S_ERASE_WAIT  wait_s(1);
//------------------------------------------------------------------------------
//         ���庯��
//------------------------------------------------------------------------------
extern void W25X_WriteDisable (void); //д��ֹ
extern void W25X_WriteEnable  (void); //д����
extern unsigned char W25X_Read_Status (void); //��æµ
extern void W25X_Erase(unsigned char type,unsigned long Addre24);//Ƭ����								 //�����ݿ������޴Σ��������ɻ�������Ĵ�С
extern unsigned char flash_read (unsigned long Addre24,unsigned short length,unsigned char *buff);//���ֽڶ�FLASH(1~256)  -W25X32-000000-3FFF00(4M)-
extern unsigned char flash_write(unsigned long Addre24,unsigned short length,unsigned char *buff); //��FLASH��дһ�������ֽ�(С��256�ֽ�)

/*
*��������flash_chip_erase
*���룺
*����ֵ��
*���ã�
*ǰ�᣺
*/
void flash_chip_erase(void);

#endif