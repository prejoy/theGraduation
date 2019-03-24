/*
 * devLed.c
 *
 *  Created on: 2019Äê1ÔÂ16ÈÕ
 *      Author: prejoy
 */

#include "devLed.h"

int setLedOn(int num,int state)
{
  if(num == 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1-state);
      return 0;
    }
  return 1;
}

int toggleLed(int num)
{
  if(num == 0)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    }
  return 1;
}
