/* ----------------------------------------------------------------------------
* WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*���ļ�Ϊ�����ļ���ִ��Ӳ����ʼ�����жϴ���Ӳ��������״̬���������ṩ�Ľӿڲ��䡣
*ͬʱ�ļ�������������ͬ�ķ���һ�𣬸��������ļ�֮�����ֱ�ӵ��á�
*
*���������ļ�
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��

//ͷ�ļ�

#include "type.h"
#include "drive.h"
#include "button.h"
#include "os.h"

//��������
//��������
unsigned short dr_rst_low_cnt=0;
unsigned short dr_set_low_cnt=0;
TIME_REG_STRUCT dr_rst_time;
TIME_REG_STRUCT dr_set_time;
//��������
//��������
/*
��������DR_key_init
��������
����ֵ����
������������ʼ��
*/
void DR_key_init()
{
  GLOBE_SET_GPIO_IN(GLOBE_RST_KEY);
  GLOBE_SET_GPIO_IN(GLOBE_SET_KEY);
  
  GLOBE_GPIO_INT_EN(GLOBE_RST_KEY);
  GLOBE_GPIO_INT_EN(GLOBE_SET_KEY);
  
  dr_rst_time.cb = DR_rst_cb;
  dr_set_time.cb = DR_set_cb;
}

/*
��������DR_key_report
��������
����ֵ����
����������ʶ������ϲ㱨
*/
void DR_key_report(uint32 num)
{
  /*���Ͱ����¼����ϱ�����Ӧ����*/
  OS_REPORT_KEY_EVENT(num);
  
}

/*
��������DR_rst_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_rst_cb(void)
{
    if (DR_key_input_check(GLOBE_RST_KEY))
    {
      return;
    }
    
    if (dr_rst_low_cnt < 10)
    {
      dr_rst_low_cnt++;
      dr_rst_time.timeout = 20;
      time_client_reg(&dr_rst_time);
    }
    else
    {
      DR_key_report(GLOBE_RST_KEY);
    }
}



/*
��������DR_set_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_set_cb(void)
{
    if (DR_key_input_check(GLOBE_SET_KEY))
    {
      return;
    }
    if (dr_set_low_cnt < 2)
    {
      dr_set_low_cnt++;
      dr_set_time.timeout = 20;
      time_client_reg(&dr_set_time);
    }
    else
    {
      DR_key_report(GLOBE_SET_KEY);
    }
}



/*
��������DR_key_isr_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_key_isr_cb()
{
  uint32 key_state;
  key_state = ~GLOBE_GET_KEY();  //�ӵص͵�ƽ��Ч������Ļ���ȡ��
  if (GLOBE_RST_KEY&key_state)
  {
    //ע�ᶨʱ��
    dr_rst_low_cnt = 0;
    dr_rst_time.timeout = 20;
    time_client_reg(&dr_rst_time);
    
  }
  else if (GLOBE_SET_KEY &key_state)
  {
    dr_set_low_cnt = 0;
    dr_set_time.timeout = 20;
    time_client_reg(&dr_set_time);
  }
}





/*
��������DR_key_input_check
��������
����ֵ����
��������ѯ���µİ����ߵ͵�ƽ
*/
unsigned int DR_key_input_check(unsigned int num)
{
  if (GLOBE_GET_KEY()&num)
    return 1;
  else
    return 0;
}
