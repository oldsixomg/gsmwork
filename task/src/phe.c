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
#include "button.h"
#include "phe.h"
#include "os.h"
#include "twc.h"



//变量引用
//变量定义
OS_FLAGS     ts_phe_event_id;
OS_FLAG_GRP  *ts_phe_cmd_event;

//函数引用
//函数定义

/*
函数名：TS_phe_task
参数：无
返回值：无
描述：FLASH初始化
*/
void TS_phe_init(void)
{
  DR_key_init();
  DR_led_init();
  DR_twc_init();
  
  


/**/
}
/*
函数名：TS_phe_task
参数：无
返回值：无
描述：FLASH初始化
*/
void TS_phe_task(void *p_arg)
{
  
  unsigned char       err;  
  OS_CMD *cmd_ptr;
  
  TS_phe_init();
  ts_phe_cmd_event = OSFlagCreate ( 0 , &err);
  
  while(1)
  { 
    ts_phe_event_id =OSFlagPend(ts_phe_cmd_event,( TS_Q_PHE_CMD ),(OS_FLAG_WAIT_SET_ANY |OS_FLAG_CONSUME ), 0 , &err);
    if (TS_Q_PHE_CMD & ts_phe_event_id)
    {
      //从命令队列里面循环取消息处理
      while ((cmd_ptr = q_get(&phe_cmd_alloc)) != NULL)
      {
        if (cmd_ptr->cmd_id == GLOBE_RST_KEY)
        {
          APP_btn_set(GLOBE_RST_KEY);      
        }
        
        if (cmd_ptr->cmd_id == GLOBE_SET_KEY)
        {
          APP_btn_set(GLOBE_SET_KEY);    
        }
        if (cmd_ptr->cmd_id == GLOBE_TWC_RX)
        {
#if 1
          char contet_s[15] = "+CMTI:\"ME\",1";
    DSM_ITEM * item = dsm_alloc_item();
    for (int i=0;i<13;i++)
    {
      item->data_ptr[i] = contet_s[i];
      item->used++;
    }
          sio_put_cmd(1,item);
    break;
   
#endif
          //从MC获取当前系统状态                        
          if (mc_get_state()==1)
          {
            //调用SET里面的函数进行处理
            APP_twc_set((unsigned int*)cmd_ptr->data_p);
          }                 
           else if (mc_get_state()==2)
          {
          //调用ALARM里面的函数进行处理
          APP_twc_alarm((unsigned int*)cmd_ptr->data_p);
        }                 
           else if (mc_get_state()==3)
          {
          //调用MONITOR里面的函数进行处理
          APP_twc_monitor((unsigned int*)cmd_ptr->data_p);
        }
          
        }
        
        
        
      }
    }
  }
}

/*
函数名：TS_phe_led_ind
参数：无
返回值：无
描述：FLASH初始化
*/
void TS_phe_led_ind(unsigned int color,unsigned int state)
{
  DR_led_set(color,state);
}

/*
函数名：TS_phe_buzz_ind
参数：无
返回值：无
描述：FLASH初始化
*/
void TS_phe_buzz_ind(unsigned int device,unsigned int state)
{
  DR_buzz_set(device,state);
  
}




