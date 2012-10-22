/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*
*
*APP主要是接受消息再响应，主要是一些被动执行的函数，跟任务打交道的函数，不能直接调用驱动底层的执行函数。
*
*主控文件，各个应用的统一状态和转换由此进行
*
* ----------------------------------------------------------------------------
*/


//宏定义
typedef enum {
  MC_SYS_SET_C = 0,
  MC_SYS_SET_D,
  MC_SYS_SET_NUM,
  MC_SYS_RESET,
  MC_SYS_NO_SET,
  MC_SYS_UNPORTECT,
  MC_SYS_PORTECT,
  MC_SYS_ALARM,
}MC_SYS_STATE;

//头文件
#include "efs.h"


//变量引用
//变量定义
MC_SYS_STATE mc_sys_state = MC_SYS_NO_SET;
unsigned long mc_c_addr_cnt = 0;
unsigned long mc_c_addr_arry[6];

unsigned long mc_d_addr_cnt = 0;
unsigned long mc_d_addr_arry[6];

char mc_sys_num[20] = {0};
//函数引用
//函数定义
/*
函数名：APP_mc
参数：无
返回值：无
描述：FLASH初始化
*/
void APP_mc(void)
{
  
  //从flash读取内容到mc_c_addr_arry,mc_c_addr_cnt
  mc_c_addr_cnt = TS_efs_nv_read(NV_C_COUNT_I);
  if (mc_c_addr_cnt <6)
  {
  for (int i = 0;i < mc_c_addr_cnt;i++)
  {
    mc_c_addr_arry[i] = TS_efs_nv_read(NV_C_ADD_S_I+i);
  }
  }
  else
  {
    mc_c_addr_cnt = 0;
  }
  //从flash读取内容到mc_d_addr_arry,mc_d_addr_cnt
  mc_d_addr_cnt = TS_efs_nv_read(NV_D_COUNT_I);
  if (mc_d_addr_cnt <20)
  {
  for (int i = 0;i < mc_d_addr_cnt;i++)
  {
    mc_d_addr_arry[i] = TS_efs_nv_read(NV_D_ADD_S_I+i);
  }
  }
  else
  {
    mc_d_addr_cnt = 0;
  }
  
}

MC_SYS_STATE mc_get_state(void)
{
  return mc_sys_state;
}

unsigned char mc_set_state(MC_SYS_STATE new_state)
{
  unsigned char result = 0;
  switch (new_state)
  {
  case MC_SYS_SET_C:
    if ((mc_sys_state == MC_SYS_PORTECT)||(mc_sys_state == MC_SYS_UNPORTECT)||(mc_sys_state == MC_SYS_NO_SET))
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_SET_D:
    if (mc_sys_state == MC_SYS_SET_C)
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_SET_NUM:
    if (mc_sys_state == MC_SYS_SET_D)
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_RESET:
      mc_sys_state = new_state;
      result =1;
    break;
  case MC_SYS_NO_SET:
    if (mc_sys_state == MC_SYS_RESET)
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_UNPORTECT:
    if ((mc_sys_state == MC_SYS_PORTECT)||(mc_sys_state == MC_SYS_SET_NUM))
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_PORTECT:
    if (mc_sys_state == MC_SYS_UNPORTECT)
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  case MC_SYS_ALARM:
    if (mc_sys_state == MC_SYS_PORTECT)
    {
      mc_sys_state = new_state;
      result =1;
    }
    break;
  }
    return result;
}



unsigned int MC_check_addr_in_c(int addr)
{
  for (int i = 0;i<mc_c_addr_cnt;i++)
  {
    if ((mc_c_addr_arry[i]& 0xfffff) == (addr & 0xfffff))
    {
      return 1;
    }
  }
  return 0;
}

unsigned int MC_write_addr_in_c(int addr)
{
  if (mc_c_addr_cnt > 5)
    return 0;
  addr = addr & 0xfffff;
  
  TS_efs_nv_write(NV_C_ADD_S_I + mc_c_addr_cnt,addr);
  
  mc_c_addr_arry[mc_c_addr_cnt] = addr;
  mc_c_addr_cnt++;
  TS_efs_nv_write(NV_C_COUNT_I,mc_c_addr_cnt);
  return 1;
}


unsigned int MC_check_addr_in_d(int addr)
{
  for (int i = 0;i<mc_d_addr_cnt;i++)
  {
    if ((mc_d_addr_arry[i]& 0xfffff) == (addr & 0xfffff))
    {
      return 1;
    }
  }
  return 0;
}

unsigned int MC_write_addr_in_d(int addr)
{
  if (mc_d_addr_cnt > 20)
    return 0;
  addr = addr & 0xfffff;
  
  TS_efs_nv_write(NV_D_ADD_S_I + mc_d_addr_cnt,addr);
  
  mc_d_addr_arry[mc_d_addr_cnt] = addr;
  mc_d_addr_cnt++;
  TS_efs_nv_write(NV_D_COUNT_I,mc_d_addr_cnt);
  return 1;
}

void MC_write_addr_in_n(void)
{
  int i=0;
  while (mc_sys_num[i] != 0)
  {
    TS_efs_nv_write(NV_N_ADD_S_I,mc_sys_num[i]);
    i++;
  }
  
}

void mc_set_sys_num(char * num_p, int len)
{
  int i;
   if ((mc_get_state() != MC_SYS_SET_NUM) || (0 == strlen(mc_sys_num)))//不在设置状态，或号码已经存在则返回。
     return ;
   
   for (i =0;i<len ;i++)
   {
     mc_sys_num[i] = num_p[i];
   }
   mc_sys_num[i] = 0;
   //写入到NV里面
   MC_write_addr_in_n();
}

