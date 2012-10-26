/* ----------------------------------------------------------------------------
* WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*此文件为串口驱动文件，主要实现串口的初始化，发送和接收操作。
*
*
* SIO层有stream_id来标识串口,USB等。
*
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件
#include "drive.h"
#include "os.h"
#include "sio.h"
#include "rs232.h"


//变量引用
//变量定义
static unsigned char dr_rs232_recv_buff[256];
static unsigned char dr_rs232_recv_len = 0;
TIME_REG_STRUCT dr_rs232_recv_time;

//函数引用
//函数定义


void DR_rs232_data_up(void)
{
    DSM_ITEM * item;
    DSM_ITEM * head;
    DSM_ITEM * list; 
    head = dsm_alloc_item();
    item = head;
    
    for (int i=0;i<dr_rs232_recv_len;i++)
    {
      if ((0 == i%32) && (0 != i))//碰到32的整数倍就重新申请一个ITEM
      {
        list = item;
        item= dsm_alloc_item();
        list->next = item; //串接起来了
        item->stream_id = SIO_RS232_STREAM_ID;
      }
      item->data_ptr[i%32] = dr_rs232_recv_buff[i];
      item->used++;
    }
    dr_rs232_recv_len = 0;
    OS_REPORT_RS232_MSG(RS232_UP_CMD,head);
}
/*
函数名：DR_rs232_recv_cb
参数：无
返回值：无
描述：串口接收中断处理函数,收到数据后直接发送给SIO任务
*/
void DR_rs232_recv_cb(unsigned char data) 
{
  
  if (dr_rs232_recv_len == 0) //从第一个收包，启动定时器
  {    //也可以从最后一个包算起
       dr_rs232_recv_time.timeout = 500;
       time_client_reg(&dr_rs232_recv_time);
  }
  
  dr_rs232_recv_buff[dr_rs232_recv_len] = data;
  dr_rs232_recv_len++;
  
  if (dr_rs232_recv_len>=256)
  {
      DR_rs232_data_up();
  }
}
void DR_rs232_time_cb(void)
{
    if (dr_rs232_recv_len > 0)
    {
      DR_rs232_data_up();
    }
}

void bsp_rs232_int_isr(void)
{
  unsigned char status;

  status = GLOBE_RS232_GET_STATE();
  if ( status & AT91C_US_RXRDY)
  {
      DR_rs232_recv_cb(GLOBE_RS232_GET_DATA());
  }
  GLOBE_RS232_CLEAR_STATE();
}

/*
函数名：DR_rs232_init
参数：无
返回值：无
描述：串口初始化
*/
void DR_rs232_init()
{
  
  dr_rs232_recv_time.cb = DR_rs232_time_cb;
  
  AT91PS_USART COM = AT91C_BASE_US0;
  AT91F_PIO_CfgPeriph( AT91C_BASE_PIOA,
                      ((unsigned int) AT91C_PA5_RXD0    ) |
                        ((unsigned int) AT91C_PA6_TXD0    ) , // Peripheral A
                        0); // Peripheral B
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_US0 ) ;
  AT91F_US_Configure (COM, AT91B_MCK , AT91C_US_ASYNC_MODE, AT91_BAUD_RATE, 0);
  // Enable usart
  COM->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;
  // Enable USART IT error and RXRDY
  AT91F_US_EnableIt(COM,AT91C_US_RXRDY);//
  // open Usart 1 interrupt
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_US0, USART_INTERRUPT_LEVEL,AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, (void (*)())bsp_rs232_int_isr);
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_US0);
  
  AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,(1<<20));
  AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,(1<<20));
  wait_ms(300);  //延迟1秒
  AT91F_PIO_SetOutput(AT91C_BASE_PIOA,(1<<20));
  wait_ms(300);  //延迟1秒
  
  AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,(1<<19));
  AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,(1<<19));
  wait_ms(1000);  //延迟1秒
  AT91F_PIO_SetOutput(AT91C_BASE_PIOA,(1<<19));
  wait_ms(300);  //延迟1秒
  
}
 




/*
函数名：DR_rs232_send
参数：无
返回值：无
描述：串口数据发送，由SIO层调用发送
*/
void DR_rs232_send(unsigned char * buff,unsigned int used)
{
   GLOBE_INT_DIS();
   for (int i = 0;i < used ; i++)
   {
       GLOBE_RS232_SEND_BYTE(buff[i]);
   }
   GLOBE_INT_EN();


}

