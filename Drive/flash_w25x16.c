/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为W25X16驱动文件，主要实现存储芯片的初始化，读，写，擦除操作。
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件
#include "W25X16.h"


//变量引用
//变量定义
#define WRITE_W25X_SPI(byte)  DR_spi_send(SPI_FLASH_CSR,(byte));
                             
#define READ_W25X_SPI         DR_spi_recv()

//函数引用
//函数定义
/*
函数名：DR_w25x16_init
参数：无
返回值：无
描述：w25x16初始化
*/
void DR_w25x16_init()
{
  //配置SPI的MOSI和MISO，SPCK为外设，而CS0,CS1为通用I/0（FLASH的CS与SPI的不能同步）
  AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, // PIO controller base address
                      ((unsigned int) AT91C_PA13_MOSI    ) |
                        ((unsigned int) AT91C_PA12_MISO    ) |
                          ((unsigned int) AT91C_PA14_SPCK    ), // Peripheral A
                          0); // Peripheral B IS NO USE
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, W25X16_CS );
  AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS);//
  
  //SPI的控制配置
  DR_spi_init();
  
  //SPI 使能
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
}

/*
函数名：DR_w25x16_disable
参数：无
返回值：无
描述：w25x16芯片关闭，在写保护打开的情况下为防止误触发而损害数据，所以要对FLASH进行写禁止
*/
void DR_w25x16_disable()
{
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_WRITE_DISABLE);
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
}
/*
函数名：DR_w25x16_enable
参数：无
返回值：无
描述：w25x16芯片使能，当要对FLASH进行擦除，写数据时必须要先使能写，在擦除或写完后要把
写使能关闭以防止误写。
*/
void DR_w25x16_enable()
{
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_WRITE_ENABLE);
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
}

/*
函数名：DR_w25x16_get_state
参数：无
返回值：无
bit0 为W25X_IS_BUSY代表FLASH忙，0为空闲
bit1 为W25X_IS_WRITE_ENABLE代表写使能打开
bit2-bit5  受保护的扇区
bit7 为W25X_IS_WP代表WP引脚拉低，整个FLASH受保护
描述：判断FLASH是空闲还是忙
*/
unsigned char DR_w25x16_get_state()
{
  unsigned char read;  
 
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_READ_STATUS);
  WRITE_W25X_SPI(W25X_NULL_COMM);
  W25X_CS_STOP_HIGH_WAIT;
  read=READ_W25X_SPI;
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
  return read;
}

/*
函数名：DR_w25x16_erase_all
参数：无
返回值：无
描述：w25x16芯片全部擦除
*/
void DR_w25x16_erase_all()
{
  DR_w25x16_enable();//写使能打开
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
 
  WRITE_W25X_SPI(W25X_CHIP_ERASE);
 
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
  W25X_CHIP_ERASE_WAIT;
 
  DR_w25x16_disable();//写使能关闭
}

