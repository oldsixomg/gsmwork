/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *time.h ���ļ�Ϊ�����ջ�����ȼ��������ǵ�Ҫ��������ط���һ�����������ȼ���һ����
 *�����ջ
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef TIME_H
#define TIME_H


#define COUNT_TO_SEC_NUM     300

#define TIMER0_INTERRUPT_LEVEL		2
#define TIMER1_INTERRUPT_LEVEL		1

/*-----------------*/
/* Clock Selection */
/*-----------------*/
#define TC_CLKS                  0x7
#define TC_CLKS_MCK2             0x0//MCK/2
#define TC_CLKS_MCK8             0x1
#define TC_CLKS_MCK32            0x2
#define TC_CLKS_MCK128           0x3
#define TC_CLKS_MCK1024          0x4

//�ṹ����
typedef struct _TIME_REG_STRUCT
{
void (*cb)(void);
unsigned int timeout;
struct _TIME_REG_STRUCT * next;
struct _TIME_REG_STRUCT * prev;
}TIME_REG_STRUCT;
typedef struct
{
unsigned short year;
unsigned char month;
unsigned char data;
unsigned char weeks;
}SYS_TIME_STRUCT;

//�ⲿ��������
extern void timer_init ( void );
extern void time_1ms_client_reg( void * client);
void time_client_reg(TIME_REG_STRUCT * client);
void timer1_start_isr(void (*func)(void));
unsigned int timer1_get_count(void);
void timer1_stop_isr(void);

#endif