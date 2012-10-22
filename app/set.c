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
*set文件主要用来关联各种设置相关，例如button,twc,led,buzz,sms,flash.
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件
#include "led.h"
#include "phe.h"


//变量引用
//变量定义
static unsigned char btn_set_state = 0;

//函数引用
//函数定义
/*
函数名：APP_sms_set
参数：无
返回值：无
描述：
*/
void APP_sms_set(unsigned char sms_event)
{
    if (sms_event == 1)  //设置手机号码
    {
        //比较密码是否匹配
          //DR_flash_write();
        //亮灯，喇叭，及写入EFS,回发短信
          //TS_phe_sms_ind();
    }
    else if (sms_event == 6) //设置密码
    {
        //写入EFS，回发短信
          //DR_flash_write();
    }
}

/*
函数名：APP_btn_set
参数：无
返回值：无
描述：
*/
void APP_btn_set(unsigned int btn_event)
{
    if (btn_event == (1<<29)) //复位键按下
    {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
        //清楚EFS数据
          //DR_flash_erase_all();
        //设置默认密码等
          //DR_flash_write();
    }
    if (btn_event == (1<<30)) //遥控器设置
    {
        //进行灯的操作并设置状态 
        if (btn_set_state == 0)   //设置遥控器
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_OFF);
          btn_set_state++;
        } 
        else if (btn_set_state == 1) //设置设备
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_OFF);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
          btn_set_state++;
        }
        else   //设置号码
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_BLINK);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_BLINK);
          btn_set_state = 0;
        }
    }
}

/*
函数名：APP_twc_set
参数：无
返回值：无
描述：
*/
void APP_twc_set(unsigned int *addr)
{
  //查看设置状态，设备还是遥控器
  if (MC_check_addr_in_c(*addr)) //判断是否在数组里面
  {
    return ;  //在那就返回
  }
  else
  {
     if (MC_write_addr_in_c(*addr)) //不在就写入，写入成功鸣喇叭
     {
       TS_phe_buzz_ind();
     }
     return ;
  }
}


/*
函数名：APP_twc_alarm
参数：无
返回值：无
描述：
*/
void APP_twc_alarm(unsigned int *addr)
{
  //查看设置状态，设备还是遥控器
  if (MC_check_addr_in_d(*addr)) //判断是否在数组里面
  {
    return ;  //在那就返回
  }
  else
  {
     if (MC_write_addr_in_d(*addr)) //不在就写入，写入成功鸣喇叭
     {
       TS_phe_buzz_ind();
     }
     return ;
  }
}



