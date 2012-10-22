/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为队列方法的实现
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

//宏定义

//头文件
#include "queue.h"

//全局变量

//本地变量

//外部变量


//函数实现
/*
函数名：queue_init
参数：queue_cb循环队列控制结构指针,base循环队列起始地址，lenght队列所占字节数
返回值：无
描述：初始化循环队列的控制结构
*/
void queue_init(QUEUE_S *queue_ptr , unsigned char *base , unsigned long lenght)
{
    queue_ptr->base   = base;
    queue_ptr->max_size   = lenght;
    queue_ptr->fornt  = 0;
    queue_ptr->real   = 0;
    queue_ptr->count  = 0;
}

/*
函数名：queue_get_count 
参数：queue_cb循环队列控制结构指针,
返回值：队列存放字节数
描述：往队列里面放一个字节
*/
unsigned long queue_get_count(QUEUE_S *queue_ptr )
{
    return queue_ptr->count;
}

/*
函数名：queue_push_byte 
参数：queue_cb循环队列控制结构指针,data要放到队列的数据
返回值：0为队列满，1为入队成功
描述：往队列里面放一个字节
*/
unsigned char queue_push_byte(QUEUE_S *queue_ptr , unsigned char data)
{
    if ((1 + queue_ptr->count) >= queue_ptr->max_size) //队列已经满了
       return 0;
    
    queue_ptr->base[queue_ptr->real] = data;
    queue_ptr->real = (queue_ptr->real + 1)%queue_ptr->max_size;
    queue_ptr->count++;
    return 1;
}

/*
函数名：queue_pop_byte
参数：queue_cb循环队列控制结构指针,data_ptr出队的地址
返回值：0为队列空，1为出队成功
描述：从队列里面取一个字节
*/
unsigned char queue_pop_byte(QUEUE_S *queue_ptr , unsigned char * data_ptr)
{
     if (0 == queue_ptr->count)
        return 0;
     
     *data_ptr = queue_ptr->base[queue_ptr->fornt];
     queue_ptr->fornt = (queue_ptr->fornt + 1)%queue_ptr->max_size;
     queue_ptr->count--;
     return 1;

}
/*
函数名：queue_reset
参数：
返回值：
描述：从队列里面全部清空
*/
void queue_reset(QUEUE_S *queue_ptr)
{
    queue_ptr->fornt  = 0;
    queue_ptr->real   = 0;
    queue_ptr->count  = 0;
}

