/*
 * @Author Shi Zhangkun
 * @Date 2019-10-31 12:01:24
 * @LastEditTime 2019-11-03 19:24:23
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\ov_2640.c
 */
/* Includes ------------------------------------------------------------------*/
#include "ov_2640.h"
#include "ov_2640Cmd.h"
#include "sccb.h"
#include "delay.h"
/* Defines -------------------------------------------------------------------*/
extern ovOutMode_t CameraMode; 

/**
 * @brief  ov2640 Init function
 * @note  
 * @param {type} none
 * @retval none
 */
HAL_StatusTypeDef ov2640_Init(void)
{
  uint16_t i = 0;
  uint16_t readID[2];
  OV_PWDN(0);
  delayMs(10);
  OV_RESET(0);
  delayMs(10);
  OV_RESET(1);
  sccbWriteReg(OV2640_DSP_RA_DLMT, 0x01); //操作sensor寄存器
  sccbWriteReg(OV2640_SENSOR_COM7, 0x80); //软复位OV2640
  delayMs(50);
  readID[0] = sccbReadReg(OV2640_SENSOR_MIDH); //读取厂家ID 高八位
  readID[0] <<= 8;
  readID[0] |= sccbReadReg(OV2640_SENSOR_MIDL); //读取厂家ID 低八位
  
  readID[1] = sccbReadReg(OV2640_SENSOR_PIDH); //读取厂家ID 高八位
  readID[1] <<= 8;
  readID[1] |= sccbReadReg(OV2640_SENSOR_PIDL); //读取厂家ID 低八位
  if(readID[0]!=OV2640_MID||readID[1]!=OV2640_PID)
  {
    sprintf(errorInform,"errOV_ID: %X %X\n",readID[0],readID[1]);
    Error_Handler();
    return HAL_ERROR;
  }
  //defult resolution radio :SVGA(800*600)
	 
  for(i=0;i<sizeof(ov2640_svga_init_reg_tbl)/2;i++)
	{
	   	sccbWriteReg(ov2640_svga_init_reg_tbl[i][0],ov2640_svga_init_reg_tbl[i][1]);
 	}
	//ovImageSizeSet(800,600);
  //ovWindowSet(0,0,800,600);
	if(CameraMode)
		ovJPEG_Mode();
	else
		ovRGB565_Mode();
  ovOutSizeSet(OV_RGB_IMGAE_WIDTH,OV_RGB_IMGAE_HEIGH);
  
  //ovSensorImageWinSet(150,0,1200,800);
  //
  /*
  
  //
  for(i=0;i<sizeof(OV2640_176x144_JPEG)/2;i++)
	{
	   	sccbWriteReg(OV2640_176x144_JPEG[i][0],OV2640_176x144_JPEG[i][1]);
 	}
   */
  return HAL_OK;
}

/**
 * @brief  Set OV2640 out put mode to JPEG
 * @note  
 * @param {type} none
 * @retval none
 */
void ovJPEG_Mode(void) 
{
	uint16_t i=0;
	//设置:YUV422格式
	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
	{
		sccbWriteReg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]); 
	} 
	
	//设置:输出JPEG数据
	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
	{
		sccbWriteReg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);  
	}  
}

/**
 * @brief  Set OV2640 out put mode to RGB565
 * @note  
 * @param {type} none
 * @retval none
 */
void ovRGB565_Mode(void) 
{
	uint16_t i=0;
	//设置:RGB565输出
	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
	{
		sccbWriteReg(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]); 
	} 
} 

/**
 * @brief  Set ov2640 auto exposure level
 * @note  
 * @param level:0~4  auto exposure level
 * @retval none
 */
