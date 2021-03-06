/*
 * @Author Shi Zhangkun
 * @Date 2019-11-17 14:22:42
 * @LastEditTime 2019-11-20 12:57:14
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Inc\wifi.h
 */
#ifndef __WIFI_H
#define __WIFI_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "M8266WIFIDrv.h"
/* Type defines --------------------------------------------------------------*/
typedef enum{
  WIFI_OK,
  WIFI_ERROR,
  WIFI_IDLE,
  WIFI_CONNECT,
  WIFI_SERVER_CONNECT
}WIFI_status_t;
/* Defines -------------------------------------------------------------------*/
#define WIFI_AP_NAME            "539"
#define WIFI_PASSPORT           "539539539"
#define TCP_SERVER_PORT         6000
#define TCP_SERVER_ADDR         "192.168.31.107"
#define TCP_LINK_NUMBER_USED    1

/* Function declaration ------------------------------------------------------*/
WIFI_status_t WIFI_init(void);
void WIFI_sendPakageData(uint8_t* addr,uint32_t size);
HAL_StatusTypeDef WIFI_reciveCmd(uint8_t* addr,uint8_t cmdLength);
void WIFI_determineConnect(void);
#ifdef __cplusplus
}
#endif
#endif
