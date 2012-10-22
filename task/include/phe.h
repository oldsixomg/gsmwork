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
#include "bsp.h"


#define TS_Q_PHE_CMD         0x0001


extern OS_FLAG_GRP  *ts_phe_cmd_event;
/*
函数名：TS_phe_task
参数：无
返回值：无
描述：
*/
void TS_phe_task(void *p_arg);


void TS_phe_led_ind(unsigned int color,unsigned int state);