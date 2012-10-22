/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为FLASH驱动文件，主要实现初始化，擦除，读，写功能。
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件



//变量引用
//变量定义


//函数引用
#define GLOBE_FLASH_INIT() DR_w25x16_init()
#define GLOBE_FLASH_WRITE(x,y,z) DR_w25x16_write(x,y,z);
#define GLOBE_FLASH_READ(x,y,z)  DR_w25x16_read(x,y,z);
#define GLOBE_FLASH_ERASE_OFFSET(y) DR_w25x16_erase_offset(0XD8,y)
#define GLOBE_FLASH_ERASE() DR_w25x16_erase_all()
//函数定义
/*
函数名：DR_flash_init
参数：无
返回值：无
描述：FLASH初始化
*/
void DR_flash_init()
{
   GLOBE_FLASH_INIT();
}

/*
函数名：DR_flash_erase_all
参数：无
返回值：无
描述：FLASH整个被擦除
*/
void DR_flash_erase_all()
{
   GLOBE_FLASH_ERASE();
}

/*
函数名：DR_flash_erase_byte
参数：无
返回值：无
描述：FLASH擦除指定字节
*/
void DR_flash_erase_byte(unsigned int addr)
{
   GLOBE_FLASH_ERASE_OFFSET(addr);
}
/*
函数名：DR_flash_write
参数：无
返回值：无
描述：FLASH写入指定长度的数据到指定位置
*/
void DR_flash_write(unsigned int addr,unsigned int *ptr,unsigned int len)
{
   GLOBE_FLASH_WRITE(addr,ptr,len);
}

/*
函数名：DR_flash_read
参数：无
返回值：无
描述：FLASH从指定位置读取指定长度的数据
*/
void DR_flash_read(unsigned int addr,unsigned int *ptr,unsigned int len)
{
   GLOBE_FLASH_READ(addr,ptr,len);
}

