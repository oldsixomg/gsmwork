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
static TIME_REG_STRUCT reg_header={0,0};//Ҳ�лص�������������ʱ1��
static unsigned long system_sec = 0;//ϵͳʱ������Ϊ��λ����׼ʱ��Ϊ2010��1��1��0ʱ0��0��
static unsigned short count_ms = 0;//���ں�����������ﵽ1000ʱ�Զ���0
int yearday[]={
  365,//2010��365��,����365��ͱ��2011��
  730,//2011��730
  1096,//2012��366
  1461,//2013:365
  1826,//2014:365
  2191,//2015:365
  2557,//2016:366
  2922,//2017:365
  3287,//2018:365
  3652,//2019:365
  4018,//2020:366
}; //
int month[]={31,28,31,30,31,30,31,31,30,31,30,31};/*���һ����ʾ����2�µ�����*/
static SYS_TIME_STRUCT sysTimes;

void (*timer1_500us_isr_cb)(void);

//��������
//��������
void set_systime(unsigned long time)
{
  system_sec = time;
}

SYS_TIME_STRUCT * get_systime(void)
{
  
  unsigned long data;
  unsigned short i , k , j;
  data = system_sec/86400;
  for ( i= 0;i<10;i++)
  {
    if (yearday[i] > data)
      break;
  }
  sysTimes.year = 2010 + i;
  
  sysTimes.weeks = ((data+4)%7)+1; //���ڼ�,20101��1��Ϊ����5
  
  i = yearday[i] - data;//���£������һ��
  if (0 == (sysTimes.year%4))
  {
    for (k = 0 ;k>12;k++)
    {
      if (k == 1 )
      {
        j = month[k] + 1;//�������1
      }
      
      if (i <= j)
      {
        break;
      }
      i = i - j;
    }
  }
  else
  {
    for (k = 0 ;k>12;k++)
    {
      if (i <= month[k])
      {
        break;
      }
      i = i - month[k];
    }
  }
  sysTimes.month = k;
  sysTimes.data = i;
  
  return &sysTimes;
}
/*
��������time_1ms_client_reg
��������
����ֵ����
������ʱ�����ע�ắ���������ҽӸ��ͻ���ע��Ļص��������ڵ�����ڸ����ͻ���ִ�С�
ģ�����ͣ�ר��ģ��(AT91SAM7S)
*/
void count_to_sec_cb(void)
{
  if (count_ms >= COUNT_TO_SEC_NUM)
  {
    count_ms = 0;
    system_sec++;//ϵͳʱ���1��
    
  }
  count_ms++;
}

/*
��������time_1ms_client_reg
��������
����ֵ����
������ʱ�����ע�ắ���������ҽӸ��ͻ���ע��Ļص��������ڵ�����ڸ����ͻ���ִ�С�
ģ�����ͣ�ר��ģ��(AT91SAM7S)
*/
void time_client_reg(TIME_REG_STRUCT * client)
{
  TIME_REG_STRUCT * list;
  
  list = &reg_header;
  //�ҽ�
  while (list->next != 0)
  {
    if (client == list) //����ͬ�ľͲ�����
    {
      return ;
    }
    list = list->next;
  }
  
  list->next = client;
  client->prev = list;
  client->next = 0;//�б�β������Ϊ0
}

/*
��������time_1ms_client_reg
��������
����ֵ����
������ʱ�����ע�ắ���������ҽӸ��ͻ���ע��Ļص��������ڵ�����ڸ����ͻ���ִ�С�
ģ�����ͣ�ר��ģ��(AT91SAM7S)
*/
void time_1ms_client_reg( void * client)
{
  TIME_REG_STRUCT * list;
  
  list = &reg_header;
  //�ҽ�
  while (list->next != 0)
  {
    list = list->next;
  }
  
  list->next = client;
  
  //client->next = 0;//�б�β������Ϊ0
}

