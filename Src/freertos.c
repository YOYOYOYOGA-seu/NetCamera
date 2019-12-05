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
#include "fatfs.h"    
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
osEventFlagsId_t  camOpEvent;
osEventFlagsId_t  imageSendEvent;
osEventFlagsId_t  cmdTransmitEvent;
/* USER CODE END Variables */
osThreadId_t StartTaskHandle;
osThreadId_t taskMainHandle;
osThreadId_t imageSendDataHandle;
osThreadId_t KeyOpreationHandle;
osThreadId_t TFCardHandle;
osThreadId_t CameraHandle;
osThreadId_t cmdTransmitHandle;
osThreadId_t motionControlHandle;
osMessageQueueId_t photoSaveQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void hellowSrting(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void tskMain(void *argument);
extern void tskImageSendData(void *argument);
extern void tskKeyOpreation(void *argument);
extern void tskTFCard(void *argument);
extern void tskCamera(void *argument);
extern void tskCmdTransmit(void *argument);
extern void tskMotionControl(void *argument);

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
  camOpEvent = osEventFlagsNew(NULL);
  imageSendEvent = osEventFlagsNew(NULL);
  cmdTransmitEvent = osEventFlagsNew(NULL);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of photoSaveQueue */
  const osMessageQueueAttr_t photoSaveQueue_attributes = {
    .name = "photoSaveQueue"
  };
  photoSaveQueueHandle = osMessageQueueNew (6, sizeof(long long), &photoSaveQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartTask */
  const osThreadAttr_t StartTask_attributes = {
    .name = "StartTask",
    .priority = (osPriority_t) osPriorityRealtime7,
    .stack_size = 512
  };
  StartTaskHandle = osThreadNew(StartDefaultTask, NULL, &StartTask_attributes);

  /* definition and creation of taskMain */
  const osThreadAttr_t taskMain_attributes = {
    .name = "taskMain",
    .priority = (osPriority_t) osPriorityNormal1,
    .stack_size = 256
  };
  taskMainHandle = osThreadNew(tskMain, NULL, &taskMain_attributes);

  /* definition and creation of imageSendData */
  const osThreadAttr_t imageSendData_attributes = {
    .name = "imageSendData",
    .priority = (osPriority_t) osPriorityNormal2,
    .stack_size = 512
  };
  imageSendDataHandle = osThreadNew(tskImageSendData, NULL, &imageSendData_attributes);

  /* definition and creation of KeyOpreation */
  const osThreadAttr_t KeyOpreation_attributes = {
    .name = "KeyOpreation",
    .priority = (osPriority_t) osPriorityRealtime,
    .stack_size = 128
  };
  KeyOpreationHandle = osThreadNew(tskKeyOpreation, NULL, &KeyOpreation_attributes);

  /* definition and creation of TFCard */
  const osThreadAttr_t TFCard_attributes = {
    .name = "TFCard",
    .priority = (osPriority_t) osPriorityHigh,
    .stack_size = 1024
  };
  TFCardHandle = osThreadNew(tskTFCard, NULL, &TFCard_attributes);

  /* definition and creation of Camera */
  const osThreadAttr_t Camera_attributes = {
    .name = "Camera",
    .priority = (osPriority_t) osPriorityNormal4,
    .stack_size = 128
  };
  CameraHandle = osThreadNew(tskCamera, NULL, &Camera_attributes);

  /* definition and creation of cmdTransmit */
  const osThreadAttr_t cmdTransmit_attributes = {
    .name = "cmdTransmit",
    .priority = (osPriority_t) osPriorityHigh1,
    .stack_size = 512
  };
  cmdTransmitHandle = osThreadNew(tskCmdTransmit, NULL, &cmdTransmit_attributes);

  /* definition and creation of motionControl */
  const osThreadAttr_t motionControl_attributes = {
    .name = "motionControl",
    .priority = (osPriority_t) osPriorityRealtime6,
    .stack_size = 512
  };
  motionControlHandle = osThreadNew(tskMotionControl, NULL, &motionControl_attributes);

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
  delayMs(500);
  hellowSrting();
  psarmInit();
  MX_FATFS_Init();
  ov2640_Init();
  WIFI_init();
  //osThreadTerminate(StartTaskHandle);
  /* Infinite loop */
  for(;;)
  {
    //test ov5640 and psram
		osDelay(portMAX_DELAY);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void hellowSrting(void)
{
  static char hellow[] = {
  "\n------------------------------\n \n\
          NetCamera        \n \n\
------------------------------\n \
Version:0.1\n \
Designer: szk\n \n\0\
  "
  };
  CDC_Transmit_FS((uint8_t*)&hellow,130);
  delayMs(5);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
