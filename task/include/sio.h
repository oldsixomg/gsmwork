/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/

#ifndef SIO_H
#define SIO_H

#define RS232_UP_CMD  1


typedef struct {
  unsigned int stream_id;
  void (*send)(void *ptr,unsigned int used);
  void (*recv)(void *ptr,int len);
}SIO_DEVICE_PORT;

typedef enum
{
  SIO_NONE_STREAM_ID = -1,
  SIO_RS232_STREAM_ID ,
  SIO_USB_STREAM_ID ,
}SIO_DEVICE_STREAM_ID;

#include "bsp.h"


#define TS_Q_SIO_CMD         0x0001


extern OS_FLAG_GRP  *ts_sio_cmd_event;
/*
��������TS_sio_task
��������
����ֵ����
������
*/
void TS_sio_task(void *p_arg);

#endif //SIO_H

