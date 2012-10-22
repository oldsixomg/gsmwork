/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�
#include "os.h"
#include "efs.h"

//ʹ��NV���ļ�

//��������
//��������
OS_FLAGS     ts_efs_event_id;
OS_FLAG_GRP  *ts_efs_cmd_event;


unsigned char efs_nv_swap_buff[256];
//��������
//��������

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
��������TS_efs_task
��������
����ֵ����
������FLASH��ʼ��
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
        //�������������ѭ��ȡ��Ϣ����
        while ((cmd_ptr = q_get(&efs_cmd_alloc)) != NULL)
        {

        }
    }
  }
}





