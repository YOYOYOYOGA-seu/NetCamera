/*
 * @Author Shi Zhangkun
 * @Date 2019-11-01 21:16:41
 * @LastEditTime 2019-11-04 21:38:44
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
/* Extern variable ------------------------------------------------------*/
extern const uint8_t* pImageStmMem[2];
extern uint8_t imageStmMemStatus[2];
extern ovOutMode_t CameraMode;

extern osSemaphoreId_t keyUpHandle;
extern osSemaphoreId_t keyDownHandle;
extern osSemaphoreId_t keyLeftHandle;
extern osSemaphoreId_t keyRightHandle;
extern osSemaphoreId_t keyPressHandle;
/* Task Handler ---------------------------------------------------------*/

/* Function declaration -------------------------------------------------*/


void tskMain(void *argument)
{
  for(;;)
  {
    
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    osDelay(500);
  }
}
void tskUsbSendData(void *argument)
{
  
  uint8_t cmdHead[2] = {0x01,0xFE};
  uint8_t cmdTail[2] = {0xFE,0x01};
	uint8_t enter[2] = "\n\n";
  uint8_t imageReadMemIndex = 0;
  uint32_t jpegHeadIndex = 0;
  uint32_t jpegSize = 0;
  uint8_t jpegHeadOk = 0;
  for(;;)
  {
    
    if(imageStmMemStatus[imageReadMemIndex])
    {
      switch (CameraMode)
      {
        case RGB_STREAM:
          CDC_Transmit_FS((uint8_t*)cmdHead,2);
          delayMs(1);
          psarmEnterMemoryMapped();
          #if	2*OV_RGB_IMGAE_WIDTH*OV_RGB_IMGAE_HEIGH < 65535
          CDC_Transmit_FS((uint8_t*)pImageStmMem[imageReadMemIndex],IMAGE_STREAM_MEM_SIZE);
          #else
          CDC_Transmit_FS((uint8_t*)pImageStmMem[imageReadMemIndex],IMAGE_STREAM_MEM_SIZE/2);
          osDelay(80);
          CDC_Transmit_FS((uint8_t*)(pImageStmMem[imageReadMemIndex]+IMAGE_STREAM_MEM_SIZE/2),IMAGE_STREAM_MEM_SIZE/2);
          #endif
          osDelay(80);
          CDC_Transmit_FS((uint8_t*)cmdTail,2);
          delayMs(1);
          imageStmMemStatus[imageReadMemIndex] = 0;
          break;
        case JPEG_STRAM:
        
          jpegHeadOk = 0;
          jpegHeadIndex = 0;
          for(jpegSize = 0; jpegSize<IMAGE_STREAM_MEM_SIZE ;jpegSize++)
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
          
          HAL_Delay(1);
          if(jpegHeadOk == 2&&jpegSize < 65535)
          {
              CDC_Transmit_FS((uint8_t*)(&pImageStmMem[imageReadMemIndex][jpegHeadIndex]),jpegSize);
          }
          if(CDC_Transmit_FS((uint8_t*)enter,2)!=USBD_OK)
          {
            osDelay(10);
          }
          imageStmMemStatus[imageReadMemIndex] = 0;
          break;
        case JPEG_PHOTO:
          /* code */
          break;
        default:
          break;
      }
    }
    imageReadMemIndex = !imageReadMemIndex;
		osDelay(10);
  }
}
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
    if(lastKeyStatus[0] == GPIO_PIN_RESET && keyStatus[0] == GPIO_PIN_SET)
    {
      osSemaphoreRelease(keyPressHandle);
    }
    osDelay(20);
  }
}
void tskLcd(void *argument)
{
  for(;;)
  {
    
    osDelay(1000);
  }
}
void tskCamera(void *argument)
{
  DCMI_Start();
  for(;;)
  {
    
    //osSemaphoreAcquire(keyPressHandle,portMAX_DELAY);
    //HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
    //DCMI_Start();
    osDelay(100);
  }
}
