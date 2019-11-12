/**
  ******************************************************************************
  * File Name          : DCMI.c
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

/* Includes ------------------------------------------------------------------*/
#include "dcmi.h"

/* USER CODE BEGIN 0 */
/* Extern variable  -----------------------------------------------------------------*/
extern osEventFlagsId_t usbSendEvent;
extern uint8_t dataTransPath;
/* variable  -----------------------------------------------------------------*/
ovOutMode_t CameraMode = JPEG_STREAM;   //default jpeg Stream 
#if USE_PSARM_AS_PHOTO_MEM == 1
  //Buff is used in photo mode, in this mode temp Mem is extern psram
  static uint16_t imageBuff[2][DCMI_BUFF_SIZE];    //double buff
  static uint16_t* pImageBuff[2] = {imageBuff[0],imageBuff[1]};     //point to opreate buff
  uint16_t imageBuffIndex = 0; //
  const uint32_t pImageStmWMem = JPEG_POHTO_MEM_W_ADDR;
  const uint32_t pImageStmRMem = JPEG_POHTO_MEM_R_ADDR;
#endif
//In side imageMem used only in stream mode, DMA write data to Mem directly
static uint16_t  imageStmMem[2][OV_JPEG_STREAM_HEIGH/4][OV_JPEG_STREAM_WIDTH/2];//double Mem
static uint16_t  imagePtoMem[OV_JPEG_PHOTO_HEIGH/4][OV_JPEG_PHOTO_WIDTH/2];
const uint8_t* pImageStmMem[3] = {(uint8_t*)imageStmMem[0],(uint8_t*)imageStmMem[1],(uint8_t*)imagePtoMem};//po
static uint8_t imageStmMemIndex = 0;
uint8_t imageStmMemStatus[3] = {0,0,0};   //Mem area busy flag, stmMem0,stmMem1, ptoMem
/* Function declare ------------------------------------------------------------------*/
void DCMI_DMA_CompleteCallback(DMA_HandleTypeDef* hdma);
void DCMI_DMA_SetForNextTransfer(uint16_t* memAddr);
/* USER CODE END 0 */

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma_dcmi;

/* DCMI init function */
void MX_DCMI_Init(void)
{

  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
  hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_LOW;
  hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.JPEGMode = DCMI_JPEG_ENABLE;
  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
  if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_DCMI_MspInit(DCMI_HandleTypeDef* dcmiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dcmiHandle->Instance==DCMI)
  {
  /* USER CODE BEGIN DCMI_MspInit 0 */
    
  /* USER CODE END DCMI_MspInit 0 */
    /* DCMI clock enable */
    __HAL_RCC_DCMI_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**DCMI GPIO Configuration    
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PC8     ------> DCMI_D2
    PC9     ------> DCMI_D3
    PB6     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* DCMI DMA Init */
    /* DCMI Init */
    hdma_dcmi.Instance = DMA1_Stream1;
    hdma_dcmi.Init.Request = DMA_REQUEST_DCMI;
    hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dcmi.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dcmi.Init.Mode = DMA_CIRCULAR;
    hdma_dcmi.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_dcmi.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_dcmi) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(dcmiHandle,DMA_Handle,hdma_dcmi);

    /* DCMI interrupt Init */
    HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
  /* USER CODE BEGIN DCMI_MspInit 1 */
		__HAL_UNLOCK(&hdma_dcmi);
		DCMI->IER=0x0;
    __HAL_DCMI_ENABLE_IT(&hdcmi,DCMI_IT_FRAME);      
    __HAL_DCMI_ENABLE(&hdcmi); 
    __HAL_DMA_ENABLE_IT(&hdma_dcmi,DMA_IT_TC);
		HAL_DMA_RegisterCallback(&hdma_dcmi,HAL_DMA_XFER_CPLT_CB_ID,DCMI_DMA_CompleteCallback);
		HAL_DMA_RegisterCallback(&hdma_dcmi,HAL_DMA_XFER_M1CPLT_CB_ID,DCMI_DMA_CompleteCallback);
    //
    HAL_DMAEx_MultiBufferStart(&hdma_dcmi,(uint32_t)&DCMI->DR,(uint32_t)&imageStmMem[0][0][0],\
																(uint32_t)&imageStmMem[0][0][0]+2*DCMI_BUFF_SIZE,DCMI_BUFF_SIZE/2);
    __HAL_DMA_DISABLE(&hdma_dcmi);//close DMA after init, open when use camera
  

  /* USER CODE END DCMI_MspInit 1 */
  }
}

