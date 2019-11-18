/*
 * @Author Shi Zhangkun
 * @Date 2019-10-30 21:46:03
 * @LastEditTime 2019-11-17 14:41:05
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath \Project\UsrHal\Src\delay.c
 */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "cmsis_os.h"
#include "task.h"
/* varibe declar----------------------------------------------------------*/
extern uint32_t SystemCoreClock;


/**
 * @brief  delay us
 * @note  OS would not change task in this function
 * @param nus: time us
 * @retval none
 */
void delay_us(uint32_t nus)
{	
  uint32_t fac_us = SystemCoreClock/1000000;	
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				// LOAD value 	    	 
	ticks=nus*fac_us; 						//total ticks 
	vTaskSuspendAll();					//suspend OS 
	told=SysTick->VAL;        	//last_VAL		
	while(1)
	{
		tnow=SysTick->VAL;				//current VAL
		if(tnow!=told)
		{
			//Systick VAL would reduce 1 after one Systick CLK
			if(tnow<told)tcnt+=told-tnow;	//when VAL < last_VAL
			else tcnt+=reload-tnow+told;	//when VAL > last_VAL    
			told=tnow;
			if(tcnt>=ticks)break;			
		}  
	};
	xTaskResumeAll();					//Resume OS											    
}

/**
 * @brief  if os is run use osDelay(),else use Hal_Delay()
 * @note  
 * @param {type} none
 * @retval none
 */
void delay_msOS(uint32_t nms)
{
  if(osKernelGetState() == osKernelRunning)
  {
    osDelay(nms);
  }
  else
  {
    HAL_Delay(nms);
  }
}
