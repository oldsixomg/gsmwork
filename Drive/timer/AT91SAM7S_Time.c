/* ----------------------------------------------------------------------------
*         vteng 公司提供支持 
* ----------------------------------------------------------------------------
* 版权：2010 VTENG公司
*
* 公司保留所有权力
*
*此文件为程序总入口，包括调用各种初始化函数，任务启动函数等
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//宏定义
//头文件
#include "drive.h"

//变量引用
//变量定义
//函数引用
//函数定义
/*
函数名：wait_s
参数：等待的单位数
返回值：无
描述：通过CPU循环等待，时间单位1秒
*/
void wait_s ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  //for(waiting_time = 0; waiting_time < SPEED *50*9*i; waiting_time++) ;
}//* End


/*
函数名：wait_ms
参数：等待的单位数
返回值：无
描述：通过CPU循环等待，时间单位1豪秒
*/
void wait_ms ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  for(waiting_time = 0; waiting_time <i*SPEED/15; waiting_time++) ;
}//* End

/*
函数名：wait_us()
参数：等待的单位数
返回值：无
描述：通过CPU循环等待，时间单位1个指令周期
*/
void wait_us ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  for(waiting_time = 0; waiting_time < i; waiting_time++) ;
}//* End

