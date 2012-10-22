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


#define TS_Q_EFS_CMD         0x0001

typedef enum
{
  NV_C_COUNT_I = 0,
  NV_D_COUNT_I,
  NV_C_ADD_S_I = 2,
  NV_C_ADD_E_I = 7,
  NV_D_ADD_S_I = 8,
  NV_D_ADD_E_I = 28,
  NV_N_ADD_S_I = 32,
  NV_N_ADD_E_I = 48,
}NV_ITEM_I;





extern void TS_efs_task(void *p_arg);