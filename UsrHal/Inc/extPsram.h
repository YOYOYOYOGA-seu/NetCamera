/*
 * @Author Shi Zhangkun
 * @Date 2019-10-23 21:28:47
 * @LastEditTime 2019-11-03 23:16:19
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Inc\extPsram.h
 */
#ifndef __EXTPSRAM_H
#define __EXTPSRAM_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"
/*Memeory area defines -------------------------------------------------------*/
#define PSRAM_TOTAL_SIZE            0x800000
#define CODE_MEM_SIZE               0x200000      //2Mbyte
#define JPEG_POHTO_IMAGE_MEM_SIZE   0x200000      //2MKbyte
#define JPEG_STREAM_IMAGE_MEM_SIZE  0x080000      //512Kbyte
#define RGB_IMAGE_MEM_SIZE          0x080000      //512Kbyte
#define RESERVED_MEM_SIZE           0x300000      //3MKbyte, managed by malloc


#define CODE_MEM_W_ADDR       0x000000
#define CODE_MEM_R_ADDR       QSPI_BASE+CODE_MEM_W_ADDR)

#define JPEG_POHTO_MEM_W_ADDR  CODE_MEM_W_ADDR+CODE_MEM_SIZE
#define JPEG_POHTO_MEM_R_ADDR  QSPI_BASE+JPEG_POHTO_MEM_W_ADDR

#define JPEG_STREAM_MEM_W_ADDR  JPEG_POHTO_MEM_W_ADDR+JPEG_POHTO_IMAGE_MEM_SIZE
#define JPEG_STREAM_MEM_R_ADDR  QSPI_BASE+JPEG_STREAM_MEM_W_ADDR

#define RGB_IMAGE_MEM_W_ADDR  JPEG_STREAM_MEM_W_ADDR+JPEG_STREAM_IMAGE_MEM_SIZE
#define RGB_IMAGE_MEM_R_ADDR  QSPI_BASE+RGB_IMAGE_MEM_W_ADDR

#define RESERVED_MEM_W_ADDR  RGB_IMAGE_MEM_W_ADDR+RGB_IMAGE_MEM_SIZE
#define RESERVED_MEM_R_ADDR  QSPI_BASE+RESERVED_MEM_W_ADDR
/*Information defines --------------------------------------------------------*/
#define PSRAM_ID_H 0x5D
#define PSRAM_ID_L 0x0D
#define PSRAM_QUICK_DUMCYCLES 0x08
#define PSRAM_QPI_READ_DUMCYCLES 0x06
/*Instruction code defines ---------------------------------------------------*/
#define PSRAM_CMD_READ_ID 0x9F
#define PSRAM_CMD_READ_QUAD 0xEB
#define PSRAM_CMD_WRITE_QUAD 0x38
#define PSRAM_CMD_ENTER_QPI 0x35
#define PSRAM_CMD_EXIT_QPI 0xF5
/*Commend defines ------------------------------------------------------------*/
#define psarmEnterMemoryMapped() QSPI_SetToMemoryMapped(PSRAM_CMD_READ_QUAD,PSRAM_QPI_READ_DUMCYCLES,     \
                                 QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES)
/* Function declaration ------------------------------------------------------*/
void psarmInit(void);
void psramWrite(uint8_t* buf, uint32_t addr, uint32_t num); 
void psramRead(uint8_t* buf, uint32_t addr, uint32_t num);

#ifdef __cplusplus
}
#endif
#endif
