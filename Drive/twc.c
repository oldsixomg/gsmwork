/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为驱动文件，执行硬件初始化，中断处理，硬件操作，状态机。往上提供的接口不变。
*同时文件放置以类型相同的放在一起，各个驱动文件之间可以直接调用。
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义
typedef enum
{
  DR_TWC_NONE = 0,
  DR_TWC_INIT,
  DR_TWC_SYNC1,
  DR_TWC_SYNC2,
  DR_TWC_CONTENT,
}DR_TWC_STATE;




//头文件
#include "drive.h"
#include "twc.h"
#include "os.h"
#include "led.h"


//变量引用
//变量定义
static DR_TWC_STATE dr_twc_state ;
static unsigned long dr_twc_valid_addr;
static unsigned int dr_twc_sync_count;
static unsigned int dr_twc_sync_count1;
static unsigned int dr_twc_sync_ms_len;
static unsigned int dr_twc_gap_count;

//函数引用
//函数定义
/*
函数名：DR_twc_1ms_cb
参数：无 
返回值：无
描述：没一秒执行一次，用来检查是否有低电平开始
*/
void DR_twc_1ms_cb(void)   //TIME1
{ 
  if (0 == (GLOBE_GET_INT_STATE() & GLOBE_TWC_RX))
  {
    //没有跳变
    if (0 == (GLOBE_GET_KEY() & GLOBE_TWC_RX))
    {//低电平
      if (dr_twc_state > DR_TWC_SYNC1)
      {
        return ;
      }
      if (dr_twc_sync_count== 7)//最小GUARD时间6MS
      {//满足4MS的持续低电平，代表探测到同步头
        DR_twc_start_recv();
      }
      dr_twc_sync_count++;//启动接收后还需要记时
    }
    else
    {//高电平
      dr_twc_sync_count= 0;
    }
  }
  else
  {
    //有跳变就清0
    dr_twc_sync_count= 0;
  }
 
}
/*
函数名：DR_twc_init
参数：无
返回值：无
描述：超再生接收初始化
*/
void DR_twc_init()
{
  GLOBE_SET_GPIO_IN(GLOBE_TWC_RX); 
  GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
  dr_twc_state = DR_TWC_INIT;
  dr_twc_sync_count= 0;
  
}

/*
函数名：DR_twc_start_recv
参数：无
返回值：无
描述：
*/
void DR_twc_start_recv()
{
  //打开中断
  GLOBE_GPIO_INT_EN(GLOBE_TWC_RX);
  dr_twc_state = DR_TWC_SYNC1;//满足同步条件，下面就是识别同步长度
}
/*
函数名：DR_twc_get_result
参数：无
返回值：无
描述：
*/
void DR_twc_get_result()
{
  //关闭中断
  DR_twc_result_report(&dr_twc_valid_addr);
  DR_twc_exit();
}
/*
函数名：DR_twc_exit
参数：无
返回值：无
描述：
*/
void DR_twc_exit()
{
  dr_twc_state = DR_TWC_INIT;
  //关闭中断,靠1ms定时器来启动
  GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
  dr_twc_sync_count = 0;
}
/*
函数名：DR_twc_count_start
参数：无
返回值：无
描述：
*/
void DR_twc_count_start()
{
  timer1_start_count();
}

/*
函数名：DR_twc_get_count
参数：无
返回值：无
描述：
*/
unsigned int DR_twc_get_count()
{
  return  timer1_get_count();
}
/*
函数名：DR_twc_isr_cb
参数：无
返回值：无
描述：超再生接收中断发生时调用的主入口
*/
void DR_twc_isr_cb()    //最开始中断来了，启动识别，定时器0.2ms检测一下低电平（IO中断关闭），直到同步满足才打开中断停止计数。
{
  switch (dr_twc_state)
  {
  case DR_TWC_INIT:   //空闲初始化状态收到中断
    break;
    
  case DR_TWC_SYNC1:        //~~|____|
    if (dr_twc_sync_count> 13)
    {
      DR_twc_exit();
    }
    else
    {
      dr_twc_sync_ms_len =dr_twc_sync_count;
      DR_twc_count_start();
      dr_twc_state = DR_TWC_SYNC2;
    }
    break;
    
  case DR_TWC_SYNC2:   //~~|____|~~|   ==第一个GAP下降
    dr_twc_sync_count1= DR_twc_get_count();
    if ((dr_twc_sync_count1> 10*dr_twc_sync_ms_len)&&(dr_twc_sync_count1< 40*dr_twc_sync_ms_len ))
    {
      dr_twc_valid_addr = 0;
      dr_twc_state =DR_TWC_CONTENT;
    }
    else
    {
      DR_twc_exit();
    }
    break;
    
  case DR_TWC_CONTENT:  //~~|____|~~|___|   ==第一个GAP上升
    dr_twc_gap_count = DR_twc_get_count();
    if ((dr_twc_gap_count < dr_twc_sync_ms_len*36)||(dr_twc_gap_count > 52*dr_twc_sync_ms_len))
    {
      DR_twc_exit();
      break;
    }
    GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
    for (int i =1;i<24;i++)
    {
      DR_twc_count_start();
      while (0 == (GLOBE_GET_INT_STATE()& GLOBE_TWC_RX));
      dr_twc_gap_count  = DR_twc_get_count();
      if (dr_twc_gap_count > 300)
      {
        dr_twc_valid_addr= (1 << i)|dr_twc_valid_addr;
      }
      else if (dr_twc_gap_count < 100)
      {
        DR_twc_exit();
        return;
      }
      while (0 == (GLOBE_GET_INT_STATE() & GLOBE_TWC_RX));
      dr_twc_gap_count  = DR_twc_get_count();
      if (dr_twc_gap_count > 500 )
      {
        DR_twc_exit();
        return;
      }
    }
    //得到数据了
    DR_twc_get_result();
    break;
  }
}
/*
函数名：DR_twc_result_report
参数：无
返回值：无
描述：超再生接收完成后上报数据给上层
*/
void DR_twc_result_report(unsigned int * num)
{
  OS_REPORT_TWC_MSG(GLOBE_TWC_RX,num);
}


