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
*�����ļ�������Ӧ�õ�ͳһ״̬��ת���ɴ˽���
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�
#include "efs.h"
#include "mc.h"
#include "led.h"
#include "golbe.h"


//��������
//��������
MC_SYS_STATE mc_sys_state = MC_SYS_NO_SET;
unsigned long mc_c_addr_cnt = 0;
unsigned long mc_c_addr_arry[6];

unsigned long mc_d_addr_cnt = 0;
unsigned long mc_d_addr_arry[6];

char mc_sys_num[20] = {0};
char mc_password[5] = {0};
//��������
//��������
/*
��������APP_mc
��������
����ֵ����
������FLASH��ʼ��
*/
void APP_mc(void)
{
  
  //��flash��ȡ���ݵ�mc_c_addr_arry,mc_c_addr_cnt
  mc_c_addr_cnt = TS_efs_nv_read(NV_C_COUNT_I);
  if (mc_c_addr_cnt <6)
  {
  for (int i = 0;i < mc_c_addr_cnt;i++)
  {
    mc_c_addr_arry[i] = TS_efs_nv_read(NV_C_ADD_S_I+i);
  }
  }
  else
  {
    mc_c_addr_cnt = 0;
  }
  //��flash��ȡ���ݵ�mc_d_addr_arry,mc_d_addr_cnt
  mc_d_addr_cnt = TS_efs_nv_read(NV_D_COUNT_I);
  if (mc_d_addr_cnt <20)
  {
  for (int i = 0;i < mc_d_addr_cnt;i++)
  {
    mc_d_addr_arry[i] = TS_efs_nv_read(NV_D_ADD_S_I+i);
  }
  }
  else
  {
    mc_d_addr_cnt = 0;
  }
  
}

MC_SYS_STATE mc_get_state(void)
{
  return mc_sys_state;
}

unsigned char mc_set_state(MC_SYS_STATE new_state)
{
  unsigned char result = 0;
  
  //ÿ��״̬����ǰ�������豸ָʾ�ظ�λ��
  TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_OFF);
  TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_OFF);
  TS_phe_buzz_ind(GLOBE_MIN_BUZZ,GLOBE_BUZZ_MUTE);
  TS_phe_buzz_ind(GLOBE_MAX_BUZZ,GLOBE_BUZZ_MUTE);
    //������ʱ��Ҳ��λ��
  switch (new_state)
  {
  case MC_SYS_SET_C:
    if ((mc_sys_state == MC_SYS_PORTECT)||(mc_sys_state == MC_SYS_UNPORTECT)||(mc_sys_state == MC_SYS_NO_SET))
    {
      mc_sys_state = new_state;
      result =1;
      //���ú�Ƴ���
      TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
    }
    break;
  case MC_SYS_SET_D:
    if (mc_sys_state == MC_SYS_SET_C)
    {
      mc_sys_state = new_state;
      result =1;
      //���ûƵƳ���
      TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
    }
    break;
  case MC_SYS_SET_NUM:
    if (mc_sys_state == MC_SYS_SET_D)
    {
      mc_sys_state = new_state;
      result =1;
      //���ú�ƵƳ���
      TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_ON);
      TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_ON);
    }
    break;
  case MC_SYS_RESET:
      mc_sys_state = new_state;
      result =1;
      //����˫����������
      TS_phe_buzz_ind(GLOBE_MIN_BUZZ,GLOBE_BUZZ_RING);
    break;
  case MC_SYS_NO_SET:
    if (mc_sys_state == MC_SYS_RESET)
    {
      mc_sys_state = new_state;
      result =1;
      //����˫��һ����
      TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_BLINK);
      TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_BLINK);
    }
    break;
  case MC_SYS_UNPORTECT:
    if ((mc_sys_state == MC_SYS_PORTECT)||(mc_sys_state == MC_SYS_SET_NUM))
    {
      mc_sys_state = new_state;
      result =1;
      //���ûƵ�����
      TS_phe_led_ind(GLOBE_LED_YELLOW,GLOBE_LED_BLINK);
    }
    break;
  case MC_SYS_PORTECT:
    if (mc_sys_state == MC_SYS_UNPORTECT)
    {
      mc_sys_state = new_state;
      result =1;
      //���ú������
      TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_BLINK);
    }
    break;
  case MC_SYS_ALARM:
    if (mc_sys_state == MC_SYS_PORTECT)
    {
      mc_sys_state = new_state;
      result =1;
      //���ú�ƿ���
      TS_phe_led_ind(GLOBE_LED_RED,GLOBE_LED_QBLINK);
      TS_phe_buzz_ind(GLOBE_MAX_BUZZ,GLOBE_BUZZ_RING);
      //����ʱ��ֻ��15�룬��ע��һ����ʱ��ʱ�䵽�������¼����л�״̬�Ӷ�ֹͣ��
    }
    break;
  }
    return result;
}



unsigned int MC_check_addr_in_c(int addr)
{
  for (int i = 0;i<mc_c_addr_cnt;i++)
  {
    if ((mc_c_addr_arry[i]& 0xfffff) == (addr & 0xfffff))
    {
      return 1;
    }
  }
  return 0;
}

unsigned int MC_write_addr_in_c(int addr)
{
  if (mc_c_addr_cnt > 5)
    return 0;
  addr = addr & 0xfffff;
  
  TS_efs_nv_write(NV_C_ADD_S_I + mc_c_addr_cnt,addr);
  
  mc_c_addr_arry[mc_c_addr_cnt] = addr;
  mc_c_addr_cnt++;
  TS_efs_nv_write(NV_C_COUNT_I,mc_c_addr_cnt);
  return 1;
}


unsigned int MC_check_addr_in_d(int addr)
{
  for (int i = 0;i<mc_d_addr_cnt;i++)
  {
    if ((mc_d_addr_arry[i]& 0xfffff) == (addr & 0xfffff))
    {
      return 1;
    }
  }
  return 0;
}

unsigned int MC_write_addr_in_d(int addr)
{
  if (mc_d_addr_cnt > 20)
    return 0;
  addr = addr & 0xfffff;
  
  TS_efs_nv_write(NV_D_ADD_S_I + mc_d_addr_cnt,addr);
  
  mc_d_addr_arry[mc_d_addr_cnt] = addr;
  mc_d_addr_cnt++;
  TS_efs_nv_write(NV_D_COUNT_I,mc_d_addr_cnt);
  return 1;
}

void MC_write_addr_in_n(void)
{
  int i=0;
  while (mc_sys_num[i] != 0)
  {
    TS_efs_nv_write(NV_N_ADD_S_I,mc_sys_num[i]);
    i++;
  }
  
}

void MC_write_password_in(void)
{
  int i=0;
  for (i=0;i<4;i++) 
  {
    TS_efs_nv_write(NV_PASS_S_I,mc_password[i]);
  }
  
}

void mc_set_sys_num(char * num_p, int len)
{
  int i;
   if ((mc_get_state() != MC_SYS_SET_NUM) || (0 == strlen(mc_sys_num)))//��������״̬��������Ѿ������򷵻ء�
     return ;
   
   for (i =0;i<len ;i++)
   {
     mc_sys_num[i] = num_p[i];
   }
   mc_sys_num[i] = 0;
   //д�뵽NV����
   MC_write_addr_in_n();
}

