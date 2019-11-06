/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usbd_cdc_if.h"
#include "UsrHal.h"
#include "dcmi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osEventFlagsId_t  keyEvent;
osEventFlagsId_t usbSendEvent;
/* USER CODE END Variables */
osThreadId_t StartTaskHandle;
osThreadId_t taskMainHandle;
osThreadId_t UsbSendDataHandle;
osThreadId_t KeyOpreationHandle;
osThreadId_t LcdHandle;
osThreadId_t CameraHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void tskMain(void *argument);
extern void tskUsbSendData(void *argument);
extern void tskKeyOpreation(void *argument);
extern void tskLcd(void *argument);
extern void tskCamera(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  keyEvent = osEventFlagsNew(NULL);
  usbSendEvent = osEventFlagsNew(NULL);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartTask */
  const osThreadAttr_t StartTask_attributes = {
    .name = "StartTask",
    .priority = (osPriority_t) osPriorityRealtime7,
    .stack_size = 256
  };
  StartTaskHandle = osThreadNew(StartDefaultTask, NULL, &StartTask_attributes);

  /* definition and creation of taskMain */
  const osThreadAttr_t taskMain_attributes = {
    .name = "taskMain",
    .priority = (osPriority_t) osPriorityNormal1,
    .stack_size = 128
  };
  taskMainHandle = osThreadNew(tskMain, NULL, &taskMain_attributes);

  /* definition and creation of UsbSendData */
  const osThreadAttr_t UsbSendData_attributes = {
    .name = "UsbSendData",
    .priority = (osPriority_t) osPriorityNormal5,
    .stack_size = 512
  };
  UsbSendDataHandle = osThreadNew(tskUsbSendData, NULL, &UsbSendData_attributes);

  /* definition and creation of KeyOpreation */
  const osThreadAttr_t KeyOpreation_attributes = {
    .name = "KeyOpreation",
    .priority = (osPriority_t) osPriorityAboveNormal5,
    .stack_size = 128
  };
  KeyOpreationHandle = osThreadNew(tskKeyOpreation, NULL, &KeyOpreation_attributes);

  /* definition and creation of Lcd */
  const osThreadAttr_t Lcd_attributes = {
    .name = "Lcd",
    .priority = (osPriority_t) osPriorityNormal3,
    .stack_size = 512
  };
  LcdHandle = osThreadNew(tskLcd, NULL, &Lcd_attributes);

  /* definition and creation of Camera */
  const osThreadAttr_t Camera_attributes = {
    .name = "Camera",
    .priority = (osPriority_t) osPriorityLow7,
    .stack_size = 128
  };
  CameraHandle = osThreadNew(tskCamera, NULL, &Camera_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    
                 
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN StartDefaultTask */
  ov2640_Init();
  
  //osThreadTerminate(StartTaskHandle);
  /* Infinite loop */
  for(;;)
  {
    //test ov5640 and psram
		osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
