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
#define GLOBE_RST_KEY (1<<29)
#define GLOBE_SET_KEY (1<<30)

#define GLOBE_SET_GPIO_IN(x)    AT91F_PIO_CfgInput(AT91C_BASE_PIOA,x)
#define GLOBE_GPIO_INT_EN(x)    AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,x)
#define GLOBE_GET_KEY()         AT91F_PIO_GetInput(AT91C_BASE_PIOA)
#define OS_REPORT_KEY_EVENT(x)  phe_put_cmd(x, 0);



/*
函数名：DR_key_init
参数：无
返回值：无
描述：按键初始化
*/
void DR_key_init();


/*
函数名：DR_key_isr_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_key_isr_cb();

/*
函数名：DR_key_isr_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
unsigned int DR_key_input_check(unsigned int num);

/*
函数名：DR_set_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_set_cb(void);

/*
函数名：DR_rst_cb
参数：无
返回值：无
描述：按键中断处理函数
*/
void DR_rst_cb(void);