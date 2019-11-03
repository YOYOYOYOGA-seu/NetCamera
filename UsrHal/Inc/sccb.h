/*
 * @Author Shi Zhangkun
 * @Date 2019-10-30 21:10:37
 * @LastEditTime 2019-11-01 11:03:23
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Inc\sccb.h
 */
#ifndef __SCCB_H
#define __SCCB_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Commend defines -----------------------------------------------------------*/
#define SCCB_SDA_IN()     {OV_SDA_GPIO_Port->MODER&=~(3*OV_SDA_Pin*OV_SDA_Pin);}	
#define SCCB_SDA_OUT()    {OV_SDA_GPIO_Port->MODER&=~(3*OV_SDA_Pin*OV_SDA_Pin);OV_SDA_GPIO_Port->MODER|=(OV_SDA_Pin*OV_SDA_Pin);} 	         
#define SCCB_SDA(a)       HAL_GPIO_WritePin(OV_SDA_GPIO_Port,OV_SDA_Pin,(GPIO_PinState)a)
#define SCCB_SDA_READ()   HAL_GPIO_ReadPin(OV_SDA_GPIO_Port,OV_SDA_Pin)
#define SCCB_SCL(a)       HAL_GPIO_WritePin(OV_SCL_GPIO_Port,OV_SCL_Pin,(GPIO_PinState)a)
#define SCCB_DELAY()       delayUs(50)
/* Defines ------------------------------------------------------------------*/
#define SCCB_ID     OV_2640_SCCB_ID
/* Function declaration ------------------------------------------------------*/

uint8_t sccbWriteReg(uint8_t reg,uint8_t data);
uint8_t sccbReadReg(uint8_t reg);
#ifdef __cplusplus
}
#endif
#endif
