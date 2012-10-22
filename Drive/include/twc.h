/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/

//宏定义
#define GLOBE_TWC_RX  (1<<3)

#define GLOBE_GET_KEY()         AT91F_PIO_GetInput(AT91C_BASE_PIOA)
#define GLOBE_SET_GPIO_IN(x)    AT91F_PIO_CfgInput(AT91C_BASE_PIOA,x)
#define GLOBE_GPIO_INT_EN(x)    AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,x)
#define GLOBE_GPIO_INT_DIS(x)   AT91F_PIO_InterruptDisable(AT91C_BASE_PIOA,x)
#define GLOBE_GET_INT_STATE()   AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA)
#define OS_REPORT_TWC_MSG(x,y)    phe_put_cmd(x,y);


void DR_twc_count_stop(void);
void DR_twc_start_recv(void);
void DR_twc_isr_cb();
void DR_twc_exit();
void DR_twc_time_isr_stop(void);
void DR_twc_result_report(unsigned int * num);