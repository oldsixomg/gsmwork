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
#define USART_INTERRUPT_LEVEL		7
#define AT91_BAUD_RATE			9600
#define UART_ID_SELECT                  AT91C_ID_US1
#define UART_BASE_SELECT                AT91C_BASE_US1
#define UART_RX_SELECT                  AT91C_PA21_RXD1
#define UART_TX_SELECT                  AT91C_PA22_TXD1
#define UART_QUEUE_BUFFER_SIZE          65//��һ�����������

//ͷ�ļ�
#include "bsp.h"
#include "drive.h"

//��������
//��������
static unsigned char queue_bufer[UART_QUEUE_BUFFER_SIZE];
static QUEUE_S uart_queue;
static unsigned char error_ptr[]="ERROR\n";

//��������
//��������
/*
��������sio_get_uart_data
������
����ֵ����
��������ȡ��������
*/
unsigned char sio_get_uart_data(unsigned char * data_ptr)
{
  int i = 0;
  unsigned char out_data;
  while (queue_pop_byte(&uart_queue , &out_data))
  {
    
    data_ptr[i] = out_data;
    i++;
    
  }
  return i;
  
}

/*
��������uart_send_byte
������
����ֵ����
���������ڵ�������һ���ֽڣ���ǰ���Ƿ��ܷ�
*/
void uart_send_byte(AT91PS_USART pUSART,int character)
{
  
  while (!(pUSART->US_CSR & AT91C_US_TXRDY));
  pUSART->US_THR = (character & 0x1FF);
  
  
}

/*
��������uart_send_error
������
����ֵ����
�����������ִ���ʱ����ERROR
*/
void uart_send_error(void)
{
  
  AT91PS_USART USART_pt = UART_BASE_SELECT;
  
  for (int i = 0;i < sizeof(error_ptr) ; i++)
  {
    uart_send_byte(USART_pt , error_ptr[i]);
    
  }
  
}
/*
��������sio_sent_uart_data
������
����ֵ����
��������ȡ��������
*/
void sio_sent_uart_data(unsigned char *data_ptr , unsigned short lenght)
{
  AT91PS_USART USART_pt = UART_BASE_SELECT;
  
  for (int i = 0;i < lenght ; i++)
  {
    uart_send_byte(USART_pt , data_ptr[i]);
    
  }
  
  
}
/*
��������uart_parse_cmd
������
����ֵ����
�������Ӵ��ڽ�����������ݣ�������������ݱ�����AT��ͷ->��������β����
����������ʱ�Ϳ����������濪ͷ�Ƿ���AT����Сд����ν�������ǾͰ�����������0
*/
void uart_parse_cmd(unsigned char input)
{
  unsigned char at[2];
#ifdef FEATURE_ED_SIO
  if (is_dm_mode())
  {
    dm_get_data(input);
    return;
  }
#endif
  if (0 == queue_push_byte( &uart_queue , input ))//������
  {
    uart_send_error();
    queue_reset(&uart_queue);//���˻�û�յ������
    return ;
  }
  if (0x1a == input)//������
  {
    queue_pop_byte( &uart_queue , at );
    queue_pop_byte( &uart_queue , at+1 );
    if ( ((at[0] == 'a') || (at[0] == 'A'))&&
        ((at[1] == 't') || (at[1] == 'T')))
    {
#ifdef FEATURE_ED_SIO
      OSFlagPost (sio_cmd_event, SIO_UART_EVENT,OS_FLAG_SET ,&err);
#endif
    }
    else//����û��ʶ������
    {
      uart_send_error();
      queue_reset(&uart_queue);
    }
  }
}

__ramfunc void irq_uart_handler(void)
{
  unsigned char status;
  AT91PS_USART USART_pt = UART_BASE_SELECT;
  
  //AT91F_US_DisableIt(USART_pt,AT91C_US_RXRDY);//
  
  status = USART_pt->US_CSR;
  if ( status & AT91C_US_RXRDY)
  {
    uart_parse_cmd(AT91F_US_GetChar(USART_pt));
  }
  
  //* Reset the satus bit
  // USART_pt->US_CR = AT91C_US_RSTSTA;
  //�߼��жϿ�������������жϱ�־���ͼ��Ķ�SR�ͽ��������
  //AT91F_AIC_ClearIt(AT91C_BASE_AIC,UART_ID_SELECT);
  //��ISR�����ǲ���P_SW�жϷ���
  //AT91F_US_RxReady(USART_pt);
  //�߼��ж����������ǡ�
  *AT91C_AIC_EOICR = 0x00;
  
  
}

//*-------------------------- External Function -------------------------------

//*----------------------------------------------------------------------------
//* Function Name       : Usart_init
//* Object              : USART initialization
//* Input Parameters    : none
//* Output Parameters   : TRUE
//*----------------------------------------------------------------------------
void uart_init ( void )
//* Begin
{
  AT91PS_USART COM = UART_BASE_SELECT;
  
  //* Configure PIO controllers to periph mode
  AT91F_PIO_CfgPeriph( AT91C_BASE_PIOA,
                      ((unsigned int) UART_RX_SELECT    ) |
                        ((unsigned int) UART_TX_SELECT    ) , // Peripheral A
                        0); // Peripheral B
  
  
  // First, enable the clock of the PIOB
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << UART_ID_SELECT ) ;
  // Usart Configure
  AT91F_US_Configure (COM, AT91B_MCK , AT91C_US_ASYNC_MODE, AT91_BAUD_RATE, 0);
  
  // Enable usart
  COM->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;
  
  //* Enable USART IT error and RXRDY
  AT91F_US_EnableIt(COM,AT91C_US_RXRDY);//
  
  //* open Usart 1 interrupt
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, UART_ID_SELECT, USART_INTERRUPT_LEVEL,AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, (void (*)())irq_uart_handler);
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, UART_ID_SELECT);
  //* End
}



