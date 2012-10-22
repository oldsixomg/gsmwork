/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *���ļ�Ϊ�������ݽṹ����
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef QUEUE_H
#define QUEUE_H
//�궨��


//ͷ�ļ�


//���ݽṹ����
typedef struct _QUEUE_S
{
unsigned char *base;//���п�ʼ��ַ
unsigned long  max_size;//�����ܴ�С,ʵ�ʴ洢�ֽ���Сһ����
signed long    fornt;//��ͷ
signed long    real;//��βƫ�Ƶ�ַ
unsigned long  count;//��βƫ�Ƶ�ַ
}QUEUE_S;

//��������
/*
��������queue_init
������queue_cbѭ�����п��ƽṹָ��,baseѭ��������ʼ��ַ��lenght������ռ�ֽ���
����ֵ����
��������ʼ��ѭ�����еĿ��ƽṹ
*/
void queue_init(QUEUE_S *queue_ptr , unsigned char *base , unsigned long lenght);

/*
��������queue_push_byte 
������queue_cbѭ�����п��ƽṹָ��,dataҪ�ŵ����е�����
����ֵ��0Ϊ��������1Ϊ��ӳɹ�
�����������������һ���ֽ�
*/
unsigned char queue_push_byte(QUEUE_S *queue_ptr , unsigned char data);

/*
��������queue_pop_byte
������queue_cbѭ�����п��ƽṹָ��,data_ptr���ӵĵ�ַ
����ֵ��0Ϊ���пգ�1Ϊ���ӳɹ�
�������Ӷ�������ȡһ���ֽ�
*/
unsigned char queue_pop_byte(QUEUE_S *queue_ptr , unsigned char * data_ptr);

/*
��������queue_reset
������
����ֵ��
�������Ӷ�������ȫ�����
*/
void queue_reset(QUEUE_S *queue_ptr);

#endif
//QUEUE_H




