/*
 * App_Pay.c
 *
 *  Created on: 2019年3月21日
 *      Author: prejoy
 */

#include "App_Pay.h"


u8 UserTable[((MAX_UID-1)/256+1)*4096];
u32  UserTable_NUM=0;

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
      UserTable_NUM =0;
    }
  else{
      UserTable_NUM =  (uint32_t)UserTable_temp[1024-2];
  }
  return 0;
}


uint32_t AddUser(PayRecord_t  *user)	//重复ID检测，未实现
{
  uint32_t UserTable_temp[1024];

  W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
  // 搜索重复ID，不能添加
  for (uint32_t i=0;i<MAX_UID;i++)
    {
      if(UserTable_temp[i*4] == user->ID)
	return 1;
    }

  UserTable_temp[UserTable_NUM*4] = user->ID;
  UserTable_temp[UserTable_NUM*4+1] =  DATA_START_ADDR+4096*UserTable_NUM;
  user->Addr = DATA_START_ADDR+4096*UserTable_NUM;
  UserTable_NUM+=1;
  UserTable_temp[1022] = UserTable_NUM;
  W25QXX_Write((uint8_t*)UserTable_temp,0,4096);

  memset(UserTable_temp,0,sizeof(UserTable_temp));
//  W25QXX_Read((uint8_t*)UserTable_temp,user->Addr,4096);
//  UserTable_temp[1000]= user->Offset = 0x00000000;
  UserTable_temp[1001]=user->Remain;
  W25QXX_Write((uint8_t*)UserTable_temp,user->Addr,4096);
  return 0;
}

uint32_t DelAllUser(uint32_t addr)
{
  uint32_t UserTable_temp[1024];
  memset(UserTable_temp,0,4096);
  W25QXX_Write((uint8_t*)UserTable_temp,0,4096);
  return 0;
}

uint32_t DelOneUser(PayRecord_t *user)
{
  uint32_t UserTable_temp[1024];
  W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
  for (uint32_t i=0;i<MAX_UID;i++)
    {
      if(UserTable_temp[i*4] == user->ID)
	{
	  UserTable_temp[i*4] = 0x00000000;
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
    return 1;
  else{
      W25QXX_Read((uint8_t*)UserTable_temp,0,4096);
      for(i=0;i<MAX_UID;i++)
	{
	  if(UserTable_temp[i*4]!=user->ID)
	    {
	      continue;
	    }
	  else
	    break;
	  return 1;
	}
      user->Addr = UserTable_temp[i*4+1];

      W25QXX_Read((uint8_t*)UserTable_temp,user->Addr,4096);
      user->Offset = UserTable_temp[1000];
      user->Remain = UserTable_temp[1001];
      return 0;
  }
}

uint32_t CostUser(PayRecord_t  *user,uint32_t cost)
{
  uint32_t UserTable_temp[1024];
  W25QXX_Read((uint8_t*)UserTable_temp,(user->Addr),4096);
  UserTable_temp[user->Offset] = cost;
  user->Offset +=2;
  UserTable_temp[1000]=user->Offset;
  if((cost & 0x80000000)==0) {
      user->Remain -=cost;
  }else{
      user->Remain +=cost;
  }
  UserTable_temp[1001] = user->Remain;
  W25QXX_Write((uint8_t*)UserTable_temp,user->Addr,4096);

  return 0;
}
