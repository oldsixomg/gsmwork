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
*指示灯驱动文件
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义




//头文件
#include "led.h"
#include "drive.h"


//变量引用
//变量定义
TIME_REG_STRUCT dr_yled_time;
TIME_REG_STRUCT dr_rled_time;
unsigned int dr_led_red_state;
unsigned int dr_led_yellow_state;

//函数引用
//函数定义
/*
函数名：DR_led_init
参数：无
返回值：无
描述：指示灯初始化
*/
void DR_led_init(void)
{
  GLOBE_SET_GPIO_OUT(GLOBE_LED_RED);
  GLOBE_SET_GPIO_OUT(GLOBE_LED_YELLOW);
  
  GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
  GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
  
  dr_yled_time.cb = DR_yled_cb;
  dr_rled_time.cb = DR_rled_cb;
}
/*
函数名：DR_led_set
参数：无
返回值：无
描述：设置指示灯工作状态
*/
void DR_led_set(unsigned int color,unsigned int state)
{
   if (GLOBE_LED_RED == color)
   {
     dr_led_red_state = state;
     switch (state)
     {
     case GLOBE_LED_ON:
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_RED);
       break;
     case GLOBE_LED_OFF:
       GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
       break;
     case GLOBE_LED_BLINK:
       dr_rled_time.timeout = 100;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_rled_time);
       break;
     case GLOBE_LED_QBLINK:
       dr_rled_time.timeout = 20;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_rled_time);
       break;
     }
   }
   
   if (GLOBE_LED_YELLOW == color)
   {
     dr_led_yellow_state = state;
     switch (state)
     {
     case GLOBE_LED_ON:
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       break;
     case GLOBE_LED_OFF:
       GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
       break;
     case GLOBE_LED_BLINK: 
       dr_yled_time.timeout = 100;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_yled_time);
       break;
     case GLOBE_LED_QBLINK:
       dr_yled_time.timeout = 20;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_yled_time);
       break;
     }
   }
  
}
/*
函数名：DR_led_check
参数：无
返回值：无
描述：查询当前指示灯状态
*/
unsigned char DR_led_check(unsigned int color)
{
  if (GLOBE_GET_LED()&color)
    return 1;
  else
    return 0;
}

/*
函数名：DR_yled_cb
参数：无
返回值：无
描述：设置指示灯工作状态
*/
void DR_yled_cb(void)
{  
  if (dr_led_yellow_state == GLOBE_LED_BLINK)
  {
      dr_yled_time.timeout = 100;
  }
  else if (dr_led_yellow_state == GLOBE_LED_QBLINK)
  {
      dr_yled_time.timeout = 20;
  }
  else
  {
    return ;
  }  
  
  if (DR_led_check(GLOBE_LED_YELLOW) == 0)
  {
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
  }
  else
  {
       GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
  }
  
  time_client_reg(&dr_yled_time);
}

/*
函数名：DR_rled_cb
参数：无
返回值：无
描述：设置指示灯工作状态
*/
void DR_rled_cb(void)
{
  if (dr_led_red_state == GLOBE_LED_BLINK)
  {
      dr_rled_time.timeout = 100;
  }
  else if (dr_led_red_state == GLOBE_LED_QBLINK)
  {
      dr_rled_time.timeout = 20;
  }
  else
  {
    return ;
  }
  
  if (DR_led_check(GLOBE_LED_RED) == 0)
  {
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_RED);
  }
  else
  {
       GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
  }
  time_client_reg(&dr_rled_time);
}


