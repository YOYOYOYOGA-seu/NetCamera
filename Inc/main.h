/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern char errorInform[20];
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOE
#define TFT_LED_Pin GPIO_PIN_11
#define TFT_LED_GPIO_Port GPIOE
#define TFT_A0_Pin GPIO_PIN_13
#define TFT_A0_GPIO_Port GPIOE
#define TFT_RESET_Pin GPIO_PIN_15
#define TFT_RESET_GPIO_Port GPIOE
#define TFT_CS_Pin GPIO_PIN_11
#define TFT_CS_GPIO_Port GPIOB
#define WF_RESET_Pin GPIO_PIN_8
#define WF_RESET_GPIO_Port GPIOD
#define KEY0_Pin GPIO_PIN_9
#define KEY0_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_10
#define KEY1_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_14
#define KEY3_GPIO_Port GPIOD
#define KEY4_Pin GPIO_PIN_15
#define KEY4_GPIO_Port GPIOD
#define OV_PWDN_Pin GPIO_PIN_8
#define OV_PWDN_GPIO_Port GPIOB
#define OV_RESET_Pin GPIO_PIN_9
#define OV_RESET_GPIO_Port GPIOB
#define OV_SCL_Pin GPIO_PIN_0
#define OV_SCL_GPIO_Port GPIOE
#define OV_SDA_Pin GPIO_PIN_1
#define OV_SDA_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
