/*
 * @Author Shi Zhangkun
 * @Date 2019-10-30 21:10:45
 * @LastEditTime 2019-10-31 13:52:08
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\sccb.c
 */

/* Includes ------------------------------------------------------------------*/
#include "sccb.h"
#include "delay.h"
#include "ov_2640.h"
/* Private function declare --------------------------------------------------*/
void sccbStart(void);
void sccbStop(void);
void sccbNoAck(void);
uint8_t sccbWriteByte(uint8_t temp);
uint8_t sccbReadByte(void);

/**
 * @brief  SCCB frame Start signal
 * @note  
 * @param {type} none
 * @retval none
 */
void sccbStart(void)
{
  SCCB_SDA_OUT();
  SCCB_SDA(1);
  SCCB_SCL(1);
  SCCB_DELAY();
  SCCB_SDA(0);
  SCCB_DELAY();
  SCCB_SCL(0);
}

/**
 * @brief  SCCB frame Stop signal
 * @note  
 * @param {type} none
 * @retval none
 */
void sccbStop(void)
{
  SCCB_SDA_OUT();
  SCCB_SDA(0);
  SCCB_DELAY();
  SCCB_SCL(1);
  SCCB_DELAY();
  SCCB_SDA(1);
  SCCB_DELAY();
}

/**
 * @brief  SCCB NO_ACK signal
 * @note  
 * @param {type} none
 * @retval none
 */
void sccbNoAck(void)
{
  SCCB_SDA_OUT();
  SCCB_DELAY(); 
  SCCB_SDA(1);
  SCCB_SCL(1);
  SCCB_DELAY();
  SCCB_SCL(0);
  SCCB_DELAY();
  SCCB_SDA(0);
}

/**
 * @brief  SCCB write one byte
 * @note  
 * @param {type} none
 * @retval 1:error  0:success
 */
uint8_t sccbWriteByte(uint8_t temp)
{
  uint8_t i, status = 0;
  SCCB_SDA_OUT();
  for (i = 0; i < 8; i++)
  {
    if (temp & 0x80)
      SCCB_SDA(1);
    else
      SCCB_SDA(0);
    temp <<= 1;
    SCCB_DELAY();
    SCCB_SCL(1);
    SCCB_DELAY();
    SCCB_SCL(0);
  }
  SCCB_SDA_IN();
  SCCB_DELAY();
  SCCB_SCL(1);
  SCCB_DELAY();
  status = SCCB_SDA_READ();
  SCCB_SCL(0);
  return status;
}

/**
 * @brief  SCCB read one byte
 * @note  
 * @param {type} none
 * @retval read value
 */
uint8_t sccbReadByte(void)
{
  uint8_t i, readValue = 0;
  SCCB_SDA_IN();
  for (i = 0; i < 8; i++)
  {
    SCCB_DELAY();
    SCCB_SCL(1);
    readValue <<= 1;
    if (SCCB_SDA_READ())
      readValue++;
    SCCB_DELAY();
    SCCB_SCL(0);
  }
  sccbNoAck();
  return readValue;
}

/**
 * @brief  
 * @note  
 * @param  reg:the register address
 * @param  data:data
 * @retval 1:error  0:success
 */
uint8_t sccbWriteReg(uint8_t reg, uint8_t data)
{
  uint8_t status = 0;
  sccbStart();
  if (sccbWriteByte(SCCB_ID))
    status = 1; //write ID
  if (sccbWriteByte(reg))
    status = 1; //Write reg adress
  if (sccbWriteByte(data))
    status = 1; //write data
  sccbStop();
  return status;
}

/**
 * @brief  
 * @note  
 * @param {type} none
 * @retval none
 */
uint8_t sccbReadReg(uint8_t reg)
{
  uint8_t readValue = 0;
  sccbStart();
  sccbWriteByte(SCCB_ID); //write ID
  sccbWriteByte(reg);     //set reg adress
  sccbStop();
  SCCB_DELAY();

  sccbStart();
  sccbWriteByte(SCCB_ID | 0X01); //sent read cmd
  readValue = sccbReadByte();   //read data
  sccbStop();
  return readValue;
}
