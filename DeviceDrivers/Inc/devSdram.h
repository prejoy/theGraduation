/*
 * devSdram.h
 *
 *  Created on: 2019��1��29��
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVSDRAM_H_
#define DEVICEDRIVERS_INC_DEVSDRAM_H_

#include "stm32f4xx.h"
#include "devDelay.h"
#include "sys.h"


extern SDRAM_HandleTypeDef hsdram1;

#define Bank5_SDRAM_ADDR    ((uint32_t)(0XC0000000)) //SDRA ��ʼ��ַ

//SDRAM ���ò���
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/* ����SDRAM ��ʼ������    */
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);

/*  ��SDRAM��������  */
uint8_t SDRAM_Send_Cmd(uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval);

/*sdram ��ʼ��*/
void SDRAM_Init(void);

/* ��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�  */
void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n);

/* ��ָ����ַ(ReadAddr+Bank5_SDRAM_ADDR)��ʼ,��������n���ֽ�  */
void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n);

/*�ڴ����*/
void fsmc_sdram_test(u16 x,u16 y);


#endif /* DEVICEDRIVERS_INC_DEVSDRAM_H_ */
