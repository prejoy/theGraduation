/*
 * App_Pay.c
 *
 *  Created on: 2019年3月21日
 *      Author: prejoy
 */

#include "App_Pay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


xCostCB xCCB={0,0,0,0,0,0};
uint32_t RecentCostDetail[2*100]={0};	//ID Money 可以配合Nand 显示信息
uint8_t posiRecentCostDetail=0;

uint32_t UserTblInit(uint32_t addr)
{
  uint32_t UserTable_temp[1024];
  uint32_t key1,key2;
  W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
  key1 = UserTable_temp[1024-4];
  key2 = UserTable_temp[1024-3];
  if((key1!=0x12345678)||(key2!=0x90abcdef))
    {
      UserTable_temp[1024-4] = 0x12345678;
      UserTable_temp[1024-3] = 0x90abcdef;
      UserTable_temp[1024-2] = 0x00000000;
      W25QXX_Write((uint8_t*)UserTable_temp,0,4096);
      xCCB.UserNum =0;
      xCCB.InitStatus = 1;
    }
  else{
      xCCB.UserNum =  (uint32_t)UserTable_temp[1024-2];
      xCCB.InitStatus = 1;
  }
//  xCCB->AllCostRecord	总交易信息 涉及 保存，暂不添加
  xCCB.CostMessage = RecentCostDetail;
  return 0;
}


uint32_t AddUser(PayRecord_t  *user)
{
  uint32_t UserTable_temp[1024];
  taskENTER_CRITICAL();
  W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
  taskEXIT_CRITICAL();
  // 搜索重复ID，不能添加
  for (uint32_t i=0;i<MAX_UID;i++)
    {
      if(UserTable_temp[i*4] == user->ID)
	{
	return REPEATID;
	}
    }

  UserTable_temp[ xCCB.UserNum*4] = user->ID;
  UserTable_temp[ xCCB.UserNum*4+1] =  DATA_START_ADDR+4096* xCCB.UserNum;
  user->Addr = DATA_START_ADDR+4096* xCCB.UserNum;
  xCCB.UserNum+=1;
  UserTable_temp[1022] =  xCCB.UserNum;
  taskENTER_CRITICAL();
  W25QXX_Write((uint8_t*)UserTable_temp,0,4096);
  memset(UserTable_temp,0,sizeof(UserTable_temp));
  UserTable_temp[1001]=user->Remain;
  W25QXX_Write((uint8_t*)UserTable_temp,user->Addr,4096);
  taskEXIT_CRITICAL();
  return 0;
}

uint32_t DelAllUser(uint32_t addr)
{
  uint32_t UserTable_temp[1024];
  memset(UserTable_temp,0,4096);
  taskENTER_CRITICAL();
  W25QXX_Write((uint8_t*)UserTable_temp,0,4096);
  taskEXIT_CRITICAL();
  return 0;
}

uint32_t DelOneUser(PayRecord_t *user)
{
  uint32_t UserTable_temp[1024];
  taskENTER_CRITICAL();
  W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
  taskEXIT_CRITICAL();
  for (uint32_t i=0;i<MAX_UID;i++)
    {
      if(UserTable_temp[i*4] == user->ID)
	{
	  UserTable_temp[i*4] = 0x00000000;
	  taskENTER_CRITICAL();
	  W25QXX_Write((uint8_t*)UserTable_temp,0,4096);
	  taskEXIT_CRITICAL();
	  return 0;
	}
    }
  return 1;
}


uint32_t CheckUser(PayRecord_t  *user)
{
  uint32_t UserTable_temp[1024];
  uint32_t i=0;
  if(user->ID == 0)
    return NOSUCHID;
  else{
      taskENTER_CRITICAL();
      W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
      taskEXIT_CRITICAL();
      for(i=0;i<MAX_UID;i++)
	{
	  if(UserTable_temp[i*4]!=user->ID)
	    {
	      continue;
	    }
	  else
	    break;
	}
      if(i >= MAX_UID)
	return NOSUCHID;
      user->Addr = UserTable_temp[i*4+1];
      taskENTER_CRITICAL();
      W25QXX_Read((uint8_t*)UserTable_temp,user->Addr,4096);
      taskEXIT_CRITICAL();
      user->Offset = UserTable_temp[1000];
      user->Remain = UserTable_temp[1001];
      return 0;
  }
}

uint32_t CostUser(PayRecord_t  *user,uint32_t cost)
{
  uint32_t UserTable_temp[1024];

  if((user->Addr == 0)||((user->Addr)%0x1000)!=0)
    CheckUser(user);
  taskENTER_CRITICAL();
  W25QXX_Read((uint8_t*)UserTable_temp,(user->Addr),4096);
  taskEXIT_CRITICAL();
  if(((cost & 0x80000000)==0))
    {
     if((cost)>user->Remain)
       {
	 return BALANCECHANGE;
       }
     else
       {
	user->Remain -=cost;
	xCCB.CostRecord++;
	xCCB.CostMoney += cost;
       }
    }
  else
    {
      user->Remain +=(cost&0x7FFFFFFF);
    }

  UserTable_temp[user->Offset] = cost;
  user->Offset +=2;
  UserTable_temp[1000]=user->Offset;
  UserTable_temp[1001] = user->Remain;
  taskENTER_CRITICAL();
  W25QXX_Write((uint8_t*)UserTable_temp,user->Addr,4096);
  taskEXIT_CRITICAL();
  return 0;
}

inline uint32_t ChargeUser(PayRecord_t  *user,uint32_t cost)
{
	return (CostUser(user,(cost|0x80000000)));
}

//
//uint32_t CheckCostMsg(PayRecord_t  *user,uint32_t list);	//涉及 刷卡，显示
//uint32_t DelCostMsg(PayRecord_t  *user);
