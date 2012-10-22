/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为WINB公司W25X16的驱动程序
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
//////FLASH命令
#define W25X_WRITE_ENABLE     0x06  //FLASH 写使能
#define W25X_WRITE_DISABLE    0x04  //写禁止
#define W25X_READ_STATUS      0X05  //读状态寄存器
#define W25X_WRITE_STATUS     0X01  //写状态寄存器
#define W25X_READ_DATA        0X03  //读数据
#define W25X_FAST_READ        0X0B  //快速读数据
#define W25X_DUAL_READ        0X3B  //双通道读数据
#define W25X_PAGE_WRITE       0X02  //写一页
#define W25X_BLOCK_ERASE      0XD8  //块擦除
#define W25X_SECTOR_ERASE     0X20  //扇区擦除
#define W25X_CHIP_ERASE       0XC7  //整个芯片擦除
#define W25X_POWER_DOWN       0XB9  //FLASH掉电
#define W25X_NULL_COMM        0X00  //空命令，用来移位
//////FLASH状态
#define W25X_IS_BUSY          0X01
#define W25X_IS_WRITE_ENABLE  0X02
#define W25X_IS_WP            0X80

#define W25X16_CS                            ((unsigned int)(1<<10))   
#define SPI_FLASH_CSR         1
//CS拉高和拉低
#define TO_W25X_CS_LOW        AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,W25X16_CS);
#define TO_W25X_CS_HIGH       AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS); 

#define WRITE_W25X_SPI(byte)  SPI_Write(AT91C_BASE_SPI,SPI_FLASH_CSR,(byte));
                             
#define READ_W25X_SPI         SPI_Read(AT91C_BASE_SPI)
//CS DE/SELECT WAIT
#define W25X_CS_LOW_START_WAIT   wait_us(46) //CS拉低到发送指令必须插入48个NOP(48M)
#define W25X_CS_STOP_HIGH_WAIT   wait_us(92) //CS拉高前必须插入48个NOP(48M)
#define W25X_WRITE_PAGE_WAIT     wait_ms(30)   //发送完写页指令后等待的时间
#define W25X_CHIP_ERASE_WAIT     wait_s(20);
#define W25X_B_AND_S_ERASE_WAIT  wait_s(1);
//------------------------------------------------------------------------------
//         定义函数
//------------------------------------------------------------------------------
extern void W25X_WriteDisable (void); //写禁止
extern void W25X_WriteEnable  (void); //写允许
extern unsigned char W25X_Read_Status (void); //判忙碌
extern void W25X_Erase(unsigned char type,unsigned long Addre24);//片擦除								 //读数据可以无限次，这里是由缓冲决定的大小
extern unsigned char flash_read (unsigned long Addre24,unsigned short length,unsigned char *buff);//多字节读FLASH(1~256)  -W25X32-000000-3FFF00(4M)-
extern unsigned char flash_write(unsigned long Addre24,unsigned short length,unsigned char *buff); //往FLASH里写一个或多个字节(小于256字节)

/*
*函数名：flash_chip_erase
*输入：
*返回值：
*作用：
*前提：
*/
void flash_chip_erase(void);

#endif