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
#include "bsp.h"


#define TS_Q_PHE_CMD         0x0001


extern OS_FLAG_GRP  *ts_phe_cmd_event;
/*
��������TS_phe_task
��������
����ֵ����
������
*/
void TS_phe_task(void *p_arg);


void TS_phe_led_ind(unsigned int color,unsigned int state);