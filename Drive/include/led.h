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
#define GLOBE_LED_RED (1<<27)
#define GLOBE_LED_YELLOW (1<<28)
#define GLOBE_SET_GPIO_OUT(x)     AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_LOW(x)     AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_HIGH(x)    AT91F_PIO_SetOutput(AT91C_BASE_PIOA,x)
#define GLOBE_GET_LED()          AT91F_PIO_GetOutputDataStatus(AT91C_BASE_PIOA)

#define GLOBE_LED_ON     0
#define GLOBE_LED_OFF    1
#define GLOBE_LED_BLINK  2
#define GLOBE_LED_QBLINK  3


void DR_led_set(unsigned int color,unsigned int state);

void DR_led_init(void);

void DR_yled_cb(void);
void DR_rled_cb(void);

unsigned char  DR_led_check(unsigned int color);