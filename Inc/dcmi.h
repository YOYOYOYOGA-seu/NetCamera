/*
 * @Author Shi Zhangkun
 * @Date 2019-11-03 19:47:56
 * @LastEditTime 2019-11-03 21:07:12
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\Inc\dcmi.h
 */
/**
  ******************************************************************************
  * File Name          : DCMI.h
  * Description        : This file provides code for the configuration
  *                      of the DCMI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dcmi_H
#define __dcmi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usrHal.h"
#define IMAGE_DATA_PATH             0         //0 sramIn   1psram
#if IMAGE_DATA_PATH == 0
  #define writeImageBuffToMem(a,len)  {uint16_t i;for(i=0;i<len;i++){*(uint16_t*)((uint32_t)pImageStmMem[imageStmMemIndex]+imageBuffIndex*2*len+2*i) = a[i];}}
#else
  #define writeImageBuffToMem(a,len)  psramWrite((uint8_t*)a,pImageStmWMem+imageBuffIndex*2*len,2*len)
#endif
#define DCMI_BUFF_SIZE          8*1024     //uint16 length

#define IMAGE_STREAM_MEM_SIZE   2*OV_RGB_IMGAE_WIDTH*OV_RGB_IMGAE_HEIGH




/* USER CODE END Includes */

extern DCMI_HandleTypeDef hdcmi;

/* USER CODE BEGIN Private defines */
extern const uint32_t pImageStmWMem;
extern const uint32_t pImageStmRMem;
/* USER CODE END Private defines */

void MX_DCMI_Init(void);

/* USER CODE BEGIN Prototypes */
void DCMI_Start(void);
void DCMI_Stop(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ dcmi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
