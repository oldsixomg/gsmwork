/*
*********************************************************************************************************
*                                            Atmel AT91 SAM7
*                                         Board Support Package
*
*                              (c) Copyright 2004, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*
* File : BSP.H
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

#ifndef BSP_H
#define BSP_H

#include "includes.h"
#include "AT91SAM7S64.h"
#include "AT91SAM7S_Time.h"
#include "AT91SAM7S-EK.h"
#include "cpu.h"
#include "lib_AT91SAM7S64.h"
#include "Spi.h"

/*
*********************************************************************************************************
*                                                 CONSTANTS
*********************************************************************************************************
*/


#ifdef FEATURE_ED_APP
#define MOTO_CONTROL                         (1<<0)	/* PA0  */
#define CC110X_CS                            ((unsigned int)(1<<11))   /* PA11*/

#define SPI_SELECT_CC110X                    (0x01<<15)
#define SPI_SELECT_W25X16                    (0x01<<16)

#define SPI_CC110X_CSR                       0
#define AT91_GPIO_PCK_EN                     (1<<0)
#define CC110X_GD0                           (1<<27)//????????????,???
#define CC110X_GD2                           (1<<28)//????????????,???



#ifdef FEATURE_ED_KEY
#define ALARM_KEY                               (1<<29)//????????????,???
#endif



#define AT91_BAUD_RATE          115200
#define USART_INTERRUPT_LEVEL   7
#define USB_INTERRUPT_LEVEL   6
#endif

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_Init(void);
void  BSP_IntDisAll(void);

extern void bsp_rs232_send(int character);
#endif //BSP_H
