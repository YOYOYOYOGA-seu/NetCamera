/*
 * @Author Shi Zhangkun
 * @Date 2019-11-17 14:22:33
 * @LastEditTime 2019-11-20 13:08:41
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\wifi.c
 */


/* Includes ------------------------------------------------------------------*/
#include "wifi.h"
#include "delay.h"
#include "main.h"
#include <stdlib.h>
/* Extern varaible -----------------------------------------------------------*/
extern SPI_HandleTypeDef hspi4;

WIFI_status_t wifiStatus = WIFI_ERROR;

/**
 * @brief  Hardware reset ALK8266 module
 * @note  
 * @param {type} none
 * @retval none
 */
void WIFI_hardwareReset(void)
{
  M8266HostIf_Set_SPI_nCS_Pin(0);
  delayMs(2);
  M8266HostIf_Set_nRESET_Pin(0);
  delayMs(5);
  M8266HostIf_Set_nRESET_Pin(1);
  delayMs(300);
  M8266HostIf_Set_SPI_nCS_Pin(1);
  delayMs(500);
}

/**
 * @brief  init ALK8266 module and set to STA mode
 * @note  API will wait 10s for wifi and TCP/UDP connect
 * @param {type} none
 * @retval none
 */
WIFI_status_t WIFI_init(void)
{
  u32  	spiCLK = 15000000;
  u8    testByte;
	u8   	sta_ap_mode = 0;
	u16  	status = 0;
  
  WIFI_hardwareReset();
  //spiCLK = SystemCoreClock/(4*(1<<((hspi4.Init.BaudRatePrescaler>>28)+1)));
  if(M8266HostIf_SPI_Select((uint32_t)hspi4.Instance, spiCLK, &status)==0)
  {
    strcpy(errorInform, "\nerrM8266SpiSelect\0");
    Error_Handler();
    wifiStatus = WIFI_ERROR;
    return wifiStatus;
 
  }
  if(M8266WIFI_SPI_Interface_Communication_OK(&testByte) == 0)
  {
    strcpy(errorInform, "\nerrM8266ComTest\0");
    Error_Handler();
    wifiStatus = WIFI_ERROR;
    return wifiStatus;
  }
  if(M8266WIFI_SPI_Set_Opmode(1,0,&status) == 0)
  {
    strcpy(errorInform, "\nerrM8266SetOpmode\0");
    Error_Handler();
    wifiStatus = WIFI_ERROR;
    return wifiStatus;
  }
  delayMs(2);
  if(M8266WIFI_SPI_Get_Opmode(&sta_ap_mode,&status) == 0)
  {
    strcpy(errorInform, "\nerrM8266GetOpmode\0");
    Error_Handler();
    wifiStatus = WIFI_ERROR;
    return wifiStatus;
  }
  if(sta_ap_mode != 1)
  {
    strcpy(errorInform, "\nerrM8266Can'tSet\0");
    Error_Handler();
    wifiStatus = WIFI_ERROR;
    return wifiStatus;
  }
  strcpy(errorInform, "\nConnectingToAp...\0");
  Error_Handler();
  if(M8266WIFI_SPI_STA_Connect_Ap(WIFI_AP_NAME,WIFI_PASSPORT,1,10,&status) == 0)
  {
    switch (status)
    {
    case 0x32:
      strcpy(errorInform, "\nerrM8266APCoTmOut\0");
      Error_Handler();
      break;
    case 0x4A:
      strcpy(errorInform, "\nerrM8266APNotFind\0");
      Error_Handler();
      break;
    case 0x4B:
      strcpy(errorInform, "\nerrM8266APKeyErr\0");
      Error_Handler();
      break;
    default:
      strcpy(errorInform, "\nerrM8266APCoErr\0");
      Error_Handler();
      break;
    }
    wifiStatus = WIFI_IDLE;
    return wifiStatus;
  }

  strcpy(errorInform, "\nokM8266APConnect\0");
  Error_Handler();
  delayMs(5);
  strcpy(errorInform, "\n\nConToTCPServ...\0");
  Error_Handler();
  if(M8266WIFI_SPI_Setup_Connection(1,0,TCP_SERVER_ADDR,TCP_SERVER_PORT,TCP_LINK_NUMBER_USED,10,&status)==0)
  {
    strcpy(errorInform, "\nerrM8266ServCoErr\0");
    Error_Handler();
    wifiStatus = WIFI_CONNECT;
    return wifiStatus;
  }
  strcpy(errorInform, "\nokM8266ServConect\0");
  Error_Handler();
  wifiStatus = WIFI_SERVER_CONNECT;
  return wifiStatus;
}


