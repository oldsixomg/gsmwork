/* ----------------------------------------------------------------------------
* WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为驱动文件，执行硬件初始化，中断处理，硬件操作，状态机。往上提供的接口不变。
*同时文件放置以类型相同的放在一起，各个驱动文件之间可以直接调用。
*
*按键驱动文件
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义

//头文件

#include "type.h"
#include "drive.h"
#include "button.h"
#include "os.h"

//变量引用
//变量定义
unsigned short dr_rst_low_cnt=0;
unsigned short dr_set_low_cnt=0;
TIME_REG_STRUCT dr_rst_time;
TIME_REG_STRUCT dr_set_time;
//函数引用
//函数定义
/*
函数名：DR_key_init
参数：无
返回值：无
描述：按键初始化
*/
void DR_key_init()
{
  GLOBE_SET_GPIO_IN(GLOBE_RST_KEY);
  GLOBE_SET_GPIO_IN(GLOBE_SET_KEY);
  
  GLOBE_GPIO_INT_EN(GLOBE_RST_KEY);
  GLOBE_GPIO_INT_EN(GLOBE_SET_KEY);
  
  dr_rst_time.cb = DR_rst_cb;
  dr_set_time.cb = DR_set_cb;
}

/*
函数名：DR_key_report
参数：无
返回值：无
描述：按键识别后往上层报
*/
void DR_key_report(uint32 num)
{
  /*发送按键事件并上报给对应任务*/
  OS_REPORT_KEY_EVENT(num);
  
}

/*
函数名：DR_rst_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_rst_cb(void)
{
    if (DR_key_input_check(GLOBE_RST_KEY))
    {
      return;
    }
    
    if (dr_rst_low_cnt < 10)
    {
      dr_rst_low_cnt++;
      dr_rst_time.timeout = 20;
      time_client_reg(&dr_rst_time);
    }
    else
    {
      DR_key_report(GLOBE_RST_KEY);
    }
}



/*
函数名：DR_set_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_set_cb(void)
{
    if (DR_key_input_check(GLOBE_SET_KEY))
    {
      return;
    }
    if (dr_set_low_cnt < 2)
    {
      dr_set_low_cnt++;
      dr_set_time.timeout = 20;
      time_client_reg(&dr_set_time);
    }
    else
    {
      DR_key_report(GLOBE_SET_KEY);
    }
}



/*
函数名：DR_key_isr_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_key_isr_cb()
{
  uint32 key_state;
  key_state = ~GLOBE_GET_KEY();  //接地低电平有效，故与的话先取反
  if (GLOBE_RST_KEY&key_state)
  {
    //注册定时器
    dr_rst_low_cnt = 0;
    dr_rst_time.timeout = 20;
    time_client_reg(&dr_rst_time);
    
  }
  else if (GLOBE_SET_KEY &key_state)
  {
    dr_set_low_cnt = 0;
    dr_set_time.timeout = 20;
    time_client_reg(&dr_set_time);
  }
}





/*
函数名：DR_key_input_check
参数：无
返回值：无
描述：查询按下的按键高低电平
*/
unsigned int DR_key_input_check(unsigned int num)
{
  if (GLOBE_GET_KEY()&num)
    return 1;
  else
    return 0;
}
