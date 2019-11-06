/*
 * @Author Shi Zhangkun
 * @Date 2019-10-31 12:00:29
 * @LastEditTime 2019-11-06 22:06:32
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Inc\ov_2640.h
 */
#ifndef __OV_2640_H
#define __OV_2640_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Type defines -----------------------------------------------------------*/
typedef enum
{
  RGB_STREAM = 0,
  JPEG_STREAM,
  JPEG_PHOTO
}ovOutMode_t;
typedef enum
{
  OV_PWOER_OFF = 0,
  OV_PWOER_ON = 1
}ovPowerStatus_t;
/* Commend defines -----------------------------------------------------------*/

#define OV_PWDN(a)      HAL_GPIO_WritePin(OV_PWDN_GPIO_Port,OV_PWDN_Pin,(GPIO_PinState)a)
#define OV_RESET(a)      HAL_GPIO_WritePin(OV_RESET_GPIO_Port,OV_RESET_Pin,(GPIO_PinState)a)
/* Defines -------------------------------------------------------------------*/
#define OV_RGB_IMGAE_WIDTH          480  
#define OV_RGB_IMGAE_HEIGH          360

#define OV_JPEG_STREAM_WIDTH        240      
#define OV_JPEG_STREAM_HEIGH        180  

#define OV_JPEG_PHOTO_WIDTH         800     
#define OV_JPEG_PHOTO_HEIGH         600 

#define OV_2640_SCCB_ID      0X60
#define OV2640_MID				0X7FA2
#define OV2640_PID				0X2642

//when choiced DSP register(0XFF=0X00),the register address of DSP
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE1       0x51
#define OV2640_DSP_VSIZE1       0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF 

//when choiced sensor register(0XFF=0X01),the register address of sensor
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62
/* Extern variable------- ------------------------------------------------------*/
extern ovPowerStatus_t ovPowerStatus;
/* Function declaration ------------------------------------------------------*/
HAL_StatusTypeDef ov2640_Init(void);
HAL_StatusTypeDef ov2640_DeInit(void);
void ovJPEG_Mode(void);
void ovRGB565_Mode(void);
void ovAutoExposure(uint8_t level);
void ovLightMode(uint8_t mode);
void ovChroma(uint8_t sat);
void ovBrightness(uint8_t bright);
void ovContrast(uint8_t contrast);
void ovSpecialEffects(uint8_t eft);
void ovColorBar(uint8_t sw);
uint8_t ovWindowSet(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
uint8_t ovOutSizeSet(uint16_t width,uint16_t height);
void ovSensorImageWinSet(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t ovImageSizeSet(uint16_t width,uint16_t height);

#ifdef __cplusplus
}
#endif
#endif
