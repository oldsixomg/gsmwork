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


#define OS_REPORT_RS232_MSG(x,y)  sio_put_cmd(x,y);

#define GLOBE_INT_EN()  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA)
#define GLOBE_INT_DIS() AT91F_AIC_DisableIt(AT91C_BASE_AIC,AT91C_ID_PIOA)
#define GLOBE_RS232_GET_STATE()  (AT91C_BASE_US0->US_CSR)
#define GLOBE_RS232_GET_DATA()   (AT91F_US_GetChar(AT91C_BASE_US0))
#define GLOBE_RS232_CLEAR_STATE() *AT91C_AIC_EOICR = 0x00
#define GLOBE_RS232_SEND_BYTE(x) while (!(AT91C_BASE_US0->US_CSR & AT91C_US_TXRDY));\
                                  AT91C_BASE_US0->US_THR = (x & 0x1FF);