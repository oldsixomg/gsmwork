/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef ALARM_H
#define ALARM_H

//宏定义

//头文件
#include "task.h"


//类型定义
typedef enum
{
ALARM_NORMAL_STATUS = 0,
ALARM_CONTROL_SET_STATUS,
ALARM_DEVICE_SET_STATUS,
ALARM_MAX_STATUS
}ALARM_TWC_STATUS;

//事件定义
#define ALARM_TWC_CMD         0x0001
#define ALARM_KEY_CMD         0x0002

extern OS_FLAG_GRP  *alarm_cmd_event;

extern void alarm_recv_cmd_cb(DS_CMD *cmd);
#endif//ALARM_H