void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef* dcmiHandle)
{

  if(dcmiHandle->Instance==DCMI)
  {
  /* USER CODE BEGIN DCMI_MspDeInit 0 */

  /* USER CODE END DCMI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DCMI_CLK_DISABLE();
  
    /**DCMI GPIO Configuration    
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PC8     ------> DCMI_D2
    PC9     ------> DCMI_D3
    PB6     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* DCMI DMA DeInit */
    HAL_DMA_DeInit(dcmiHandle->DMA_Handle);

    /* DCMI interrupt Deinit */
    HAL_NVIC_DisableIRQ(DCMI_IRQn);
  /* USER CODE BEGIN DCMI_MspDeInit 1 */

  /* USER CODE END DCMI_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
 * @brief  Start DCMI data transfer
 * @note  
 * @param {type} none
 * @retval none
 */
uint8_t DCMI_Start(void)
{
  if(CameraMode == JPEG_STREAM)
  {
    if(DCMI->CR&0X01)  //if DCMI is running
    {
      return HAL_OK;
    }
    DCMI_Stop();
    DCMI_DMA_SetForNextTransfer(&imageStmMem[imageStmMemIndex][0][0]);
    if(!imageStmMemStatus[0])
      imageStmMemIndex = 0;
    else if(!imageStmMemStatus[1])
      imageStmMemIndex = 1;
    else  //if both mem are busy,we can't open DMA(it would be opened in DCMI_FRAME_IT when Mem not busy)
    {
      DCMI->CR|=DCMI_CR_CAPTURE;
      return HAL_OK;
    }
  }
  else if(CameraMode == JPEG_PHOTO)
  {
    DCMI_DMA_SetForNextTransfer(&imagePtoMem[0][0]);
    if(imageStmMemStatus[2]) //if ptoMem is Busy
    {
      DCMI_Stop();
      return HAL_BUSY;
    }
  }
    __HAL_DMA_ENABLE(&hdma_dcmi);
    __HAL_DMA_ENABLE_IT(&hdma_dcmi,DMA_IT_TC);
    DCMI->CR|=DCMI_CR_CAPTURE;
    return HAL_OK;
}

/**
 * @brief  Stop DCMI data transfer
 * @note  
 * @param {type} none
 * @retval none
 */
void DCMI_Stop(void)
{ 
  
  DCMI->CR&=~(DCMI_CR_CAPTURE);       //关闭捕获
  while(DCMI->CR&0X01);               //等待传输完成
  __HAL_DMA_DISABLE(&hdma_dcmi);//关闭DMA
} 

/**
 * @brief  DCMI frame event callback function
 * @note  
 * @param {type} none
 * @retval none
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{

  if(CameraMode == JPEG_STREAM)
  {
    //alternate use two Memory area to store imgage
    if(imageStmMemIndex<2)                    //If last frame be writen to Mem
      imageStmMemStatus[imageStmMemIndex] = 1;//suspend this Mem
    __HAL_DCMI_ENABLE_IT(hdcmi,DCMI_IT_FRAME);
    __HAL_DMA_DISABLE(&hdma_dcmi);           //Puse DMA
    while(((DMA_Stream_TypeDef   *)hdma_dcmi.Instance)->CR&0X01);//wait DMA stop
    if(imageStmMemStatus[0]&&imageStmMemStatus[1])  //All Mem are busy, not open DMA
    {
      imageStmMemIndex = INVALID_MEM_INDEX;
    }
    else //If not ,prepear DMA for next frame
    {
      if(!imageStmMemStatus[0])
        imageStmMemIndex = 0;
      else if(!imageStmMemStatus[1])
        imageStmMemIndex = 1;
      DCMI_DMA_SetForNextTransfer(&imageStmMem[imageStmMemIndex][0][0]);
      __HAL_DMA_ENABLE(&hdma_dcmi);
      __HAL_DMA_ENABLE_IT(&hdma_dcmi,DMA_IT_TC);
    }
  }
  else if(CameraMode == JPEG_PHOTO)
 {
    DCMI_Stop();
    __HAL_DCMI_ENABLE_IT(hdcmi,DCMI_IT_FRAME);
    __HAL_DMA_CLEAR_FLAG(&hdma_dcmi,DMA_FLAG_FEIF1_5);
    imageStmMemStatus[2] = 1;  
  }
 // 
  if(dataTransPath);

  else
	  osEventFlagsSet(usbSendEvent,USB_SEND_IMAGE_EVENT_BIT);
}

/**
 * @brief  DCMI_DMA Mem0, Mem1 complete IT Callback function 
 * @note  
 * @param {type} none
 * @retval none
 */
void DCMI_DMA_CompleteCallback(DMA_HandleTypeDef* hdma)
{
  if(((DMA_Stream_TypeDef   *)hdma->Instance)->CR&(1<<19))
  {
    ((DMA_Stream_TypeDef   *)hdma->Instance)->M0AR=((DMA_Stream_TypeDef   *)hdma->Instance)->M1AR+2*DCMI_BUFF_SIZE;
  }
  else
  {
    ((DMA_Stream_TypeDef   *)hdma->Instance)->M1AR=((DMA_Stream_TypeDef   *)hdma->Instance)->M0AR+2*DCMI_BUFF_SIZE;
  }
} 

/**
 * @brief Config some DMA register for next transfer
 * @note  Must set DMA_SxCR_CT to zero
 * @param {type} none
 * @retval none
 */
void DCMI_DMA_SetForNextTransfer(uint16_t* memAddr)
{
  ((DMA_Stream_TypeDef   *)hdma_dcmi.Instance)->NDTR = DCMI_BUFF_SIZE/2;
    //Must be clean, because after last transfer CT many be 1(it means data will be write to Mem1 first)
  ((DMA_Stream_TypeDef   *)hdma_dcmi.Instance)->CR &= ~DMA_SxCR_CT_Msk; 
  ((DMA_Stream_TypeDef   *)hdma_dcmi.Instance)->M0AR = (uint32_t)memAddr;
  ((DMA_Stream_TypeDef   *)hdma_dcmi.Instance)->M1AR = (uint32_t)memAddr+2*DCMI_BUFF_SIZE;

}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
