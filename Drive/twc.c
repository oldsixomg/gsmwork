/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*���ļ�Ϊ�����ļ���ִ��Ӳ����ʼ�����жϴ���Ӳ��������״̬���������ṩ�Ľӿڲ��䡣
*ͬʱ�ļ�������������ͬ�ķ���һ�𣬸��������ļ�֮�����ֱ�ӵ��á�
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��
typedef enum
{
  DR_TWC_NONE = 0,
  DR_TWC_INIT,
  DR_TWC_SYNC1,
  DR_TWC_SYNC2,
  DR_TWC_CONTENT,
}DR_TWC_STATE;




//ͷ�ļ�
#include "drive.h"
#include "twc.h"
#include "os.h"
#include "led.h"


//��������
//��������
static DR_TWC_STATE dr_twc_state ;
static unsigned long dr_twc_valid_addr;
static unsigned int dr_twc_sync_count;
static unsigned int dr_twc_sync_count1;
static unsigned int dr_twc_sync_ms_len;
static unsigned int dr_twc_gap_count;

//��������
//��������
/*
��������DR_twc_1ms_cb
�������� 
����ֵ����
������ûһ��ִ��һ�Σ���������Ƿ��е͵�ƽ��ʼ
*/
void DR_twc_1ms_cb(void)   //TIME1
{ 
  if (0 == (GLOBE_GET_INT_STATE() & GLOBE_TWC_RX))
  {
    //û������
    if (0 == (GLOBE_GET_KEY() & GLOBE_TWC_RX))
    {//�͵�ƽ
      if (dr_twc_state > DR_TWC_SYNC1)
      {
        return ;
      }
      if (dr_twc_sync_count== 7)//��СGUARDʱ��6MS
      {//����4MS�ĳ����͵�ƽ������̽�⵽ͬ��ͷ
        DR_twc_start_recv();
      }
      dr_twc_sync_count++;//�������պ���Ҫ��ʱ
    }
    else
    {//�ߵ�ƽ
      dr_twc_sync_count= 0;
    }
  }
  else
  {
    //���������0
    dr_twc_sync_count= 0;
  }
 
}
/*
��������DR_twc_init
��������
����ֵ����
���������������ճ�ʼ��
*/
void DR_twc_init()
{
  GLOBE_SET_GPIO_IN(GLOBE_TWC_RX); 
  GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
  dr_twc_state = DR_TWC_INIT;
  dr_twc_sync_count= 0;
  
}

/*
��������DR_twc_start_recv
��������
����ֵ����
������
*/
void DR_twc_start_recv()
{
  //���ж�
  GLOBE_GPIO_INT_EN(GLOBE_TWC_RX);
  dr_twc_state = DR_TWC_SYNC1;//����ͬ���������������ʶ��ͬ������
}
/*
��������DR_twc_get_result
��������
����ֵ����
������
*/
void DR_twc_get_result()
{
  //�ر��ж�
  DR_twc_result_report(&dr_twc_valid_addr);
  DR_twc_exit();
}
/*
��������DR_twc_exit
��������
����ֵ����
������
*/
void DR_twc_exit()
{
  dr_twc_state = DR_TWC_INIT;
  //�ر��ж�,��1ms��ʱ��������
  GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
  dr_twc_sync_count = 0;
}
/*
��������DR_twc_count_start
��������
����ֵ����
������
*/
void DR_twc_count_start()
{
  timer1_start_count();
}

/*
��������DR_twc_get_count
��������
����ֵ����
������
*/
unsigned int DR_twc_get_count()
{
  return  timer1_get_count();
}
/*
��������DR_twc_isr_cb
��������
����ֵ����
�����������������жϷ���ʱ���õ������
*/
void DR_twc_isr_cb()    //�ʼ�ж����ˣ�����ʶ�𣬶�ʱ��0.2ms���һ�µ͵�ƽ��IO�жϹرգ���ֱ��ͬ������Ŵ��ж�ֹͣ������
{
  switch (dr_twc_state)
  {
  case DR_TWC_INIT:   //���г�ʼ��״̬�յ��ж�
    break;
    
  case DR_TWC_SYNC1:        //~~|____|
    if (dr_twc_sync_count> 13)
    {
      DR_twc_exit();
    }
    else
    {
      dr_twc_sync_ms_len =dr_twc_sync_count;
      DR_twc_count_start();
      dr_twc_state = DR_TWC_SYNC2;
    }
    break;
    
  case DR_TWC_SYNC2:   //~~|____|~~|   ==��һ��GAP�½�
    dr_twc_sync_count1= DR_twc_get_count();
    if ((dr_twc_sync_count1> 10*dr_twc_sync_ms_len)&&(dr_twc_sync_count1< 40*dr_twc_sync_ms_len ))
    {
      dr_twc_valid_addr = 0;
      dr_twc_state =DR_TWC_CONTENT;
    }
    else
    {
      DR_twc_exit();
    }
    break;
    
  case DR_TWC_CONTENT:  //~~|____|~~|___|   ==��һ��GAP����
    dr_twc_gap_count = DR_twc_get_count();
    if ((dr_twc_gap_count < dr_twc_sync_ms_len*36)||(dr_twc_gap_count > 52*dr_twc_sync_ms_len))
    {
      DR_twc_exit();
      break;
    }
    GLOBE_GPIO_INT_DIS(GLOBE_TWC_RX);
    for (int i =1;i<24;i++)
    {
      DR_twc_count_start();
      while (0 == (GLOBE_GET_INT_STATE()& GLOBE_TWC_RX));
      dr_twc_gap_count  = DR_twc_get_count();
      if (dr_twc_gap_count > 300)
      {
        dr_twc_valid_addr= (1 << i)|dr_twc_valid_addr;
      }
      else if (dr_twc_gap_count < 100)
      {
        DR_twc_exit();
        return;
      }
      while (0 == (GLOBE_GET_INT_STATE() & GLOBE_TWC_RX));
      dr_twc_gap_count  = DR_twc_get_count();
      if (dr_twc_gap_count > 500 )
      {
        DR_twc_exit();
        return;
      }
    }
    //�õ�������
    DR_twc_get_result();
    break;
  }
}
/*
��������DR_twc_result_report
��������
����ֵ����
������������������ɺ��ϱ����ݸ��ϲ�
*/
void DR_twc_result_report(unsigned int * num)
{
  OS_REPORT_TWC_MSG(GLOBE_TWC_RX,num);
}


