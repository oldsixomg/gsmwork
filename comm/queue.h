/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为队列数据结构定义
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef QUEUE_H
#define QUEUE_H
//宏定义


//头文件


//数据结构定义
typedef struct _QUEUE_S
{
unsigned char *base;//队列开始地址
unsigned long  max_size;//队列总大小,实际存储字节数小一个。
signed long    fornt;//队头
signed long    real;//队尾偏移地址
unsigned long  count;//队尾偏移地址
}QUEUE_S;

//函数声明
/*
函数名：queue_init
参数：queue_cb循环队列控制结构指针,base循环队列起始地址，lenght队列所占字节数
返回值：无
描述：初始化循环队列的控制结构
*/
void queue_init(QUEUE_S *queue_ptr , unsigned char *base , unsigned long lenght);

/*
函数名：queue_push_byte 
参数：queue_cb循环队列控制结构指针,data要放到队列的数据
返回值：0为队列满，1为入队成功
描述：往队列里面放一个字节
*/
unsigned char queue_push_byte(QUEUE_S *queue_ptr , unsigned char data);

/*
函数名：queue_pop_byte
参数：queue_cb循环队列控制结构指针,data_ptr出队的地址
返回值：0为队列空，1为出队成功
描述：从队列里面取一个字节
*/
unsigned char queue_pop_byte(QUEUE_S *queue_ptr , unsigned char * data_ptr);

/*
函数名：queue_reset
参数：
返回值：
描述：从队列里面全部清空
*/
void queue_reset(QUEUE_S *queue_ptr);

#endif
//QUEUE_H