/**
 * @brief  WIFI_sendPakageData,devide big pakage as some 1KB size TCP/UDP pakage
 * @note  
 * @param {type} none
 * @retval none
 */
void WIFI_sendPakageData(uint8_t* addr,uint32_t size)
{
  uint16_t status;
  uint8_t* currentAddr = addr;
  uint32_t remainingSize = size;
  if(wifiStatus == WIFI_SERVER_CONNECT)  //return when haven't set connection with TCP server
  {
    while(remainingSize>0)// TCP/UDP package Max size = 1000
    {
      M8266WIFI_SPI_Send_Data(currentAddr,(remainingSize<1000?remainingSize:1000),1,&status);
      if(remainingSize<1000)
      {
        remainingSize = 0;
      }
      else
      {
        remainingSize -= 1000;
        currentAddr += 1000;
      }
    }
  }
}
HAL_StatusTypeDef WIFI_reciveCmd(uint8_t* addr,uint8_t cmdLength)
{
  uint8_t i;
  uint8_t cmdRecvBuff[100];
  uint16_t status = 0xFF;
  uint16_t cmdRecvLen;
  uint8_t linkNo;
  if(M8266WIFI_SPI_Has_DataReceived())
  {
    cmdRecvLen = M8266WIFI_SPI_RecvData(cmdRecvBuff,100,1,&linkNo,&status);
    if(status == 0x00&&cmdRecvLen>=cmdLength)     //don't care TCP pakage is lager than cmdbuffSize
    {
      for(i=0;i<cmdRecvLen-(cmdLength-1);i++)
      {
        if(cmdRecvBuff[i]=='C'&&cmdRecvBuff[i+1]=='T')
        {
          
          memcpy(addr,&cmdRecvBuff[i],cmdLength);
          return HAL_OK;
        }
      }
    }
  }
  
  return HAL_ERROR;
}
/**
 * @brief  Determine if wifi and TCPserver connected,if not, connect it
 * @note  
 * @param {type} none
 * @retval none
 */
void WIFI_determineConnect(void)
{
  uint8_t tcpConStatus;
  uint8_t wifiConStatus;
  uint16_t status;
  if(M8266WIFI_SPI_Get_STA_Connection_Status(&wifiConStatus,&status))//if successfully get M8266 wifi status
  {
    switch (wifiConStatus)
    {
    case STA_AP_STATUS_CONNECTED: //wifi connect
      if(M8266WIFI_SPI_Query_Connection(TCP_LINK_NUMBER_USED,NULL,&tcpConStatus,NULL,NULL,NULL,&status))
      {
        if(tcpConStatus==ANYLINKIN_WIFI_LINK_CONNECTION_STATE_CONNECT||\
        tcpConStatus==ANYLINKIN_WIFI_LINK_CONNECTION_STATE_WRITE||\
        tcpConStatus==ANYLINKIN_WIFI_LINK_CONNECTION_STATE_READ)    //TCP server connect
        {
          
          if(wifiStatus!=WIFI_SERVER_CONNECT)
          {
            strcpy(errorInform, "\nokM8266ServConect\0");
            Error_Handler();
            wifiStatus=WIFI_SERVER_CONNECT; 
          }
        }
        else //TCP server disconnect
        {
          if(wifiStatus==WIFI_SERVER_CONNECT) //if last status TCP connect, show "TCP"
          {
            strcpy(errorInform, "\nerrM8266ServBreak\0");
            Error_Handler();
          }
          else if(wifiStatus!=WIFI_CONNECT)  //if last status wifi disconnect connect
          {
            strcpy(errorInform, "\nokM8266APConnect\0");
            Error_Handler();
          }
          
          wifiStatus=WIFI_CONNECT;  //wifi connect but TCP server haven't
          M8266WIFI_SPI_Setup_Connection(1,0,TCP_SERVER_ADDR,TCP_SERVER_PORT,TCP_LINK_NUMBER_USED,0,&status);  
        }
      }
      break;
    default://wifi haven't connect
      wifiStatus=WIFI_IDLE;   
      /*Because M8266 will connect to wifi automatically, so don't need to send connection Cmd*/
      break;
    }
  }
}
