/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *���ļ�Ϊ
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef ALARM_H
#define ALARM_H

//�궨��

//ͷ�ļ�
#include "task.h"


//���Ͷ���
typedef enum
{
ALARM_NORMAL_STATUS = 0,
ALARM_CONTROL_SET_STATUS,
ALARM_DEVICE_SET_STATUS,
ALARM_MAX_STATUS
}ALARM_TWC_STATUS;

//�¼�����
#define ALARM_TWC_CMD         0x0001
#define ALARM_KEY_CMD         0x0002

extern OS_FLAG_GRP  *alarm_cmd_event;

extern void alarm_recv_cmd_cb(DS_CMD *cmd);
#endif//ALARM_H


