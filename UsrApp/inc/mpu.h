#ifndef __MPU_H
#define __MPU_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//MPU��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//uint8_t MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u32 ap);
uint8_t MPU_Set_Protection(uint32_t baseaddr,uint32_t size,uint32_t rnum,uint8_t ap,uint8_t sen,uint8_t cen,uint8_t ben);
void MPU_Memory_Protection(void);
#endif
