/**
  ******************************************************************************
  * File Name          : QUADSPI.c
  * Description        : This file provides code for the configuration
  *                      of the QUADSPI instances.
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
#include "quadspi.h"

/* USER CODE BEGIN 0 */
static QSPI_CommandTypeDef hqspic;
static QSPI_MemoryMappedTypeDef hqspimm;
/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 22;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_2;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**QUADSPI GPIO Configuration    
    PB2     ------> QUADSPI_CLK
    PE7     ------> QUADSPI_BK2_IO0
    PE8     ------> QUADSPI_BK2_IO1
    PE9     ------> QUADSPI_BK2_IO2
    PE10     ------> QUADSPI_BK2_IO3
    PC11     ------> QUADSPI_BK2_NCS 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();
  
    /**QUADSPI GPIO Configuration    
    PB2     ------> QUADSPI_CLK
    PE7     ------> QUADSPI_BK2_IO0
    PE8     ------> QUADSPI_BK2_IO1
    PE9     ------> QUADSPI_BK2_IO2
    PE10     ------> QUADSPI_BK2_IO3
    PC11     ------> QUADSPI_BK2_NCS 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/**
  * @brief  QSPI set and sent cmmond
  * @param  instr : 8bit instruction
  * @param  adress : adress
  * @param  dumCycles : empty clock cycles after address 
  * @param  iMode : instruction send mode
  *                 00 empty 
  *                 01 one line
  *                 10 two line
  *                 11 four line 
  * @param  aMode : address send mode(same as upon)
  * @param  aSize : address bit size
  * @param  dMode : data send mode(same as upon)
  * @retval None
  */
void QSPI_Send_CMD(uint8_t instr,uint32_t address,uint32_t dumCycles,uint32_t iMode,uint32_t aMode,uint32_t aSize,uint32_t dMode)
{
  //if QSPI is in MEM_MAPPED mode, we must abort it or wen can't operate QSPI
  if(hqspi.State == HAL_QSPI_STATE_BUSY_MEM_MAPPED)
  {
    hqspi.Instance->CR |= QUADSPI_CR_ABORT;
    hqspi.State = HAL_QSPI_STATE_READY;
  }
  hqspic.Instruction = instr;
  hqspic.Address = address;
  hqspic.DummyCycles = dumCycles;
  hqspic.InstructionMode = iMode;
  hqspic.AddressMode = aMode;
  hqspic.AddressSize = aSize;
  hqspic.DataMode = dMode;
  hqspic.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  hqspic.DdrHoldHalfCycle = QSPI_DDR_HHC_HALF_CLK_DELAY;
  hqspic.DdrMode = QSPI_DDR_MODE_DISABLE;
  hqspic.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  if(HAL_QSPI_Command(&hqspi,&hqspic,3000)!=HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  QSPI read data
  * @param  buf : data buff
  * @param  num : size of data
  * @retval Status
  */
HAL_StatusTypeDef QSPI_Read(uint8_t* buf,uint32_t num)
{
  hqspi.Instance->DLR = num - 1;
  return HAL_QSPI_Receive(&hqspi,buf,5000);
}

/**
  * @brief  QSPI write data
  * @param  buf : data buff
  * @param  num : size of data
  * @retval Status
  */
HAL_StatusTypeDef QSPI_Write(uint8_t* buf,uint32_t num)
{
  
  hqspi.Instance->DLR = num - 1;
  return HAL_QSPI_Transmit(&hqspi,buf,5000);
}


/**
 * @brief  set QSPI to MemoryMapped Mode
 * @note  
 * @param  instr : 8bit instruction
 * @param  adress : adress
 * @param  dumCycles : empty clock cycles after address 
 * @param  iMode : instruction send mode
 *                 00 empty 
 *                 01 one line
 *                 10 two line
 *                 11 four line 
 * @param  aMode : address send mode(same as upon)
 * @param  aSize : address bit size
 * @param  dMode : data send mode(same as upon)
 * @retval none
 */
HAL_StatusTypeDef QSPI_SetToMemoryMapped(uint8_t instr,uint32_t dumCycles,uint32_t iMode,uint32_t aMode,uint32_t aSize,uint32_t dMode)
{
  //At first set read cmd and other setings(auto load) 
  hqspic.Instruction = instr;
  hqspic.Address = 0x00;
  hqspic.DummyCycles = dumCycles;
  hqspic.InstructionMode = iMode;
  hqspic.AddressMode = aMode;
  hqspic.AddressSize = aSize;
  hqspic.DataMode = dMode;
  hqspic.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  hqspic.DdrHoldHalfCycle = QSPI_DDR_HHC_HALF_CLK_DELAY;
  hqspic.DdrMode = QSPI_DDR_MODE_DISABLE;
  hqspic.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
  if(HAL_QSPI_Command(&hqspi,&hqspic,3000)!=HAL_OK)
  {
    Error_Handler();
  }

  hqspimm.TimeOutActivation = QSPI_TIMEOUT_COUNTER_ENABLE;
  hqspimm.TimeOutPeriod = 1000;
  //Enter memory mapped mode
  if(HAL_QSPI_MemoryMapped(&hqspi,&hqspic,&hqspimm)!=HAL_OK)
  {
    Error_Handler();
  }

  return HAL_OK;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
