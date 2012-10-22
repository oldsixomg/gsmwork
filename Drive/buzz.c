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
*蜂鸣器驱动文件
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义
#define GLOBE_MIN_BUZZ        (1<<23)
#define GLOBE_MAX_BUZZ       (1<<23)
#define GLOBE_BUZZ_MUTE 0
#define GLOBE_BUZZ_RING 1
#define GLOBE_SET_GPIO_OUT(x)    AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_HIGH(x)   AT91F_PIO_SetOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_LOW(x)   AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,x)
#define GLOBE_CHECK_GPIO()        AT91F_PIO_GetOutputDataStatus(AT91C_BASE_PIOA)

//头文件
#include "type.h"
#include "drive.h"
#include "button.h"
#include "os.h"


//变量引用
//变量定义


//函数引用
//函数定义
/*
函数名：DR_buzz_init
参数：无
返回值：无
描述：蜂鸣器初始化
*/
void DR_buzz_init()
{
   GLOBE_SET_GPIO_OUT(GLOBE_MIN_BUZZ);
   GLOBE_SET_GPIO_OUT(GLOBE_MAX_BUZZ);

   GLOBE_SET_GPIO_LOW(GLOBE_MIN_BUZZ);
   GLOBE_SET_GPIO_LOW(GLOBE_MAX_BUZZ);
}

/*
函数名：DR_buzz_set
参数：无
返回值：无
描述：设置蜂鸣器工作状态
*/
void DR_buzz_set(unsigned int num,unsigned int state)
{
    if (GLOBE_BUZZ_RING ==state)   //喇叭响
    {
       GLOBE_SET_GPIO_HIGH(num);
    }
    else                       //其他情况喇叭熄灭
    {
       GLOBE_SET_GPIO_LOW(num);
     }
}


/*
函数名：DR_buzz_check
参数：无
返回值：无
描述：查询当前蜂鸣器状态
*/
unsigned int DR_buzz_check(unsigned int num)
{
   if (num &GLOBE_CHECK_GPIO())
         return 1;
   else
         return 0;
}
