/*
 * devSdram.c
 *
 *  Created on: 2019年1月29日
 *      Author: prejoy
 */


#include "devSdram.h"

//uint16_t testsram[250000] __attribute__((section(".sdram")));

void SDRAM_Init(void)
{
  SDRAM_Initialization_Sequence(&hsdram1);
  HAL_SDRAM_ProgramRefreshRate(&hsdram1,683);
}

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
  uint32_t temp=0;

    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0);
    Delaysomenop(400);                                  //至少延时200us
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);

    temp=(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 |
	  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
	  SDRAM_MODEREG_CAS_LATENCY_3           |
	  SDRAM_MODEREG_OPERATING_MODE_STANDARD |
	  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp);

}


uint8_t SDRAM_Send_Cmd(uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;

    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;
    Command.CommandMode=cmd;
    Command.CommandTarget=target_bank;
    Command.AutoRefreshNumber=refresh;
    Command.ModeRegisterDefinition=regval;
    if(HAL_SDRAM_SendCommand(&hsdram1,&Command,0X1000)==HAL_OK)
    {
        return 0;
    }
    else
      return 1;
}


void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n)
{
    for(;n!=0;n--)
    {
	*(volatile uint8_t*)(Bank5_SDRAM_ADDR+WriteAddr)=*pBuffer;
	WriteAddr++;
	pBuffer++;
    }
}



void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
{
    for(;n!=0;n--)
    {
	*pBuffer++=*(volatile uint8_t*)(Bank5_SDRAM_ADDR+ReadAddr);
	ReadAddr++;
    }
}


void fsmc_sdram_test(u16 x,u16 y)
{
	u32 i=0;
	u32 temp=0;
	u32 sval=0;

	for(i=0;i<32*1024*1024;i+=16*1024)
	{
		*(vu32*)(Bank5_SDRAM_ADDR+i)=temp;
		temp++;
	}
 	for(i=0;i<32*1024*1024;i+=16*1024)
	{
  		temp=*(vu32*)(Bank5_SDRAM_ADDR+i);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;
		printf("SDRAM Capacity:%dKB\r\n",(u16)(temp-sval+1)*16);
 	}
}
