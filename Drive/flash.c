/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*���ļ�ΪFLASH�����ļ�����Ҫʵ�ֳ�ʼ��������������д���ܡ�
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�



//��������
//��������


//��������
#define GLOBE_FLASH_INIT() DR_w25x16_init()
#define GLOBE_FLASH_WRITE(x,y,z) DR_w25x16_write(x,y,z);
#define GLOBE_FLASH_READ(x,y,z)  DR_w25x16_read(x,y,z);
#define GLOBE_FLASH_ERASE_OFFSET(y) DR_w25x16_erase_offset(0XD8,y)
#define GLOBE_FLASH_ERASE() DR_w25x16_erase_all()
//��������
/*
��������DR_flash_init
��������
����ֵ����
������FLASH��ʼ��
*/
void DR_flash_init()
{
   GLOBE_FLASH_INIT();
}

/*
��������DR_flash_erase_all
��������
����ֵ����
������FLASH����������
*/
void DR_flash_erase_all()
{
   GLOBE_FLASH_ERASE();
}

/*
��������DR_flash_erase_byte
��������
����ֵ����
������FLASH����ָ���ֽ�
*/
void DR_flash_erase_byte(unsigned int addr)
{
   GLOBE_FLASH_ERASE_OFFSET(addr);
}
/*
��������DR_flash_write
��������
����ֵ����
������FLASHд��ָ�����ȵ����ݵ�ָ��λ��
*/
void DR_flash_write(unsigned int addr,unsigned int *ptr,unsigned int len)
{
   GLOBE_FLASH_WRITE(addr,ptr,len);
}

/*
��������DR_flash_read
��������
����ֵ����
������FLASH��ָ��λ�ö�ȡָ�����ȵ�����
*/
void DR_flash_read(unsigned int addr,unsigned int *ptr,unsigned int len)
{
   GLOBE_FLASH_READ(addr,ptr,len);
}

