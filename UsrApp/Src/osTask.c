/*
 * @Author Shi Zhangkun
 * @Date 2019-11-01 21:16:41
 * @LastEditTime 2019-11-13 00:49:17
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrApp\Src\osTask.c
 */
/* Includes -------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "ff.h"
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
/* Handler extern ------------------------------------------------------*/

extern osMessageQueueId_t photoSaveQueueHandle;
extern osEventFlagsId_t keyEvent;
extern osEventFlagsId_t usbSendEvent;
FATFS tfCardFS;
/* Flag variable ---------------------------------------------------------*/
uint8_t dataTransPath = 0;   //0:Usb     1:wifi
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
  uint32_t jpegMessage[2] = {0,0};
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
          jpegMessage[jpegHeadNum] = 0;
          for(jpegMessage[jpegSize] = 0; jpegMessage[jpegSize]<(imageReadMemIndex<2?IMAGE_STREAM_MEM_SIZE:IMAGE_PHOTO_MEM_SIZE);jpegMessage[jpegSize]++)
          {
            
            if(pImageStmMem[imageReadMemIndex][jpegMessage[jpegSize]] == 0xFF&&pImageStmMem[imageReadMemIndex][jpegMessage[jpegSize] + 1] == 0xD8&&jpegHeadOk == 0)
            {
              jpegMessage[jpegHeadNum] = jpegMessage[jpegSize];
              jpegHeadOk = 1;
            }
            else if(pImageStmMem[imageReadMemIndex][jpegMessage[jpegSize]] == 0xFF&&pImageStmMem[imageReadMemIndex][jpegMessage[jpegSize] + 1] == 0xD9&&jpegHeadOk == 1)
            {
              jpegMessage[jpegSize] = jpegMessage[jpegSize] - jpegMessage[jpegHeadNum] + 2;
              jpegHeadOk = 2;
              break;
            }
          
          }
          if(jpegHeadOk == 2&&jpegMessage[jpegSize] < 65535)
          {
            CDC_Transmit_FS((uint8_t*)(&pImageStmMem[imageReadMemIndex][jpegMessage[jpegHeadNum]]),jpegMessage[jpegSize]);
          }
          if(CDC_Transmit_FS((uint8_t*)enter,2)!=USBD_OK)
          {
            osDelay(5);
          }
          imageStmMemStatus[imageReadMemIndex] = 0;  
          if(imageReadMemIndex == 2&&jpegHeadOk == 2)
          {
            osMessageQueuePut(photoSaveQueueHandle,jpegMessage,0,0);
          }
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
 * @brief  TFCard operate task
 * @note  task: Save photo to SD card
 * @param {type} none
 * @retval none
 */
void tskTFCard(void *argument)
{
  uint8_t pohotoFileID = 0;
  char photoFileName[] = "PHOTO/CAM000.jpg";
  FIL file;
  FRESULT resPhotoFolder;   //Signed if photo save folder prepared
  //const char testChar[] = "test Dma R/W\r\n";
  UINT len;
  uint32_t photoMessage[2];

  resPhotoFolder = f_mount(&tfCardFS,"0:",1);
  //determine if photo folder exist
  if(resPhotoFolder==FR_OK&&f_stat("PHOTO",NULL)==FR_NO_FILE)
  {
    //if SD card mount successful and photo folder not exist, create it
    resPhotoFolder = f_mkdir("PHOTO");
  }
  for(;;)
  {
    osMessageQueueGet(photoSaveQueueHandle,photoMessage,NULL,portMAX_DELAY);
    if(resPhotoFolder==FR_OK) //if file system is OK
    {
      //scan exist photo file for naming current photo
      for(;pohotoFileID < 255;pohotoFileID++)
      {
        sprintf(photoFileName,"PHOTO/CAM%03d.jpg",pohotoFileID);
        //If this photoID haven't been used
        if(f_stat(photoFileName,NULL)==FR_NO_FILE)
        {
          if(f_open(&file,photoFileName,FA_CREATE_NEW|FA_WRITE)==FR_OK)
          {
            f_write(&file,&pImageStmMem[2][photoMessage[jpegHeadNum]],photoMessage[jpegSize],&len);
          }
          break;
        }
      }
      osDelay(30);
      f_close(&file);
    }
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

