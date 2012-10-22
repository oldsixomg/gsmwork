/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *所有与时间相关操作和定义,针对AT91SAM7S64
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */


#ifndef AT91SMA7S_TIME_H
#define AT91SMA7S_TIME_H

//#include "global.h"



#define AT91B_MAIN_OSC        18432000               // Main Oscillator MAINCK
#define AT91B_MCK             ((AT91B_MAIN_OSC*73/14)/2)   // Output PLL Clock (48 MHz)


#define MCKKHz  (AT91B_MCK/1000)
#define SPEED 		(MCKKHz/10)

extern void wait_s ( int i );//通过CPU循环等待，时间单位1秒
extern void wait_ms ( int i );//通过CPU循环等待，时间单位1豪秒
extern void wait_us ( int i );//通过CPU循环等待，时间单位1us



#endif //AT91SMA7S_TIME_H