/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*
*
*APP��Ҫ�ǽ�����Ϣ����Ӧ����Ҫ��һЩ����ִ�еĺ�����������򽻵��ĺ���������ֱ�ӵ��������ײ��ִ�к�����
*
*set�ļ���Ҫ������������������أ�����button,twc,led,buzz,sms,flash.
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�
#include "led.h"
#include "phe.h"


//��������
//��������
static unsigned char btn_set_state = 0;

//��������
//��������
/*
��������APP_sms_set
��������
����ֵ����
������
*/
void APP_sms_set(unsigned char sms_event)
{
    if (sms_event == 1)  //�����ֻ�����
    {
        //�Ƚ������Ƿ�ƥ��
          //DR_flash_write();
        //���ƣ����ȣ���д��EFS,�ط�����
          //TS_phe_sms_ind();
    }
    else if (sms_event == 6) //��������
    {
        //д��EFS���ط�����
          //DR_flash_write();
    }
}

/*
��������APP_btn_set
��������
����ֵ����
������
*/
void APP_btn_set(unsigned int btn_event)
{
    if (btn_event == (1<<29)) //��λ������
    {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
        //���EFS����
          //DR_flash_erase_all();
        //����Ĭ�������
          //DR_flash_write();
    }
    if (btn_event == (1<<30)) //ң��������
    {
        //���еƵĲ���������״̬ 
        if (btn_set_state == 0)   //����ң����
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_OFF);
          btn_set_state++;
        } 
        else if (btn_set_state == 1) //�����豸
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_OFF);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
          btn_set_state++;
        }
        else   //���ú���
        {
          TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_BLINK);
          TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_BLINK);
          btn_set_state = 0;
        }
    }
}

/*
��������APP_twc_set
��������
����ֵ����
������
*/
void APP_twc_set(unsigned int *addr)
{
  //�鿴����״̬���豸����ң����
  if (MC_check_addr_in_c(*addr)) //�ж��Ƿ�����������
  {
    return ;  //���Ǿͷ���
  }
  else
  {
     if (MC_write_addr_in_c(*addr)) //���ھ�д�룬д��ɹ�������
     {
       TS_phe_buzz_ind();
     }
     return ;
  }
}


/*
��������APP_twc_alarm
��������
����ֵ����
������
*/
void APP_twc_alarm(unsigned int *addr)
{
  //�鿴����״̬���豸����ң����
  if (MC_check_addr_in_d(*addr)) //�ж��Ƿ�����������
  {
    return ;  //���Ǿͷ���
  }
  else
  {
     if (MC_write_addr_in_d(*addr)) //���ھ�д�룬д��ɹ�������
     {
       TS_phe_buzz_ind();
     }
     return ;
  }
}



