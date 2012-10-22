/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *���ļ�Ϊ��������ID���塣
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef DATA_H
#define DATA_H

//�궨��
#define DS_MAX_CMD_CNT   32

//ͷ�ļ�
#include "task.h"


//���Ͷ���

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

//�¼�����
#define DS_Q_CMD         0x0001
//����ϵͳ״̬�����ṹ
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

//��������
extern  OS_FLAGS     data_event_id;
extern OS_FLAG_GRP  *data_cmd_event;

extern SYSTEM_DB db;




/*
��������updata_system_db
������
����ֵ����
����������ϵͳ״̬
*/
void updata_system_db(SYSTEM_DB_SET *);

extern DSM_ITEM * dsm_alloc_item(void);
#endif//DATA_H


