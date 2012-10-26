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


//�궨��


//ͷ�ļ�
#include "sio.h"
#include "os.h"




//��������
//��������
OS_FLAGS     ts_sio_event_id;
OS_FLAG_GRP  *ts_sio_cmd_event;
SIO_DEVICE_PORT globe_device_port[];

//��������
//��������
extern void DR_rs232_send(void * ptr,unsigned int used);
extern void APP_gsm_recv(void * ptr,int len);

void TS_sio_init(void)
{
  DR_rs232_init();
  globe_device_port[SIO_RS232_STREAM_ID].send = DR_rs232_send;
  globe_device_port[SIO_RS232_STREAM_ID].recv = APP_gsm_recv;
  globe_device_port[SIO_RS232_STREAM_ID].stream_id = SIO_RS232_STREAM_ID;
  
  APP_gsm();
}

/*
��������TS_sio_task
��������
����ֵ����
������FLASH��ʼ��
*/
void TS_sio_task(void *p_arg)
{
  unsigned char       err; 
  OS_CMD *cmd_ptr;
  
  ts_sio_cmd_event = OSFlagCreate ( 0 , &err);
  
  TS_sio_init();
  
  while(1)
  { 
    ts_sio_event_id =OSFlagPend(ts_sio_cmd_event,( TS_Q_SIO_CMD ),(OS_FLAG_WAIT_SET_ANY |OS_FLAG_CONSUME ), 0 , &err);
    if (TS_Q_SIO_CMD & ts_sio_event_id)  //������Ϣ����
    {
        //�������������ѭ��ȡ��Ϣ����
        while ((cmd_ptr = q_get(&sio_cmd_alloc)) != NULL)
        {
           if (cmd_ptr->cmd_id == RS232_UP_CMD)
           {
              globe_device_port[((DSM_ITEM *)(cmd_ptr->data_p))->stream_id].recv((DSM_ITEM *)(cmd_ptr->data_p),0);
           }
           free_a_cmd(cmd_ptr);
           dsm_free_item((DSM_ITEM *)(cmd_ptr->data_p));
        }
    }
  }
}

void TS_sio_transmit(DSM_ITEM *item)
{
    globe_device_port[item->stream_id].send(item->data_ptr,item->used);
    dsm_free_item(item);
}




