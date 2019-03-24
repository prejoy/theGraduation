/*
 * devAT24C02.c
 *
 *  Created on: 2019Äê2ÔÂ1ÈÕ
 *      Author: prejoy
 */

#include "devAT24C02.h"

#define ADDR_AT24C02_Write 0xA0
#define ADDR_AT24C02_Read 0xA1

uint8_t I2C_Buffer_Write[256];
uint8_t I2C_Buffer_Read[256];


uint32_t EEPROM_WRITE(void)
{
  for(uint32_t i=0; i<256; i=i+8)
    {
	if (HAL_I2C_Mem_Write(&hi2c2, ADDR_AT24C02_Write, i, I2C_MEMADD_SIZE_8BIT,&(I2C_Buffer_Write[i]),8, 1000) == HAL_OK)
	    continue;
	else
	    return 1;
    }
  return 0;
}

uint32_t EEPROM_READ(void)
{
  uint32_t i=0;
  printf("\r\n Reading from AT24C02:\r\n");
  HAL_I2C_Mem_Read(&hi2c2, ADDR_AT24C02_Read, 0, I2C_MEMADD_SIZE_8BIT,I2C_Buffer_Read,256, 1000);
  for(i=0; i<256; i++)
  {
    printf("0x%02X  ",I2C_Buffer_Read[i]);
  }
  return 0;
}

/*	simple test
    uint16_t i=0;
    printf("\r\n I2C Write Buffer:\r\n");
    for(i=0; i<256; i++)
    {
	I2C_Buffer_Write[i]=i;    // WriteBuffer Initialization
	printf("%X ", I2C_Buffer_Write[i]);

    }

    EEPROM_WRITE();
    EEPROM_READ();
    HAL_Delay(2000);
*/

