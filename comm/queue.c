/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *���ļ�Ϊ���з�����ʵ��
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

//�궨��

//ͷ�ļ�
#include "queue.h"

//ȫ�ֱ���

//���ر���

//�ⲿ����


//����ʵ��
/*
��������queue_init
������queue_cbѭ�����п��ƽṹָ��,baseѭ��������ʼ��ַ��lenght������ռ�ֽ���
����ֵ����
��������ʼ��ѭ�����еĿ��ƽṹ
*/
void queue_init(QUEUE_S *queue_ptr , unsigned char *base , unsigned long lenght)
{
    queue_ptr->base   = base;
    queue_ptr->max_size   = lenght;
    queue_ptr->fornt  = 0;
    queue_ptr->real   = 0;
    queue_ptr->count  = 0;
}

/*
��������queue_get_count 
������queue_cbѭ�����п��ƽṹָ��,
����ֵ�����д���ֽ���
�����������������һ���ֽ�
*/
unsigned long queue_get_count(QUEUE_S *queue_ptr )
{
    return queue_ptr->count;
}

/*
��������queue_push_byte 
������queue_cbѭ�����п��ƽṹָ��,dataҪ�ŵ����е�����
����ֵ��0Ϊ��������1Ϊ��ӳɹ�
�����������������һ���ֽ�
*/
unsigned char queue_push_byte(QUEUE_S *queue_ptr , unsigned char data)
{
    if ((1 + queue_ptr->count) >= queue_ptr->max_size) //�����Ѿ�����
       return 0;
    
    queue_ptr->base[queue_ptr->real] = data;
    queue_ptr->real = (queue_ptr->real + 1)%queue_ptr->max_size;
    queue_ptr->count++;
    return 1;
}

/*
��������queue_pop_byte
������queue_cbѭ�����п��ƽṹָ��,data_ptr���ӵĵ�ַ
����ֵ��0Ϊ���пգ�1Ϊ���ӳɹ�
�������Ӷ�������ȡһ���ֽ�
*/
unsigned char queue_pop_byte(QUEUE_S *queue_ptr , unsigned char * data_ptr)
{
     if (0 == queue_ptr->count)
        return 0;
     
     *data_ptr = queue_ptr->base[queue_ptr->fornt];
     queue_ptr->fornt = (queue_ptr->fornt + 1)%queue_ptr->max_size;
     queue_ptr->count--;
     return 1;

}
/*
��������queue_reset
������
����ֵ��
�������Ӷ�������ȫ�����
*/
void queue_reset(QUEUE_S *queue_ptr)
{
    queue_ptr->fornt  = 0;
    queue_ptr->real   = 0;
    queue_ptr->count  = 0;
}

