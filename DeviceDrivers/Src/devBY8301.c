/*
 * devBY8301.c
 *
 *  Created on: 2019年3月15日
 *      Author: prejoy
 */

#include "devBY8301.h"



uint32_t Voice_SendCmd(uint32_t cmd,uint8_t *para,uint32_t len)
{
  uint8_t prtocolcmd[10];
  prtocolcmd[0]=0x7E;
  prtocolcmd[1]=3+len;
  prtocolcmd[2]=cmd;
  if(len == 0)
    {
      prtocolcmd[3]=prtocolcmd[1]^prtocolcmd[2];
      prtocolcmd[4]=0xEF;
      HAL_UART_Transmit(&huart4,prtocolcmd,5,0xffff);
    }
  else if (len == 1)
    {
      prtocolcmd[3]=*para;
      prtocolcmd[4]=prtocolcmd[1]^prtocolcmd[2]^prtocolcmd[3];
      prtocolcmd[5]=0xEF;
      HAL_UART_Transmit(&huart4,prtocolcmd,6,0xffff);
    }
  else if (len == 2)
    {
      prtocolcmd[3]=*para;
      prtocolcmd[4]=*(++para);
      prtocolcmd[5]=prtocolcmd[1]^prtocolcmd[2]^prtocolcmd[3]^prtocolcmd[4];
      prtocolcmd[6]=0xEF;
      HAL_UART_Transmit(&huart4,prtocolcmd,7,0xffff);
    }
  return 0;
}

uint32_t SinglePlay(uint8_t Voice_NO)
{
  uint8_t xCommand[7]={0x7E,0x05,0x41,0x00,0,0,0xEF};
  xCommand[4]=Voice_NO;
  xCommand[5]=xCommand[1]^xCommand[2]^xCommand[3]^xCommand[4];
  HAL_UART_Transmit(&huart4,xCommand,7,0xffff);
  return 0;
}

uint32_t ContinuePlay(uint8_t *list,uint8_t udLen)
{
  uint8_t xCommand[7]={0x7E,0x05,0x41,0x00,0,0,0xEF};

  if(udLen > 10 )
    return 1;

  for (uint8_t i=0;i<udLen;i++)
    {
      xCommand[4]=list[i];
      xCommand[5]=xCommand[1]^xCommand[2]^xCommand[3]^xCommand[4];
      HAL_UART_Transmit(&huart4,xCommand,7,0xffff);
    }
  return 0;
  //高位
  //低位
  //校验码

}
