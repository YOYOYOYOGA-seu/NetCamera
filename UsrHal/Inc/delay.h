/*
 * @Author Shi Zhangkun
 * @Date 2019-10-30 21:46:12
 * @LastEditTime 2019-10-31 12:55:35
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrSYS\delay.h
 */

#ifndef __DELAY_H
#define __DELAY_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
/*Commend defines ------------------------------------------------------------*/
#define delayMs(ms)     HAL_Delay(ms)
#define delayUs(us)     delay_us(us)
#define osDelayMs(ms)   delay_msOS(ms)
/* Function declaration ------------------------------------------------------*/
void delay_us(uint32_t nus);
void delay_msOS(uint32_t nms);
#ifdef __cplusplus
}
#endif
#endif