void ovAutoExposure(uint8_t level)
{  
	uint8_t i;
	uint8_t *p=(uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
	for(i=0;i<4;i++)
	{ 
		sccbWriteReg(p[i*2],p[i*2+1]); 
	} 
} 

/**
 * @brief  Set ov2640 white banlance mode
 * @note  
 * @param mode:0  auto
 *            :1  sunny
 *            :2  cloudy
 *            :3  office
 *            :4  home
 * @retval none
 */
void ovLightMode(uint8_t mode)
{
	uint8_t regccval=0X5E;//Sunny 
	uint8_t regcdval=0X41;
	uint8_t regceval=0X54;
	switch(mode)
	{ 
		case 0://auto 
			sccbWriteReg(0XFF,0X00);	 
			sccbWriteReg(0XC7,0X10);//AWB ON 
			return;  	
		case 2://cloudy
			regccval=0X65;
			regcdval=0X41;
			regceval=0X4F;
			break;	
		case 3://office
			regccval=0X52;
			regcdval=0X41;
			regceval=0X66;
			break;	
		case 4://home
			regccval=0X42;
			regcdval=0X3F;
			regceval=0X71;
			break;	
	}
	sccbWriteReg(0XFF,0X00);	 
	sccbWriteReg(0XC7,0X40);	//AWB OFF 
	sccbWriteReg(0XCC,regccval); 
	sccbWriteReg(0XCD,regcdval); 
	sccbWriteReg(0XCE,regceval);  
}

/**
 * @brief  Set ov2640 output picture chroma
 * @note  
 * @param sat:0   -2
 *           :1   -1
 *           :2   0
 *           :3   +1
 *           :4   +2
 * @retval none
 */
void ovChroma(uint8_t sat)
{ 
	uint8_t reg7dval=((sat+2)<<4)|0X08;
	sccbWriteReg(0XFF,0X00);		
	sccbWriteReg(0X7C,0X00);		
	sccbWriteReg(0X7D,0X02);				
	sccbWriteReg(0X7C,0X03);			
	sccbWriteReg(0X7D,reg7dval);			
	sccbWriteReg(0X7D,reg7dval); 		
}

/**
 * @brief  Set ov2640 brightness 
 * @note  
 * @param  bright:0   -2
 *               :1   -1
 *               :2   0
 *               :3   +1
 *               :4   +2   
 * @retval none
 */
void ovBrightness(uint8_t bright)
{
  sccbWriteReg(0xff, 0x00);
  sccbWriteReg(0x7c, 0x00);
  sccbWriteReg(0x7d, 0x04);
  sccbWriteReg(0x7c, 0x09);
  sccbWriteReg(0x7d, bright<<4); 
  sccbWriteReg(0x7d, 0x00); 
}

/**
 * @brief  Set ov2640 contrast
 * @note  
 * @param  contrast:0   -2
 *                 :1   -1
 *                 :2   0
 *                 :3   +1
 *                 :4   +2
 * @retval none
 */
void ovContrast(uint8_t contrast)
{
	uint8_t reg7d0val=0X20;//默认为普通模式
	uint8_t reg7d1val=0X20;
  	switch(contrast)
	{
		case 0://-2
			reg7d0val=0X18;	 	 
			reg7d1val=0X34;	 	 
			break;	
		case 1://-1
			reg7d0val=0X1C;	 	 
			reg7d1val=0X2A;	 	 
			break;	
		case 3://1
			reg7d0val=0X24;	 	 
			reg7d1val=0X16;	 	 
			break;	
		case 4://2
			reg7d0val=0X28;	 	 
			reg7d1val=0X0C;	 	 
			break;	
	}
	sccbWriteReg(0xff,0x00);
	sccbWriteReg(0x7c,0x00);
	sccbWriteReg(0x7d,0x04);
	sccbWriteReg(0x7c,0x07);
	sccbWriteReg(0x7d,0x20);
	sccbWriteReg(0x7d,reg7d0val);
	sccbWriteReg(0x7d,reg7d1val);
	sccbWriteReg(0x7d,0x06);
}

/**
 * @brief  Set ov2640 output image special effects
 * @note  
 * @param  eft:0   normal
 *            :1   negative
 *            :2   B&W
 *            :3   red
 *            :4   green
 *            :5   blue
 *            :6   restore ancient
 * 
 * @retval none
 */   
void ovSpecialEffects(uint8_t eft)
{
	uint8_t reg7d0val=0X00;//默认为普通模式
	uint8_t reg7d1val=0X80;
	uint8_t reg7d2val=0X80; 
	switch(eft)
	{
		case 1://负片
			reg7d0val=0X40; 
			break;	
		case 2://黑白
			reg7d0val=0X18; 
			break;	 
		case 3://偏红色
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0XC0; 
			break;	
		case 4://偏绿色
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0X40; 
			break;	
		case 5://偏蓝色
			reg7d0val=0X18; 
			reg7d1val=0XA0;
			reg7d2val=0X40; 
			break;	
		case 6://复古
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0XA6; 
			break;	 
	}
	sccbWriteReg(0xff,0x00);
	sccbWriteReg(0x7c,0x00);
	sccbWriteReg(0x7d,reg7d0val);
	sccbWriteReg(0x7c,0x05);
	sccbWriteReg(0x7d,reg7d1val);
	sccbWriteReg(0x7d,reg7d2val); 
}

/**
 * @brief  Color bar test 
 * @note   The color bar will overlie the image
 * @param  sw:0   close
 *           :1   open
 * @retval none
 */
void ovColorBar(uint8_t sw)
{
	uint8_t reg;
	sccbWriteReg(0XFF,0X01);
	reg=sccbReadReg(0X12);
	reg&=~(1<<1);
	if(sw)reg|=1<<1; 
	sccbWriteReg(0X12,reg);
}

/**
 * @brief  Set window size of output image 
 * @note  
 * @param  offx:start point X coordinate
 * @param  offy:start point Y coordinate  
 * @param  width:window width
 * @param  height:window height  
 * @retval none
 */
uint8_t ovWindowSet(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height)
{
  uint16_t hsize;
	uint16_t vsize;
	uint8_t temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	sccbWriteReg(0XFF,0X00);	
	sccbWriteReg(0XE0,0X04);					
	sccbWriteReg(0X51,hsize&0XFF);		//设置H_SIZE的低八位
	sccbWriteReg(0X52,vsize&0XFF);		//设置V_SIZE的低八位
	sccbWriteReg(0X53,offx&0XFF);		//设置offx的低八位
	sccbWriteReg(0X54,offy&0XFF);		//设置offy的低八位
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07; 
	sccbWriteReg(0X55,temp);				//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	sccbWriteReg(0X57,(hsize>>2)&0X80);	//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	sccbWriteReg(0XE0,0X00);	
	return 0;
}

/**
 * @brief  Set the size of out put image
 * @note  width and height must be the multiple of 4
 *        and after change it you should also change the DMCI setiing
 *        width:(horizontal)     height:(vertical)
 * @param  width: width
 * @param  height: height  
 * @retval 0:   success
 *         1:   error
 */
uint8_t ovOutSizeSet(uint16_t width,uint16_t height)
{
	uint16_t outh;
	uint16_t outw;
	uint8_t temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4; 
	sccbWriteReg(0XFF,0X00);	
	sccbWriteReg(0XE0,0X04);			
	sccbWriteReg(0X5A,outw&0XFF);		//设置OUTW的低八位
	sccbWriteReg(0X5B,outh&0XFF);		//设置OUTH的低八位
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	sccbWriteReg(0X5C,temp);				//设置OUTH/OUTW的高位 
	sccbWriteReg(0XE0,0X00);	
	return 0;
}

/**
 * @brief  Set the window size of sensor output image signal
 * @note   width and height must bigger than the value set in 
 *         function ovOutSizeSet(outWidth,outHeight).
 *         DSP will caculate the scale rate by ovOutSizeSet() 
 *         and size in this function.
 * @param  sx:start point X coordinate
 * @param  sy:start point Y coordinate  
 * @param  width:window width
 * @param  height:window height  
 * @retval 0:   success
 *         1:   error
 */
void ovSensorImageWinSet(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
  	uint16_t endx;
	uint16_t endy;
	uint8_t temp; 
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;
	
	sccbWriteReg(0XFF,0X01);			
	temp=sccbReadReg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	sccbWriteReg(0X03,temp);				//设置Vref的start和end的最低2位
	sccbWriteReg(0X19,sy>>2);			//设置Vref的start高8位
	sccbWriteReg(0X1A,endy>>2);			//设置Vref的end的高8位
	
	temp=sccbReadReg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	sccbWriteReg(0X32,temp);				//设置Href的start和end的最低3位
	sccbWriteReg(0X17,sx>>3);			//设置Href的start高8位
	sccbWriteReg(0X18,endx>>3);			//设置Href的end的高8位
	
} 

/**
 * @brief  Set the size of sensor output image
 * @note   UXGA:1600*1200,SVGA:800*600,CIF:352*288
 * @param  width:width
 * @param  height:height  
 * @retval 0:   success
 *         1:   error
 */
uint8_t ovImageSizeSet(uint16_t width,uint16_t height)
{ 
	uint8_t temp; 
	sccbWriteReg(0XFF,0X00);			
	sccbWriteReg(0XE0,0X04);			
	sccbWriteReg(0XC0,(width)>>3&0XFF);		//设置HSIZE的10:3位
	sccbWriteReg(0XC1,(height)>>3&0XFF);		//设置VSIZE的10:3位
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80; 
	sccbWriteReg(0X8C,temp);	
	sccbWriteReg(0XE0,0X00);				 
	return 0;
}