/*
函数名：DR_w25x16_erase_offset
参数：type 为擦除区域类型，W25X_CHIP_ERASE为整个FLASH擦除、W25X_BLOCK_ERASE擦除某一块、W25X_SECTOR_ERASE擦除某一扇区
Addre24 擦除的起始地址,块的话要求地址是0X10000的倍数，扇区则是0X1000.(FLASH会自己进行与，前面的倍数可以取消)
返回值：无
描述：擦除前必须要写使能。在写那些受保护的地址时命令将被忽略
擦除一个扇或块要1秒，整个FLASH要20秒。
*/
void DR_w25x16_erase_offset(unsigned char type,unsigned long Addre24)
{
  unsigned char addr23,addr15,addr7;
 
  DR_w25x16_enable();//写使能打开
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  if(W25X_CHIP_ERASE==type)//整个芯片擦除发送一个命令即可
  {
    WRITE_W25X_SPI(type);
  }
  else
  {
    WRITE_W25X_SPI(type);//发完命令后要发送地址
    WRITE_W25X_SPI(addr23);//发送地址要先发送高位
    WRITE_W25X_SPI(addr15);
    WRITE_W25X_SPI(addr7);
  }
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
 
  ////擦除后进行等待并关闭写使能
  if(W25X_CHIP_ERASE==type)
  {
    W25X_CHIP_ERASE_WAIT;
  }
  else
  {
    W25X_B_AND_S_ERASE_WAIT;
  }
  DR_w25x16_disable();
}
/*
函数名：DR_w25x16_read_status
参数：
返回值：
描述：
*/
unsigned char DR_w25x16_read_status (void)
{
  unsigned char read;   
  
  
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_READ_STATUS);
  WRITE_W25X_SPI(W25X_NULL_COMM);
  W25X_CS_STOP_HIGH_WAIT;
  read=READ_W25X_SPI;
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
  return read;
}
/*
函数名：DR_w25x16_read
参数：Addre24 24位被读数据的起始地址
length 被读数据的长度，读取数据可以从发送过去的起始地址到FLASH的最后一个字节
buff  读取到的数据存放起始地址
返回值：是否已经读成功
描述：读数据时起始地址可随机。
*/
void DR_w25x16_read(unsigned long Addre24,unsigned char *buff,unsigned short length)
{
  unsigned char addr23,addr15,addr7;
 
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
 
  if((DR_w25x16_read_status()&W25X_IS_BUSY)==W25X_IS_BUSY)//FLASH忙
    return ;
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_READ_DATA);//发送读FLASH命令
  WRITE_W25X_SPI(addr23);//发送地址要先发送高位
  WRITE_W25X_SPI(addr15);
  WRITE_W25X_SPI(addr7);
  //读数据时，发送完命令和地址后要CLK的上升延才进行地址和命令锁存，为制造一个就多发 一个字节。
  //经过测试多发一个不会造成错误。
  WRITE_W25X_SPI(W25X_NULL_COMM);
  for(int i=0;i<length;i++)
  {  
    WRITE_W25X_SPI(W25X_NULL_COMM);
    buff[i]=READ_W25X_SPI;
  }
  W25X_CS_STOP_HIGH_WAIT;
 
  TO_W25X_CS_HIGH ;
 
  return ;
}

/*
函数名：DR_w25x16_write
参数：Addre24 写数据的起始地址，当要写整个页时最低8位必须为0，否则溢出部分会从页相对位移0开始写
length 要写数据的长度，必须小于等于256
buff 要写到FLASH的数据起始地址
返回值：是否写成功
描述：往某一页里面写数据，不能跨页写，也就是写一次最多256字节，起始地址可以不是也的起始地址，当起始地址为
随机地址时要注意数据长度要小于页顶地址减随机地址。
一个页可分几次写，但被写的区域要求必须是擦除没污染过的。
*/
void DR_w25x16_write(unsigned long Addre24,unsigned char *buff,unsigned short lenght)
{
  unsigned char addr23,addr15,addr7;
  unsigned char curr_lenght;
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
  if((DR_w25x16_read_status()&W25X_IS_BUSY)==W25X_IS_BUSY)//FLASH忙
    return ;
 
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
 
  //要支持跨页写
  if((addr7+lenght)>256)
  {
    curr_lenght = 256 - addr7;
    DR_w25x16_enable();//写使能打开
    TO_W25X_CS_LOW ;
    W25X_CS_LOW_START_WAIT; 
    WRITE_W25X_SPI(W25X_PAGE_WRITE);//发送读FLASH命令
    WRITE_W25X_SPI(addr23);//发送地址要先发送高位
    WRITE_W25X_SPI(addr15);
    WRITE_W25X_SPI(addr7);
    for(int i=0;i<curr_lenght;i++)
    {
      WRITE_W25X_SPI((unsigned char )buff[i]);
    }
    W25X_CS_STOP_HIGH_WAIT;
    TO_W25X_CS_HIGH ;
    W25X_WRITE_PAGE_WAIT;
    W25X_WRITE_PAGE_WAIT;
    W25X_WRITE_PAGE_WAIT;
    DR_w25x16_disable();
    addr7 = 0;//下一页重新开始
    buff += curr_lenght;
    if (addr15 == 255)
    {
      addr15 = 0;
      addr23++;
    }
    else
    {
      addr15++;
    }
    lenght = lenght - curr_lenght;
  }
 
  DR_w25x16_enable();//写使能打开
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_PAGE_WRITE);//发送读FLASH命令
  WRITE_W25X_SPI(addr23);//发送地址要先发送高位
  WRITE_W25X_SPI(addr15);
  WRITE_W25X_SPI(addr7);
  for(int i=0;i<lenght;i++)
  {
    WRITE_W25X_SPI((unsigned char )buff[i]);
  }
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
  W25X_WRITE_PAGE_WAIT;
  DR_w25x16_disable();
  return ;
}