//*------------------------- Internal Function --------------------------------
//*----------------------------------------------------------------------------
//* Function Name       : AT91F_TC_Open
//* Object              : Initialize Timer Counter Channel and enable is clock
//* Input Parameters    : <tc_pt> = TC Channel Descriptor Pointer
//*                       <mode> = Timer Counter Mode
//*                     : <TimerId> = Timer peripheral ID definitions
//* Output Parameters   : None
//*----------------------------------------------------------------------------
void AT91F_TC_Open ( AT91PS_TC TC_pt, unsigned int Mode, unsigned int TimerId)
//* Begin
{
  unsigned int dummy;
  
  //* First, enable the clock of the TIMER
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1<< TimerId ) ;
  
  //* Disable the clock and the interrupts
  TC_pt->TC_CCR = AT91C_TC_CLKDIS ;
  TC_pt->TC_IDR = 0xFFFFFFFF ;
  
  //* Clear status bit
  dummy = TC_pt->TC_SR;
  //* Suppress warning variable "dummy" was set but never used
  dummy = dummy;
  //* Set the Mode of the Timer Counter
  TC_pt->TC_CMR = Mode | (0x06<<13);//ADD FOR 1MS
  TC_pt->TC_RC = 22222;//ADD FOR 1MS
  //* Enable the clock
  TC_pt->TC_CCR = AT91C_TC_CLKEN ;
  //* End
}

//*------------------------- Interrupt Function -------------------------------

//*----------------------------------------------------------------------------
//* Function Name       : timer0_c_irq_handler
//* Object              : C handler interrupt function called by the interrupts
//*                       assembling routine
//* Output Parameters   : increment count_timer0_interrupt
//*----------------------------------------------------------------------------
void timer0_c_irq_handler(void)
{
   
  TIME_REG_STRUCT * list = &reg_header;
  AT91PS_TC TC_pt = AT91C_BASE_TC0;
  unsigned int dummy;
  //* Acknowledge interrupt status
  dummy = TC_pt->TC_SR;
  //* Suppress warning variable "dummy" was set but never used
  dummy = dummy;
  
  DR_twc_1ms_cb();
  
  while (list != 0)
  {
    list->timeout--;
    if (list->timeout < 1)
    {
      if (list->cb != 0)
      {
        //���ͷŵ�
        list->prev->next = list->next;
        list->next->prev = list->prev;
        list->cb();
      }
    }
    list = list->next;
  }
}

//*-------------------------- External Function -------------------------------

//*----------------------------------------------------------------------------
//* Function Name       : timer_init
//* Object              : Init timer counter
//* Input Parameters    : none
//* Output Parameters   : TRUE
//*----------------------------------------------------------------------------
void timer_init(void)
//* Begin
{
  //init the timer interrupt counter
  //count_timer0_interrupt=0;
  
  //* Open timer0
  AT91F_TC_Open(AT91C_BASE_TC0,TC_CLKS_MCK2,AT91C_ID_TC0);
  
  //* Open Timer 0 interrupt
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_TC0, TIMER0_INTERRUPT_LEVEL,AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, (void(*)())timer0_c_irq_handler);
  AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;  //  IRQ enable CPC
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_TC0);
  
  //* Start timer0
  AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG ;
  
  reg_header.cb = count_to_sec_cb;
  

}


//��ʼ����ʱ��1


//������ʱ��1����
void timer1_start_count(void)
{
  unsigned int dummy;
  
  //* First, enable the clock of the TIMER
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1<< AT91C_ID_TC1 ) ;
  
  //* Disable the clock and the interrupts
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKDIS ;
  AT91C_BASE_TC1->TC_IDR = 0xFFFFFFFF ;
  
  //* Clear status bit
  dummy = AT91C_BASE_TC1->TC_SR;
  //* Suppress warning variable "dummy" was set but never used
  dummy = dummy;
  //* Set the Mode of the Timer Counter
  AT91C_BASE_TC1->TC_CMR = TC_CLKS_MCK128 ;//����64MS
  
  //* Enable the clock
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN ;
  
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG ;
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG ;
}

unsigned int timer1_get_count(void)  //��0��ʼ
{
   return AT91C_BASE_TC1->TC_CV;
}

void timer1_c_irq_handler(void)
{
  
  AT91PS_TC TC_pt = AT91C_BASE_TC1;
  unsigned int dummy;
  //* Acknowledge interrupt status
  dummy = TC_pt->TC_SR;
  //* Suppress warning variable "dummy" was set but never used
  dummy = dummy;
  
  timer1_500us_isr_cb();
  
}
//������ʱ��1MS��ʱ��ע��ص�������
void timer1_start_isr(void (*func)(void))
{
    //* Open timer1
  AT91F_TC_Open(AT91C_BASE_TC1,TC_CLKS_MCK2,AT91C_ID_TC1);
  
  //* Open Timer 1 interrupt
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_TC1, TIMER1_INTERRUPT_LEVEL,AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, (void(*)())timer1_c_irq_handler);
  AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS;  //  IRQ enable CPC
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_TC1);
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG ;
  timer1_500us_isr_cb = func;
}


void timer1_stop_isr(void)
{
  AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKDIS ;
}