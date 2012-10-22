/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*���ļ�ΪW25X16�����ļ�����Ҫʵ�ִ洢оƬ�ĳ�ʼ��������д������������
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�
#include "W25X16.h"


//��������
//��������
#define WRITE_W25X_SPI(byte)  DR_spi_send(SPI_FLASH_CSR,(byte));
                             
#define READ_W25X_SPI         DR_spi_recv()

//��������
//��������
/*
��������DR_w25x16_init
��������
����ֵ����
������w25x16��ʼ��
*/
void DR_w25x16_init()
{
  //����SPI��MOSI��MISO��SPCKΪ���裬��CS0,CS1Ϊͨ��I/0��FLASH��CS��SPI�Ĳ���ͬ����
  AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, // PIO controller base address
                      ((unsigned int) AT91C_PA13_MOSI    ) |
                        ((unsigned int) AT91C_PA12_MISO    ) |
                          ((unsigned int) AT91C_PA14_SPCK    ), // Peripheral A
                          0); // Peripheral B IS NO USE
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, W25X16_CS );
  AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS);//
  
  //SPI�Ŀ�������
  DR_spi_init();
  
  //SPI ʹ��
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
}

/*
��������DR_w25x16_disable
��������
����ֵ����
������w25x16оƬ�رգ���д�����򿪵������Ϊ��ֹ�󴥷��������ݣ�����Ҫ��FLASH����д��ֹ
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
��������DR_w25x16_enable
��������
����ֵ����
������w25x16оƬʹ�ܣ���Ҫ��FLASH���в�����д����ʱ����Ҫ��ʹ��д���ڲ�����д���Ҫ��
дʹ�ܹر��Է�ֹ��д��
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
��������DR_w25x16_get_state
��������
����ֵ����
bit0 ΪW25X_IS_BUSY����FLASHæ��0Ϊ����
bit1 ΪW25X_IS_WRITE_ENABLE����дʹ�ܴ�
bit2-bit5  �ܱ���������
bit7 ΪW25X_IS_WP����WP�������ͣ�����FLASH�ܱ���
�������ж�FLASH�ǿ��л���æ
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
��������DR_w25x16_erase_all
��������
����ֵ����
������w25x16оƬȫ������
*/
void DR_w25x16_erase_all()
{
  DR_w25x16_enable();//дʹ�ܴ�
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
 
  WRITE_W25X_SPI(W25X_CHIP_ERASE);
 
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
  W25X_CHIP_ERASE_WAIT;
 
  DR_w25x16_disable();//дʹ�ܹر�
}

/*
��������DR_w25x16_erase_offset
������type Ϊ�����������ͣ�W25X_CHIP_ERASEΪ����FLASH������W25X_BLOCK_ERASE����ĳһ�顢W25X_SECTOR_ERASE����ĳһ����
Addre24 ��������ʼ��ַ,��Ļ�Ҫ���ַ��0X10000�ı�������������0X1000.(FLASH���Լ������룬ǰ��ı�������ȡ��)
����ֵ����
����������ǰ����Ҫдʹ�ܡ���д��Щ�ܱ����ĵ�ַʱ���������
����һ���Ȼ��Ҫ1�룬����FLASHҪ20�롣
*/
void DR_w25x16_erase_offset(unsigned char type,unsigned long Addre24)
{
  unsigned char addr23,addr15,addr7;
 
  DR_w25x16_enable();//дʹ�ܴ�
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  if(W25X_CHIP_ERASE==type)//����оƬ��������һ�������
  {
    WRITE_W25X_SPI(type);
  }
  else
  {
    WRITE_W25X_SPI(type);//���������Ҫ���͵�ַ
    WRITE_W25X_SPI(addr23);//���͵�ַҪ�ȷ��͸�λ
    WRITE_W25X_SPI(addr15);
    WRITE_W25X_SPI(addr7);
  }
  W25X_CS_STOP_HIGH_WAIT;
  TO_W25X_CS_HIGH ;
 
  ////��������еȴ����ر�дʹ��
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
��������DR_w25x16_read_status
������
����ֵ��
������
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
��������DR_w25x16_read
������Addre24 24λ�������ݵ���ʼ��ַ
length �������ݵĳ��ȣ���ȡ���ݿ��Դӷ��͹�ȥ����ʼ��ַ��FLASH�����һ���ֽ�
buff  ��ȡ�������ݴ����ʼ��ַ
����ֵ���Ƿ��Ѿ����ɹ�
������������ʱ��ʼ��ַ�������
*/
void DR_w25x16_read(unsigned long Addre24,unsigned char *buff,unsigned short length)
{
  unsigned char addr23,addr15,addr7;
 
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
 
  if((DR_w25x16_read_status()&W25X_IS_BUSY)==W25X_IS_BUSY)//FLASHæ
    return ;
 
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_READ_DATA);//���Ͷ�FLASH����
  WRITE_W25X_SPI(addr23);//���͵�ַҪ�ȷ��͸�λ
  WRITE_W25X_SPI(addr15);
  WRITE_W25X_SPI(addr7);
  //������ʱ������������͵�ַ��ҪCLK�������ӲŽ��е�ַ���������棬Ϊ����һ���Ͷ෢ һ���ֽڡ�
  //�������Զ෢һ��������ɴ���
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
��������DR_w25x16_write
������Addre24 д���ݵ���ʼ��ַ����Ҫд����ҳʱ���8λ����Ϊ0������������ֻ��ҳ���λ��0��ʼд
length Ҫд���ݵĳ��ȣ�����С�ڵ���256
buff Ҫд��FLASH��������ʼ��ַ
����ֵ���Ƿ�д�ɹ�
��������ĳһҳ����д���ݣ����ܿ�ҳд��Ҳ����дһ�����256�ֽڣ���ʼ��ַ���Բ���Ҳ����ʼ��ַ������ʼ��ַΪ
�����ַʱҪע�����ݳ���ҪС��ҳ����ַ�������ַ��
һ��ҳ�ɷּ���д������д������Ҫ������ǲ���û��Ⱦ���ġ�
*/
void DR_w25x16_write(unsigned long Addre24,unsigned char *buff,unsigned short lenght)
{
  unsigned char addr23,addr15,addr7;
  unsigned char curr_lenght;
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
  if((DR_w25x16_read_status()&W25X_IS_BUSY)==W25X_IS_BUSY)//FLASHæ
    return ;
 
  addr7=Addre24;
  addr15=Addre24>>8;
  addr23=Addre24>>16;
 
 
  //Ҫ֧�ֿ�ҳд
  if((addr7+lenght)>256)
  {
    curr_lenght = 256 - addr7;
    DR_w25x16_enable();//дʹ�ܴ�
    TO_W25X_CS_LOW ;
    W25X_CS_LOW_START_WAIT; 
    WRITE_W25X_SPI(W25X_PAGE_WRITE);//���Ͷ�FLASH����
    WRITE_W25X_SPI(addr23);//���͵�ַҪ�ȷ��͸�λ
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
    addr7 = 0;//��һҳ���¿�ʼ
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
 
  DR_w25x16_enable();//дʹ�ܴ�
  TO_W25X_CS_LOW ;
  W25X_CS_LOW_START_WAIT;
  WRITE_W25X_SPI(W25X_PAGE_WRITE);//���Ͷ�FLASH����
  WRITE_W25X_SPI(addr23);//���͵�ַҪ�ȷ��͸�λ
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

