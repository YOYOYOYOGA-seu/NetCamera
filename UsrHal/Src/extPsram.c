/*
 * @Author Shi Zhangkun
 * @Date 2019-10-23 21:28:20
 * @LastEditTime 2019-11-17 16:57:50
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\extPsram.c
 */
/*
 * @Author Shi Zhangkun
 * @Date 2019-10-23 21:28:20
 * @LastEditTime 2019-10-25 00:19:34
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\extPsram.c
 */

/* Includes ------------------------------------------------------------------*/
#include "extPsram.h"
#include "delay.h"
static uint8_t psramID[4];
/*Commend defines ------------------------------------------------------------*/
#define psramEnterQPI()   QSPI_Send_CMD(PSRAM_CMD_ENTER_QPI,0x00,0,\
                         QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE)
#define psramExitQPI()   QSPI_Send_CMD(PSRAM_CMD_EXIT_QPI,0x00,0,\
                         QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE)
#define psramReadID()                \
{\
  psramExitQPI();\
  QSPI_Send_CMD(PSRAM_CMD_READ_ID,0x00,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE);\
  QSPI_Read(psramID,2);                    \
}

/**
  * @brief  initilize psram and set to QPI mode
  * @param  :none
  * @retval none
  */
void psarmInit(void)
{
  psramReadID();

  if(psramID[0] != PSRAM_ID_L||psramID[1] != PSRAM_ID_H)
  {
    strcpy(errorInform, "\nerrPsramEnterQPI\0");
    Error_Handler();
    return;
  }
  delayMs(10);
  psramEnterQPI();
  delayMs(10);
  psramWrite(&psramID[0],0x000000,2);
  delayMs(10);
  psramRead(&psramID[2],0x000000,2);
  if(psramID[2] != PSRAM_ID_L||psramID[3] != PSRAM_ID_H)
  {
    strcpy(errorInform, "\nerrPsramEnterQPI\0");
    Error_Handler(); 
    return;
  }
  strcpy(errorInform, "\nokPsramInit\0");
  Error_Handler();
  return;
}

/**
  * @brief  Write data to psram
  * @param  buf:data buff
  * @param  addr:address
  * @param  num:size of data 
  * @retval none
  */
void psramWrite(uint8_t* buf, uint32_t addr, uint32_t num)
{
  
  QSPI_Send_CMD(PSRAM_CMD_WRITE_QUAD,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
  if(QSPI_Write(buf,num)!=HAL_OK)
  {
    strcpy(errorInform, "\nerrPsramIndir_W\0");
    Error_Handler();
  }
  
}

/**
  * @brief  Read data to psram
  * @param  buf:data buff
  * @param  addr:address
  * @param  num:size of data 
  * @retval none
  */
void psramRead(uint8_t* buf, uint32_t addr, uint32_t num)
{
  QSPI_Send_CMD(PSRAM_CMD_READ_QUAD,addr,PSRAM_QPI_READ_DUMCYCLES,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
  if(QSPI_Read(buf,num)!=HAL_OK)
  {
    strcpy(errorInform, "\nerrPsramIndir_R\0");
    Error_Handler();
  }
}


