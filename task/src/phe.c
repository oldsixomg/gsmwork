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
#include "button.h"
#include "phe.h"
#include "os.h"
#include "twc.h"



//��������
//��������
OS_FLAGS     ts_phe_event_id;
OS_FLAG_GRP  *ts_phe_cmd_event;

//��������
//��������

/*
��������TS_phe_task
��������
����ֵ����
������FLASH��ʼ��
*/
void TS_phe_init(void)
{
  DR_key_init();
  DR_led_init();
  DR_twc_init();
  
  


/**/
}
/*
��������TS_phe_task
��������
����ֵ����
������FLASH��ʼ��
*/
void TS_phe_task(void *p_arg)
{
  
  unsigned char       err;  
  OS_CMD *cmd_ptr;
  
  TS_phe_init();
  ts_phe_cmd_event = OSFlagCreate ( 0 , &err);
  
  while(1)
  { 
    ts_phe_event_id =OSFlagPend(ts_phe_cmd_event,( TS_Q_PHE_CMD ),(OS_FLAG_WAIT_SET_ANY |OS_FLAG_CONSUME ), 0 , &err);
    if (TS_Q_PHE_CMD & ts_phe_event_id)
    {
      //�������������ѭ��ȡ��Ϣ����
      while ((cmd_ptr = q_get(&phe_cmd_alloc)) != NULL)
      {
        if (cmd_ptr->cmd_id == GLOBE_RST_KEY)
        {
          APP_btn_set(GLOBE_RST_KEY);      
        }
        
        if (cmd_ptr->cmd_id == GLOBE_SET_KEY)
        {
          APP_btn_set(GLOBE_SET_KEY);    
        }
        if (cmd_ptr->cmd_id == GLOBE_TWC_RX)
        {
#if 1
          char contet_s[15] = "+CMTI:\"ME\",1";
    DSM_ITEM * item = dsm_alloc_item();
    for (int i=0;i<13;i++)
    {
      item->data_ptr[i] = contet_s[i];
      item->used++;
    }
          sio_put_cmd(1,item);
    break;
   
#endif
          //��MC��ȡ��ǰϵͳ״̬                        
          if (mc_get_state()==1)
          {
            //����SET����ĺ������д���
            APP_twc_set((unsigned int*)cmd_ptr->data_p);
          }                 
           else if (mc_get_state()==2)
          {
          //����ALARM����ĺ������д���
          APP_twc_alarm((unsigned int*)cmd_ptr->data_p);
        }                 
           else if (mc_get_state()==3)
          {
          //����MONITOR����ĺ������д���
          APP_twc_monitor((unsigned int*)cmd_ptr->data_p);
        }
          
        }
        
        
        
      }
    }
  }
}

/*
��������TS_phe_led_ind
��������
����ֵ����
������FLASH��ʼ��
*/
void TS_phe_led_ind(unsigned int color,unsigned int state)
{
  DR_led_set(color,state);
}

/*
��������TS_phe_buzz_ind
��������
����ֵ����
������FLASH��ʼ��
*/
void TS_phe_buzz_ind(unsigned int device,unsigned int state)
{
  DR_buzz_set(device,state);
  
}




