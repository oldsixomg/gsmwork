/* ----------------------------------------------------------------------------
*         vteng ��˾�ṩ֧�� 
* ----------------------------------------------------------------------------
* ��Ȩ��2010 VTENG��˾
*
* ��˾��������Ȩ��
*
*���ļ�Ϊ��������ڣ��������ø��ֳ�ʼ����������������������
*
*
*
*
*
* ----------------------------------------------------------------------------
*/


//�궨��
//ͷ�ļ�
#include "drive.h"

//��������
//��������
//��������
//��������
/*
��������wait_s
�������ȴ��ĵ�λ��
����ֵ����
������ͨ��CPUѭ���ȴ���ʱ�䵥λ1��
*/
void wait_s ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  //for(waiting_time = 0; waiting_time < SPEED *50*9*i; waiting_time++) ;
}//* End


/*
��������wait_ms
�������ȴ��ĵ�λ��
����ֵ����
������ͨ��CPUѭ���ȴ���ʱ�䵥λ1����
*/
void wait_ms ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  for(waiting_time = 0; waiting_time <i*SPEED/15; waiting_time++) ;
}//* End

/*
��������wait_us()
�������ȴ��ĵ�λ��
����ֵ����
������ͨ��CPUѭ���ȴ���ʱ�䵥λ1��ָ������
*/
void wait_us ( int i )
{//* Begin
  unsigned int waiting_time ;
  
  for(waiting_time = 0; waiting_time < i; waiting_time++) ;
}//* End

