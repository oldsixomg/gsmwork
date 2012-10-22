/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *������ʱ����ز����Ͷ���,���AT91SAM7S64
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

extern void wait_s ( int i );//ͨ��CPUѭ���ȴ���ʱ�䵥λ1��
extern void wait_ms ( int i );//ͨ��CPUѭ���ȴ���ʱ�䵥λ1����
extern void wait_us ( int i );//ͨ��CPUѭ���ȴ���ʱ�䵥λ1us



#endif //AT91SMA7S_TIME_H