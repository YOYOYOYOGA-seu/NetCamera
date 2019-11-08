/*
 * @Author Shi Zhangkun
 * @Date 2019-11-01 21:16:41
 * @LastEditTime 2019-11-09 01:58:30
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrApp\Src\osTask.c
 */
/* Includes -------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "osTask.h"
#include "main.h"

#include "usbd_cdc_if.h"
#include "UsrHal.h"
#include "dcmi.h"
#include "sdmmc.h"
/* Extern variable ------------------------------------------------------*/
extern const uint8_t* pImageStmMem[3];
extern uint8_t imageStmMemStatus[3];
extern ovOutMode_t CameraMode;

extern SD_HandleTypeDef hsd2;
/* Event handler extern ------------------------------------------------------*/
extern osEventFlagsId_t keyEvent;
extern osEventFlagsId_t usbSendEvent;
/* Task Handler ---------------------------------------------------------*/

/* Function declaration -------------------------------------------------*/

/**
 * @brief  Main task
 * @note  
 * @param {type} none
 * @retval none
 */
void tskMain(void *argument)
{
  for(;;)
  {
    CDC_Transmit_FS((uint8_t*)&(hsd2.SdCard),36);
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    osDelay(500);
  }
}

/**
 * @brief  Usb CDC sent data task
 * @note  
 * @param {type} none
 * @retval none
 */
void tskUsbSendData(void *argument)
{
  uint32_t usbSendEventBit;
	uint8_t enter[2] = "\n\n";
  uint8_t imageReadMemIndex = 0;
  uint32_t jpegHeadIndex = 0;
  uint32_t jpegSize = 0;
  uint8_t jpegHeadOk = 0;
  for(;;)
  {
    usbSendEventBit = osEventFlagsWait(usbSendEvent,USB_SEND_ALL_EVENT_BIT,osFlagsWaitAny,portMAX_DELAY);
    if(usbSendEventBit&USB_SEND_IMAGE_EVENT_BIT)
    {
      for(imageReadMemIndex = 0;imageReadMemIndex<3;imageReadMemIndex++)
      {
        if(imageStmMemStatus[imageReadMemIndex])
        {
          jpegHeadOk = 0;
          jpegHeadIndex = 0;
          for(jpegSize = 0; jpegSize<(imageReadMemIndex<2?IMAGE_STREAM_MEM_SIZE:IMAGE_PHOTO_MEM_SIZE);jpegSize++)
          {
            
            if(pImageStmMem[imageReadMemIndex][jpegSize] == 0xFF&&pImageStmMem[imageReadMemIndex][jpegSize + 1] == 0xD8&&jpegHeadOk == 0)
            {
              jpegHeadIndex = jpegSize;
              jpegHeadOk = 1;
            }
            else if(pImageStmMem[imageReadMemIndex][jpegSize] == 0xFF&&pImageStmMem[imageReadMemIndex][jpegSize + 1] == 0xD9&&jpegHeadOk == 1)
            {
              jpegSize = jpegSize - jpegHeadIndex + 2;
              jpegHeadOk = 2;
              break;
            }
          
          }
          if(jpegHeadOk == 2&&jpegSize < 65535)
          {
              CDC_Transmit_FS((uint8_t*)(&pImageStmMem[imageReadMemIndex][jpegHeadIndex]),jpegSize);
          }
          if(CDC_Transmit_FS((uint8_t*)enter,2)!=USBD_OK)
          {
            osDelay(5);
          }
          imageStmMemStatus[imageReadMemIndex] = 0;  
        }
      }
    }
    osDelay(10);
  }
  
}

/**
 * @brief  Scan the key action
 * @note  50Hz sampling freqency
 * @param {type} none
 * @retval none
 */
void tskKeyOpreation(void *argument)
{
  uint8_t lastKeyStatus[5] ;
  uint8_t keyStatus[5] = {0,0,0,0,0};
  uint8_t i;
  for(;;)
  {
    for(i = 0;i<5 ;i++)
    {
      lastKeyStatus[i] = keyStatus[i];
    }
    keyStatus[0] = HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin);
    keyStatus[1] = HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
    keyStatus[2] = HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
    keyStatus[3] = HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin);
    keyStatus[4] = HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin);
    for(i = 0;i<5 ;i++)
    {
      if(lastKeyStatus[i] == GPIO_PIN_SET && keyStatus[i] == GPIO_PIN_RESET)
      {
        osEventFlagsSet(keyEvent,1<<i);
      }
    }
    osDelay(20);
  }
}

/**
 * @brief  
 * @note  
 * @param {type} none
 * @retval none
 */
void tskLcd(void *argument)
{
  for(;;)
  {
    
    osDelay(1000);
  }
}

/**
 * @brief  Camera about task
 * @note  Handle the camera control task
 * @param {type} none
 * @retval none
 */
void tskCamera(void *argument)
{
  uint32_t keyEventBit;
 // DCMI_Start();  //Open camera defautly 
  for(;;)
  {
    keyEventBit = osEventFlagsWait(keyEvent,KEY_ALL_EVENT_BIT,osFlagsWaitAny,portMAX_DELAY);
    //start Camera
    if(keyEventBit&KEY_LEFT_EVENT_BIT)
    {
      if(!ovPowerStatus)     //if camera haven't opened, init it
      {
        osKernelLock();  //warning :Will case influence to realtime task
        ov2640_Init();
        osKernelUnlock();
      }
      osDelay(200);
      CameraMode = JPEG_STREAM;
      ovOutSizeSet(OV_JPEG_STREAM_WIDTH,OV_JPEG_STREAM_HEIGH);
      if(DCMI_Start()!=HAL_OK)
        OV_PWDN(1);
    }
    //Close camera
    if(keyEventBit&KEY_UP_EVENT_BIT)//stop Camera
    {
      DCMI_Stop();
      ov2640_DeInit();
    }
    //Take photo
    if(keyEventBit&KEY_PRESS_EVENT_BIT)
    {
      if(ovPowerStatus)         //if camera not open, can't take photo
      {
        DCMI_Stop();
        ovOutSizeSet(OV_JPEG_PHOTO_WIDTH,OV_JPEG_PHOTO_HEIGH);
        CameraMode = JPEG_PHOTO;
        DCMI_Start();
        osDelay(100);
      }
    }
    
  }
}

