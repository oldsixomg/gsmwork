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

#define GLOBE_MIN_BUZZ        (1<<23)
#define GLOBE_MAX_BUZZ       (1<<23)
#define GLOBE_BUZZ_MUTE 0
#define GLOBE_BUZZ_RING 1
#define GLOBE_SET_GPIO_OUT(x)    AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_HIGH(x)   AT91F_PIO_SetOutput(AT91C_BASE_PIOA,x)
#define GLOBE_SET_GPIO_LOW(x)   AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,x)
#define GLOBE_CHECK_GPIO()        AT91F_PIO_GetOutputDataStatus(AT91C_BASE_PIOA)