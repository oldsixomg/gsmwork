/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为串型数据ID定义。
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef DATA_H
#define DATA_H

//宏定义
#define DS_MAX_CMD_CNT   32

//头文件
#include "task.h"


//类型定义

typedef enum{
DATA_STREAM_UART1_ID = 0,
DATA_STREAM_UART2_ID,
DATA_STREAM_USB_ID,
DATA_STREAM_SFN_ID,
DATA_STREAM_EFS_ID,
DATA_STREAM_TWC_ID,
DATA_STREAM_GPIO_ID,
DATA_STREAM_ALARM_ID,
DATA_STREAM_MAX_ID
}DATA_STREAM_ID;


typedef struct _DS_CMD{
DATA_STREAM_ID id;
void *data_p;
struct _DS_CMD* next;
}DS_CMD;

typedef struct {
DATA_STREAM_ID source;
DATA_STREAM_ID raw_dest;
DATA_STREAM_ID at_dest;
void (*func_cb)(DS_CMD *) ;
}STREAM_ID_DIRECT;



typedef struct{
DS_CMD * head;
DS_CMD * last;
}DS_CMD_LIST;

#define DSM_ITEM_MAX_SIZE 64
#define DSM_ITEM_MAX_CNT  20
typedef struct _DSM_ITEM{
struct _DSM_ITEM * next;
unsigned long cnt;
unsigned char buff[DSM_ITEM_MAX_SIZE];
}DSM_ITEM;

//事件定义
#define DS_Q_CMD         0x0001
//定义系统状态变量结构
typedef enum{
  SYSTEM_TIME = 1,
  SYSTEM_RSSI,
  SYSTEM_BATTERY,
}SYSTEM_DB_ITEM;

typedef struct _SYSTEM_DB
{
unsigned long system_time;
unsigned char rssi;
unsigned char battery;
}SYSTEM_DB;

typedef struct _SYSTEM_DB_SET
{
    SYSTEM_DB_ITEM item;
    unsigned long value;
}SYSTEM_DB_SET;

//变量定义
extern  OS_FLAGS     data_event_id;
extern OS_FLAG_GRP  *data_cmd_event;

extern SYSTEM_DB db;




/*
函数名：updata_system_db
参数：
返回值：无
描述：更新系统状态
*/
void updata_system_db(SYSTEM_DB_SET *);

extern DSM_ITEM * dsm_alloc_item(void);
#endif//DATA_H


