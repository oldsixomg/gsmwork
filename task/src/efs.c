/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件
#include "os.h"
#include "efs.h"

//使用NV和文件

//变量引用
//变量定义
OS_FLAGS     ts_efs_event_id;
OS_FLAG_GRP  *ts_efs_cmd_event;


unsigned char efs_nv_swap_buff[256];
//函数引用
//函数定义

void TS_efs_nv_write(unsigned int item,unsigned int data)
{/**/
  DR_flash_read(NV_C_COUNT_I,efs_nv_swap_buff,220);
  efs_nv_swap_buff[item*4] = data;
  efs_nv_swap_buff[item*4+1] = data>>8;
  efs_nv_swap_buff[item*4+2] = data>>16;
  efs_nv_swap_buff[item*4+3] = data>>24;
  DR_flash_erase_byte(NV_C_COUNT_I);
  //DR_flash_write(NV_C_COUNT_I,efs_nv_swap_buff,220);
  DR_flash_write(NV_C_COUNT_I+item,&data,4);
  
  //DR_flash_write(item*4,&data,4);
  
}
unsigned int TS_efs_nv_read(unsigned int item)
{
  unsigned int temp;
  DR_flash_read(item*4,&temp,4);
  return temp;
}

void TS_efs_file_write(char * file,char *src,unsigned int len)
{
  
}

void TS_efs_init(void)
{
    //DR_flash_init();
    
    //APP_mc();
}

/*
函数名：TS_efs_task
参数：无
返回值：无
描述：FLASH初始化
*/
void TS_efs_task(void *p_arg)
{
  unsigned char       err;  
  OS_CMD *cmd_ptr;
  TS_efs_init();
  ts_efs_cmd_event = OSFlagCreate ( 0 , &err);
  
  while(1)
  { 
    ts_efs_event_id =OSFlagPend(ts_efs_cmd_event,( TS_Q_EFS_CMD ), (OS_FLAG_WAIT_SET_ANY |OS_FLAG_CONSUME ),0 , &err);
    if (TS_Q_EFS_CMD & ts_efs_event_id)
    {
        //从命令队列里面循环取消息处理
        while ((cmd_ptr = q_get(&efs_cmd_alloc)) != NULL)
        {

        }
    }
  }
}





