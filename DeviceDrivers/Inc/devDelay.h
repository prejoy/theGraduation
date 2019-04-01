/*
 * devDelay.h
 *
 *  Created on: 2019Äê1ÔÂ12ÈÕ
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVDELAY_H_
#define DEVICEDRIVERS_INC_DEVDELAY_H_

#include "stdint.h"
#include "tim.h"
#include "stm32f4xx_hal.h"

//#undef  FREERTOS

#ifndef FREERTOS
#define OSDelay_ms(x)	delay_ms(x)
#else
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#define OSDelay_ms(x)  vTaskDelay(pdMS_TO_TICKS(x))
#endif

void Delaysomenop(uint32_t num);
void delay_us(uint16_t nus);
void delay_ms(uint32_t nms);


#endif /* DEVICEDRIVERS_INC_DEVDELAY_H_ */
