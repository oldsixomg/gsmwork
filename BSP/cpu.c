/* ----------------------------------------------------------------------------
*         vteng 公司提供支持 
* ----------------------------------------------------------------------------
* 版权：2010 VTENG公司
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
#define  BSP_RAM_REMAP_TEST_BYTE             (*(INT8U  *)0x00000030L)

//头文件
#include "bsp.h"

//变量引用
//变量定义
//函数引用
//函数定义
/*******************************************************************************************************
*                                              CPU_Init
*说明：初始化FLASH等待状态号并设置片上PLL
********************************************************************************************************/
void CPU_Init(void) {
  AT91PS_PMC     pPMC = AT91C_BASE_PMC;
  
  AT91C_BASE_MC->MC_FMR = ((AT91C_MC_FMCN)&(50 <<16)) | AT91C_MC_FWS_1FWS ;    // 设置Flash等待状态
  // 单周期访问，30 MHz或 40 MHz
  // 若 MCK = 47923200，则50个周期为 1 uS
  AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS;                                // 禁止看门狗
  
  pPMC->PMC_MOR = (( AT91C_CKGR_OSCOUNT & (0x06 <<8) | AT91C_CKGR_MOSCEN ));   // 设置 MCK at 47 923 200
  // 允许主振荡器
  // SCK = 1/32768 = 30.51 uS
  while(!(pPMC->PMC_SR & AT91C_PMC_MOSCS)) {                                   // 等待启动时间
    ;
  }
  
  // 检查主振荡器频率(可选)
  // 设置PLL及除法器
  // - 除以 5： Fin = 3,6864 =(18,432 / 5)
  // - 乘 25+1: Fout = 95,8464 =(3,6864 *26)
  // 对于 96 MHz晶振，误差为 0.16%
  // 为使用的域 = 0
  // PLLCOUNT： PLL建立时间约为 0.844 ms
  // PLLCOUNT： 28 = 0.000844 /(1/32768)
  pPMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x05) | (AT91C_CKGR_PLLCOUNT & (28<<8)) | (AT91C_CKGR_MUL & (25<<16)));
  while(!(pPMC->PMC_SR & AT91C_PMC_LOCK))  {                                   // 等待启动时间
    ;
  }
  pPMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_2 ;     	        // 选择主时钟和处理器时钟
  
  /*
  
 *AT91C_PMC_SCER = (1<<0)   //Processor Clock Enable
                  |(1<<7);  //USB Device Port Clock Enable
                  
 
 *AT91C_CKGR_PLLR = (0x0E<<0)  //DIV
                   |(0x3F<<8)  //PLL COUNT
                   |(0x00<<14)  //OUT
                   |(0x48<<16)  //MUL
                   |(0x01<<28); //USB DIV
  */
  
}

 
 
 