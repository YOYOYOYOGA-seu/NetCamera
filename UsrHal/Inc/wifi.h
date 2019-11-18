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
#define WIFI_AP_NAME          "539"
#define WIFI_PASSPORT         "539539539"
#define TCP_SERVER_PORT       6000
#define TCP_SERVER_ADDR       "192.168.31.107"
#define TCP_LINK_NUMBER_USED  1
/* Function declaration ------------------------------------------------------*/
WIFI_status_t WIFI_init(void);
void WIFI_sendPakageData(uint8_t* addr,uint32_t size);
void WIFI_determineConnect(void);
#ifdef __cplusplus
}
#endif
#endif
