/*
 * @Author Shi Zhangkun
 * @Date 2019-11-01 21:16:16
 * @LastEditTime 2019-11-19 00:18:04
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrApp\inc\osTask.h
 */
#ifndef __OSTASK_H
#define __OSTASK_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Type define --------------------------------------------------------------*/
typedef enum{
  jpegHeadNum = 0,
  jpegSize
}jpegMessageIndex_t;
/* Event bit defines ------------------------------------------------------------------*/
#define KEY_ALL_EVENT_BIT         0x001F
#define KEY_LEFT_EVENT_BIT        0x0001
#define KEY_RIGHT_EVENT_BIT       0x0002
#define KEY_UP_EVENT_BIT          0x0004
#define KEY_DOWN_EVENT_BIT        0x0008
#define KEY_PRESS_EVENT_BIT       0x0010

#define CAMERA_ALL_EVENT_BIT      0x001F
#define CAMERA_START_EVENT_BIT    0x0001
#define CAMERA_STOP_EVENT_BIT     0x0002
#define CAMERA_PHOTO_EVENT_BIT    0x0004


#define IMAGE_SEND_ALL_EVENT_BIT        0x000F
#define IMAGE_SEND_ONE_FRAME_EVENT_BIT  0x0001

#define WIFI_SEND_ALL_EVENT_BIT    0x000F
#define WIFI_SEND_IMAGE_EVENT_BIT  0x0001

#ifdef __cplusplus
}
#endif
#endif
