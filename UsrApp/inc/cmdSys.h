/*
 * @Author Shi Zhangkun
 * @Date 2019-11-20 10:24:32
 * @LastEditTime 2019-11-20 12:38:00
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrApp\inc\cmdSys.h
 */
#ifndef __CMDSYS_H
#define __CMDSYS_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
/* WIFI commond define ------------------------------------------------------*/
#define WIFI_CMD_LENGTH                 8
#define CMD_NUM                         (sizeof(CMD_DEFINE))/(sizeof(CMD_DEFINE[0]))        
#define CMD_IDLE                        "CT_IDLE_"
/* Type defines --------------------------------------------------------------*/
typedef enum{
  CMD_IDLE_INDEX,             
  CMD_START_STREAM_INDEX,    
  CMD_STOP_STREAM_INDEX,      
  CMD_PHOTO_INDEX   
}cmdIndex_t;
static const char CMD_DEFINE[][WIFI_CMD_LENGTH+1] = 
{
  CMD_IDLE,
  "CT_START",
  "CT_STOP_",
  "CT_PHOTO"
};

/* Function declaration ------------------------------------------------------*/
uint8_t CMD_Analysis(uint8_t* cmd);

#ifdef __cplusplus
}
#endif
#endif
