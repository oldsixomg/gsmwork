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
*�����������ļ�
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
#include "golbe.h"


//��������
//��������


//��������
//��������
/*
��������DR_buzz_init
��������
����ֵ����
��������������ʼ��
*/
void DR_buzz_init()
{
   GLOBE_SET_GPIO_OUT(GLOBE_MIN_BUZZ);
   GLOBE_SET_GPIO_OUT(GLOBE_MAX_BUZZ);

   GLOBE_SET_GPIO_LOW(GLOBE_MIN_BUZZ);
   GLOBE_SET_GPIO_LOW(GLOBE_MAX_BUZZ);
}

/*
��������DR_buzz_set
��������
����ֵ����
���������÷���������״̬
*/
void DR_buzz_set(unsigned int num,unsigned int state)
{
    if (GLOBE_BUZZ_RING ==state)   //������
    {
       GLOBE_SET_GPIO_HIGH(num);
    }
    else                       //�����������Ϩ��
    {
       GLOBE_SET_GPIO_LOW(num);
     }
}


/*
��������DR_buzz_check
��������
����ֵ����
��������ѯ��ǰ������״̬
*/
unsigned int DR_buzz_check(unsigned int num)
{
   if (num &GLOBE_CHECK_GPIO())
         return 1;
   else
         return 0;
}
