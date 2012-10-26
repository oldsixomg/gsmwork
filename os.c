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
#include "bsp.h"
#include "phe.h"
#include "sio.h"


//变量引用
//变量定义
OS_CMD os_cmd[32];
OS_CMD_LIST cmd_free_list;
OS_CMD_LIST phe_cmd_alloc;
OS_CMD_LIST sio_cmd_alloc;
OS_CMD_LIST efs_cmd_alloc;

static DSM_ITEM dsm_item_table[DSM_ITEM_MAX_CNT];
static DSM_ITEM *dsm_free_list ;
//函数引用
//函数定义

/*
函数名：os_init
参数：无
返回值：无
描述：初始化
*/
void os_init(void)
{
  /////////////////////////////////////////////
  dsm_free_list = &dsm_item_table[0];
  for (int j =0;j<DSM_ITEM_MAX_CNT-1;j++)
  {
    dsm_item_table[j].next = &dsm_item_table[j+1];
  }
  dsm_item_table[DSM_ITEM_MAX_CNT-1].next = 0;
  ////////////////////////////////////////
  cmd_free_list.head = &os_cmd[0];
  for (int i=0;i<31;i++)
  {
    os_cmd[i].next = &os_cmd[i+1];
  }
  os_cmd[31].next = 0;
  cmd_free_list.last = &os_cmd[31];
  /////////////////////////////////////
  phe_cmd_alloc.head = 0;
  phe_cmd_alloc.last = 0;
  
}

DSM_ITEM * dsm_alloc_item(void)
{
  DSM_ITEM * dsm;
  if (dsm_free_list != 0)
  {
    dsm = dsm_free_list;
    dsm_free_list = dsm->next;
    dsm->next = 0;
  }
  else
  {
    dsm = (DSM_ITEM *)0;
  }
  return dsm;
}

void dsm_free_item(DSM_ITEM *dsm)
{
  DSM_ITEM *list = dsm;
  while (list->next != 0) //走到连的最后一个
  {
    list->used = 0;
    list = list->next;
  }
  list->used = 0;
  list->next = dsm_free_list;  //最后一个节点接上原来free的连，并插在前面
  dsm_free_list = dsm;  //重新对free指针赋值
}


/*
函数名：put_cmd
参数：无
返回值：无
描述：分配一个命令
*/
OS_CMD * alloc_a_cmd(void)
{
  OS_CMD * cmd_ptr ;
  if (cmd_free_list.head != cmd_free_list.last)
  {
    cmd_ptr = cmd_free_list.head;
    cmd_free_list.head = cmd_ptr->next;
    cmd_ptr->next = 0;//断掉连
    return cmd_ptr;
  }
  else 
  {
    return 0;
  }
  
}

/*
函数名：free_a_cmd
参数：无
返回值：无
描述：命令用完需要释放掉
*/
void free_a_cmd(OS_CMD *cmd_ptr)
{
  cmd_free_list.last->next = cmd_ptr;
  cmd_free_list.last = cmd_ptr;
}

/*
函数名：q_put
参数：无
返回值：无
描述：把命令加入队列
*/
void q_put(OS_CMD *cmd_ptr,OS_CMD_LIST *alloc_list)
{
  if (alloc_list->head == 0)
  {
    alloc_list->head = cmd_ptr;
    alloc_list->last = cmd_ptr;
  }
  else
  {
    alloc_list->last->next = cmd_ptr;
    alloc_list->last = cmd_ptr;
  }
}

/*
函数名：q_get
参数：无
返回值：无
描述：从队列中取数据
*/
OS_CMD * q_get(OS_CMD_LIST *alloc_list)
{
  OS_CMD * cmd_ptr; 
  
  cmd_ptr = alloc_list->head;
  if (cmd_ptr != 0)
  {
    alloc_list->head = cmd_ptr->next;
    cmd_ptr->next = 0;
    return cmd_ptr;
  }
  return 0;
}

/*
函数名：put_cmd
参数：无
返回值：无
描述：把命令加入队列
*/
void phe_put_cmd(unsigned int cmd,void *ptr)
{
  unsigned char       err;  
  OS_CMD * cmd_ptr;
  
  cmd_ptr = alloc_a_cmd();
  cmd_ptr->cmd_id = cmd;
  cmd_ptr->data_p = ptr;
  q_put(cmd_ptr,&phe_cmd_alloc);
  OSFlagPost (ts_phe_cmd_event, TS_Q_PHE_CMD,OS_FLAG_SET ,&err);
}
/*
函数名：sio_put_cmd
参数：无
返回值：无
描述：把命令加入队列
*/
void sio_put_cmd(unsigned int cmd,void *ptr)
{
  unsigned char       err;  
  OS_CMD * cmd_ptr;
  
  cmd_ptr = alloc_a_cmd();
  cmd_ptr->cmd_id = cmd;
  cmd_ptr->data_p = ptr;
  q_put(cmd_ptr,&sio_cmd_alloc);
  OSFlagPost (ts_sio_cmd_event, TS_Q_SIO_CMD,OS_FLAG_SET ,&err);
}

unsigned char str_include(char *str1,char *str2)
{
  while (*str2 != 0)
  {
    if (*str2 != *str1)
    {
      return 0;
    }
    str2++;
    str1++;
  }
  return 1;
}

int str_to_int(char *str1)
{
  char   *p   =   str1;
  char   c;
  int   i   =   0;
  while(c=*p++)
  {
    if((c>='0') && (c<='9'))
    {
      i= i*10 + (c-'0');
    }
    else
      return -1;
  }
  return   i;
  
}
