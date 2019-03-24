/*
 * devBY8301.h
 *
 *  Created on: 2019Äê3ÔÂ15ÈÕ
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVBY8301_H_
#define DEVICEDRIVERS_INC_DEVBY8301_H_

#include "stdint.h"
#include "stm32f4xx.h"


extern UART_HandleTypeDef huart4;

uint32_t SinglePlay(uint8_t Voice_NO);
uint32_t ContinuePlay(uint8_t *list,uint8_t udLen);

#endif /* DEVICEDRIVERS_INC_DEVBY8301_H_ */
