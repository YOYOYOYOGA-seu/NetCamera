/*
 * @Author Shi Zhangkun
 * @Date 2019-11-20 10:24:24
 * @LastEditTime  2020-02-01 13:24:09
 * @LastEditors   Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrApp\Src\cmdSys.c
 */

/* Includes -------------------------------------------------------------*/
#include "cmdSys.h"
#include "cmsis_os.h"
#include "wifi.h"
#include <string.h>
/**
 * @brief  
 * @note  
 * @param {type} none
 * @retval none
 */
uint8_t CMD_Analysis(uint8_t *cmd)
{
  uint8_t cmdIndex = 0;
  uint16_t status;
  for (cmdIndex = 1; cmdIndex <= CMD_NUM; cmdIndex++)
  {
    if (!strcmp((const char *)cmd, (const char *)CMD_DEFINE[cmdIndex]))
    {
      return cmdIndex;
    }
  }
  return CMD_IDLE_INDEX;
}
