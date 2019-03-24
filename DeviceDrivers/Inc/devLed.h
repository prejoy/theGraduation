/*
 * devLed.h
 *
 *  Created on: 2019Äê1ÔÂ16ÈÕ
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVLED_H_
#define DEVICEDRIVERS_INC_DEVLED_H_

#include "stm32f4xx.h"

int setLedOn(int num,int state);
int toggleLed(int num);

#endif /* DEVICEDRIVERS_INC_DEVLED_H_ */
