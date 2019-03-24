/*
 * devAT24C02.h
 *
 *  Created on: 2019Äê2ÔÂ1ÈÕ
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVAT24C02_H_
#define DEVICEDRIVERS_INC_DEVAT24C02_H_

#include "stm32f4xx.h"
#include "i2c.h"

extern uint8_t I2C_Buffer_Write[256];
extern uint8_t I2C_Buffer_Read[256];

uint32_t EEPROM_WRITE(void);
uint32_t EEPROM_READ(void);
#endif /* DEVICEDRIVERS_INC_DEVAT24C02_H_ */
