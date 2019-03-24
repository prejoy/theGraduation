/*
 * devKey.c
 *
 *  Created on: 2019年1月16日
 *      Author: prejoy
 */

#include "devKey.h"

int keyScan(int mode)
{
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != 0)
    {
      Delaysomenop(30*1000);
      if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != 0)
	{
	  return 0;
	}
    }
  return 1;
}



inline uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
   return ((uint16_t)GPIOx->IDR);
}

int key_scan(void)
{
  volatile static uint16_t lastkeyvalue = 0xff;
  uint16_t keyvalue=0;
  uint16_t temp = 0;						//判断列
  uint16_t temp1 = 0;						//判断行
  GPIO_InitTypeDef GPIO_Initure;

  GPIO_Initure.Pin = KEY_COL_Pin_ALL;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;
  HAL_GPIO_Init(KEY_COL_GPIO_Port,&GPIO_Initure);
  HAL_GPIO_WritePin (KEY_COL_GPIO_Port, KEY_COL_Pin_ALL,GPIO_PIN_RESET);//拉低列线

  GPIO_Initure.Pin = KEY_ROW_Pin_ALL;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
  HAL_GPIO_Init(KEY_ROW_GPIO_Port,&GPIO_Initure);

//  delay_us (3);
  temp = (GPIO_ReadInputData (KEY_ROW_GPIO_Port)) & KEY_ROW_Pin_ALL;		//行应该在变化
//  printf("ROW:0x%x\r\n",GPIO_ReadInputData (KEY_ROW_GPIO_Port));
  temp ^= KEY_ROW_Pin_ALL;
  if(temp != 0)
//  if((temp&KEY_ROW_Pin_ALL) != KEY_ROW_Pin_ALL)
  {
      OSDelay_ms(5);
      temp = (GPIO_ReadInputData (KEY_ROW_GPIO_Port)) & KEY_ROW_Pin_ALL;
      temp ^= KEY_ROW_Pin_ALL;
        if(temp != 0)
	{
	  goto readkey;
	}
      else
	{
	  goto ret_lab;
	}
  }
  lastkeyvalue = 0xff;
ret_lab:
{
    return 0xff;
}

readkey:
{
  GPIO_Initure.Pin = KEY_ROW_Pin_ALL;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull=GPIO_PULLUP;
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;
  HAL_GPIO_Init(KEY_ROW_GPIO_Port,&GPIO_Initure);
  HAL_GPIO_WritePin (KEY_ROW_GPIO_Port, KEY_ROW_Pin_ALL,GPIO_PIN_RESET);

  GPIO_Initure.Pin = KEY_COL_Pin_ALL;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
  HAL_GPIO_Init(KEY_COL_GPIO_Port,&GPIO_Initure);


//  delay_us (2);
  HAL_GPIO_WritePin (KEY_COL_GPIO_Port, KEY_COL_Pin_ALL,GPIO_PIN_RESET);
  temp1 = (GPIO_ReadInputData (KEY_COL_GPIO_Port)) & KEY_COL_Pin_ALL;
  temp1 ^= KEY_COL_Pin_ALL;

  if(temp == KEY_ROW1_Pin) keyvalue += (0+ROW1_CORRECT);
  else if (temp == KEY_ROW2_Pin) keyvalue += (4+ROW2_CORRECT);
  else if (temp == KEY_ROW3_Pin) keyvalue += (8+ROW3_CORRECT);
  else if (temp == KEY_ROW4_Pin) keyvalue += (12+ROW4_CORRECT);
  else if (temp == KEY_ROW5_Pin) keyvalue += (16+ROW5_CORRECT);
  else goto ret_lab;

  if(temp1 == KEY_COL1_Pin) keyvalue += (1+COL1_CORRECT);
  else if(temp1 == KEY_COL2_Pin) keyvalue += (2+COL2_CORRECT);
  else if(temp1 == KEY_COL3_Pin) keyvalue += (3+COL3_CORRECT);
  else if(temp1 == KEY_COL4_Pin) keyvalue += (4+COL4_CORRECT);
  else goto ret_lab;

  if(lastkeyvalue == 0xff )
    {
      lastkeyvalue = keyvalue;
      return keyvalue;
    }
  else
    {
      return 0xff;
    }
}

}
