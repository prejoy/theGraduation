/*
 * devDelay.c
 *
 *  Created on: 2019Äê1ÔÂ12ÈÕ
 *      Author: prejoy
 */


#include "devDelay.h"



void Delaysomenop(uint32_t num)
{
	while(num--)
	{
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
		__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");__asm("nop\n");
	}
}

void delay_us(uint16_t nus)
{
  __HAL_TIM_SET_COUNTER(&htim3,0);
  __HAL_TIM_ENABLE(&htim3);
  while(__HAL_TIM_GET_COUNTER(&htim3) < nus);
  __HAL_TIM_DISABLE(&htim3);
}

//volatile static uint32_t Tcounter = 0;
void delay_ms(uint32_t nms)
{
//  Tcounter = nms ;
//  while(Tcounter);
  HAL_Delay(nms);
}



//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if(htim==(&htim3))
//  {
//      if(Tcounter)
//	Tcounter--;
//  }
//  if (htim->Instance == TIM6) {
//      HAL_IncTick();
//    }
//}

