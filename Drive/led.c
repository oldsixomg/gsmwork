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
*ָʾ�������ļ�
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��




//ͷ�ļ�
#include "led.h"
#include "drive.h"


//��������
//��������
TIME_REG_STRUCT dr_yled_time;
TIME_REG_STRUCT dr_rled_time;
unsigned int dr_led_red_state;
unsigned int dr_led_yellow_state;

//��������
//��������
/*
��������DR_led_init
��������
����ֵ����
������ָʾ�Ƴ�ʼ��
*/
void DR_led_init(void)
{
  GLOBE_SET_GPIO_OUT(GLOBE_LED_RED);
  GLOBE_SET_GPIO_OUT(GLOBE_LED_YELLOW);
  
  GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
  GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
  
  dr_yled_time.cb = DR_yled_cb;
  dr_rled_time.cb = DR_rled_cb;
}
/*
��������DR_led_set
��������
����ֵ����
����������ָʾ�ƹ���״̬
*/
void DR_led_set(unsigned int color,unsigned int state)
{
   if (GLOBE_LED_RED == color)
   {
     dr_led_red_state = state;
     switch (state)
     {
     case GLOBE_LED_ON:
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_RED);
       break;
     case GLOBE_LED_OFF:
       GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
       break;
     case GLOBE_LED_BLINK:
       dr_rled_time.timeout = 100;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_rled_time);
       break;
     case GLOBE_LED_QBLINK:
       dr_rled_time.timeout = 20;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_rled_time);
       break;
     }
   }
   
   if (GLOBE_LED_YELLOW == color)
   {
     dr_led_yellow_state = state;
     switch (state)
     {
     case GLOBE_LED_ON:
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       break;
     case GLOBE_LED_OFF:
       GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
       break;
     case GLOBE_LED_BLINK: 
       dr_yled_time.timeout = 100;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_yled_time);
       break;
     case GLOBE_LED_QBLINK:
       dr_yled_time.timeout = 20;
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
       time_client_reg(&dr_yled_time);
       break;
     }
   }
  
}
/*
��������DR_led_check
��������
����ֵ����
��������ѯ��ǰָʾ��״̬
*/
unsigned char DR_led_check(unsigned int color)
{
  if (GLOBE_GET_LED()&color)
    return 1;
  else
    return 0;
}

/*
��������DR_yled_cb
��������
����ֵ����
����������ָʾ�ƹ���״̬
*/
void DR_yled_cb(void)
{  
  if (dr_led_yellow_state == GLOBE_LED_BLINK)
  {
      dr_yled_time.timeout = 100;
  }
  else if (dr_led_yellow_state == GLOBE_LED_QBLINK)
  {
      dr_yled_time.timeout = 20;
  }
  else
  {
    return ;
  }  
  
  if (DR_led_check(GLOBE_LED_YELLOW) == 0)
  {
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_YELLOW);
  }
  else
  {
       GLOBE_SET_GPIO_LOW(GLOBE_LED_YELLOW);
  }
  
  time_client_reg(&dr_yled_time);
}

/*
��������DR_rled_cb
��������
����ֵ����
����������ָʾ�ƹ���״̬
*/
void DR_rled_cb(void)
{
  if (dr_led_red_state == GLOBE_LED_BLINK)
  {
      dr_rled_time.timeout = 100;
  }
  else if (dr_led_red_state == GLOBE_LED_QBLINK)
  {
      dr_rled_time.timeout = 20;
  }
  else
  {
    return ;
  }
  
  if (DR_led_check(GLOBE_LED_RED) == 0)
  {
       GLOBE_SET_GPIO_HIGH(GLOBE_LED_RED);
  }
  else
  {
       GLOBE_SET_GPIO_LOW(GLOBE_LED_RED);
  }
  time_client_reg(&dr_rled_time);
}


